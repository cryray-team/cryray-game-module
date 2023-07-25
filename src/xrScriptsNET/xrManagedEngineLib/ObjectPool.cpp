#include "stdafx.h"
#include "ObjectPool.h"
#include "ClassRegistrator.h"

using namespace System::Linq::Expressions;
using namespace System::Reflection;

UInt32 XRay::ObjectPool::CreateObject(IntPtr pDllPure)
{
	ConditionalInitialize();
	DLL_Pure* DllPure = reinterpret_cast<DLL_Pure*>(pDllPure.ToPointer());
	Type^ ClassType = ClassRegistrator::GetTypeForClassId(DllPure->CLS_ID);

	//#TEMP: Uncomment when spectre will be in active development
    if (!pStructCryRay->csharp_class_reg_debug)
    {
        if (ClassType == nullptr)
            return 0;
    }
    else
    {
        if (ClassType == nullptr)
        {
            string16 temp;
            CLSID2TEXT(DllPure->CLS_ID, temp);
            string256 Msg;
            xr_strconcat(Msg, "There is no class type: ", temp);
            XRay::Log::Warning(gcnew String(Msg));
            return 0;
        }
    }

	// Get compiled lambda activator for that type
	System::Object^ Activator = nullptr;
	if (!ConstructorCache->TryGetValue(ClassType, Activator))
	{
		Activator = CreateFastObjectActivator(ClassType);
		ConstructorCache->Add(ClassType, Activator);
	}

	IntPtr BoxedPtr(DllPure);

	List< XRay::NativeObject^>^ ObjectList = nullptr;
	if (!GlobalRegistry->TryGetValue(BoxedPtr, ObjectList))
	{
		ObjectList = gcnew List<XRay::NativeObject ^>();
		GlobalRegistry->Add(BoxedPtr, ObjectList);
	}

	// Call activator to create a newer object
	XRay::NativeObject^ CreatedObject = nullptr;
	ObjectActivator^ NativeActivator = dynamic_cast<ObjectActivator^>(Activator);
	ScriptedObjectActivator^ ScriptActivator = dynamic_cast<ScriptedObjectActivator^>(Activator);

	XRay::ClassRegistrator::SetFactoryTarget(BoxedPtr);
	CreatedObject = ScriptActivator == nullptr ? NativeActivator(BoxedPtr) : ScriptActivator();
	XRay::ClassRegistrator::ResetFactoryTarget();

	ObjectList->Add(CreatedObject);

	IndexRegistry->Add(++IndexCounter, CreatedObject);

	return IndexCounter;
}

void XRay::ObjectPool::DestroyObject(IntPtr pDllPure)
{
	List< XRay::NativeObject^>^ ObjectList = nullptr;
	DLL_Pure* DllPure = reinterpret_cast<DLL_Pure*>(pDllPure.ToPointer());
	if (GlobalRegistry->TryGetValue(pDllPure, ObjectList))
	{
		for each (XRay::NativeObject^ NativeObj in ObjectList)
		{
			NativeObj->OnDestroyed();
		}
		ObjectList->Clear();
		GlobalRegistry->Remove(pDllPure);
		IndexRegistry->Remove(DllPure->SpectreObjectId);
	}
}

void XRay::ObjectPool::DestroyAllObjects()
{
	array<UInt32>^ AllObjectHandles = gcnew array<UInt32>(IndexRegistry->Count);
	IndexRegistry->Keys->CopyTo(AllObjectHandles, 0);

	for each (UInt32 ObjHandle in AllObjectHandles)
	{
		NativeObject^ Obj = GetObjectByHandle(ObjHandle);
		if (Obj != nullptr)
		{
			DestroyObject(Obj->GetNativeObject());
		}
	}
}

XRay::NativeObject^ XRay::ObjectPool::GetObjectByHandle(UInt32 Handle)
{
	XRay::NativeObject^ NatObject = nullptr;
	IndexRegistry->TryGetValue(Handle, NatObject);
	return NatObject;
}

void XRay::ObjectPool::ConditionalInitialize()
{
	if (GlobalRegistry == nullptr)
	{
		GlobalRegistry = gcnew Dictionary<IntPtr, List<XRay::NativeObject ^> ^>();
		ConstructorCache = gcnew Dictionary < Type^, System::Object^ >();
		IndexRegistry = gcnew Dictionary<UInt32, XRay::NativeObject ^>();
		IndexCounter = 0;
	}
}

System::Object^ XRay::ObjectPool::CreateFastObjectActivator(Type^ ClassType)
{
	LambdaExpression^ lambda = nullptr;
	System::Object^ returnValue = nullptr;

	array<Type^>^ noConstructorsParams = gcnew array<Type ^>(0);
	ConstructorInfo^ NativeConstructor = ClassType->GetConstructor(BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance, nullptr, noConstructorsParams, nullptr);
	
	if (NativeConstructor == nullptr) 
	{
		array<Type^>^ constructorsParams = gcnew array<Type ^>(1);
		constructorsParams[0] = IntPtr::typeid;

		NativeConstructor = ClassType->GetConstructor(BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance, nullptr, constructorsParams, nullptr);

		if (NativeConstructor == nullptr)
		{
			throw gcnew Exception("There's no constructor for "+ClassType->ToString());
			return nullptr;
		}

		ParameterExpression^ InNativeObjectParam = Expression::Parameter(IntPtr::typeid);
		NewExpression^ ConstructorExpression = Expression::New(NativeConstructor, InNativeObjectParam);
		
		lambda = Expression::Lambda(ObjectActivator::typeid, ConstructorExpression, InNativeObjectParam);
		returnValue = (ObjectActivator^)lambda->Compile();
	}
	else
	{
		NewExpression^ ConstructorExpression = Expression::New(NativeConstructor);
		lambda = Expression::Lambda(ScriptedObjectActivator::typeid, ConstructorExpression);
		returnValue = (ScriptedObjectActivator^)lambda->Compile();
	}

	return returnValue;
}
