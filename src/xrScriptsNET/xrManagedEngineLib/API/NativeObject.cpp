#include "stdafx.h"
#include "NativeObject.h"
#include "ObjectPool.h"

#include "ClassRegistrator.h"

using namespace System::Reflection;

XRay::NativeObject^ XRay::NativeObject::Create(IntPtr InNativeObject, Type^ InTargetType)
{
	// First, let's check, if object with this type and ptr already created
 	List<NativeObject^>^ Objects = nullptr;
	NativeObject^ ExistedObject = nullptr;
 
 	if (ObjectPool::GlobalRegistry->TryGetValue(InNativeObject, Objects))
 	{
		for each (NativeObject^ var in Objects)
		{
			if (var->GetType() == InTargetType)
			{
				ExistedObject = var;
				break;
			}
		}
 	}
 
 	if (ExistedObject != nullptr) return ExistedObject;
 
 	// Object not picked, create a new one
	XRay::ClassRegistrator::SetFactoryTarget(InNativeObject);
	array<::System::Object^>^ activationArgs = gcnew array<::System::Object ^>(1);
	activationArgs[0] = InNativeObject;
	NativeObject^ ProxyObject = (NativeObject^)::System::Activator::CreateInstance(InTargetType, activationArgs, nullptr);
 	Objects->Add(ProxyObject);
	XRay::ClassRegistrator::ResetFactoryTarget();
 
 	return ProxyObject;
}

bool XRay::NativeObject::IsValid()
{
	return pNativeObject != nullptr;
}

void XRay::NativeObject::OnDestroyed()
{
	FIRE_EVENT(OnObjectBeingDestroyed);
	pNativeObject = nullptr;
}

System::IntPtr XRay::NativeObject::GetNativeObject()
{
	return IntPtr(pNativeObject);
}

void XRay::NativeObject::AddVirtualMethod(String^ MethodName)
{
	Type^ ThisType = GetType();
	MethodInfo^ Result = ThisType->GetMethod(MethodName);
	if (Result == nullptr)
	{
		String^ Message = String::Format("Can't add virtual method {0} of type {1}", MethodName, ThisType->ToString());
		XRay::Log::Error(Message);
		return;
	}

	UInt32 CRC32 = XRay::Utils::CRC32(MethodName);
	DynamicMethodsTable.Add(CRC32, Result);
}

System::Reflection::MethodInfo^ XRay::NativeObject::GetCachedMethod(UInt32 CRC32)
{
	MethodInfo^ Result = nullptr;
	DynamicMethodsTable.TryGetValue((CRC32), Result);
	return Result;
}

XRay::NativeObject::NativeObject(IntPtr InNativeObject)
{
	pNativeObject = reinterpret_cast<DLL_Pure*> (InNativeObject.ToPointer());
}
