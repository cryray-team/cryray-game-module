#include "stdafx.h"
#include "ClassRegistrator.h"

void XRay::ClassRegistrator::Register(Type^ UserClass, UInt64 BaseClassID)
{
	ConditionalInitialize();

	if (sInstance->UserDefinitions.ContainsKey(BaseClassID))
	{
		sInstance->UserDefinitions.Remove(BaseClassID);
	}
	sInstance->UserDefinitions.Add(BaseClassID, UserClass);
}

System::Type^ XRay::ClassRegistrator::GetTypeForClassId(UInt64 ClassID)
{
	::System::Type^ Result = nullptr;
	sInstance->UserDefinitions.TryGetValue(ClassID, Result);
	return Result;
}

System::IntPtr XRay::ClassRegistrator::GetFactoryTarget()
{
	return currentTarget;
}

void XRay::ClassRegistrator::SetFactoryTarget(IntPtr InNativeObject)
{
	currentTarget = InNativeObject;
}

void XRay::ClassRegistrator::ResetFactoryTarget()
{
	SetFactoryTarget(IntPtr::Zero);
}

void XRay::ClassRegistrator::ConditionalInitialize()
{
	if (sInstance == nullptr)
	{
		sInstance = gcnew ClassRegistrator();

		// Initialize basic class store
		sInstance->LoadBaseClassDefinitions();
	}
}

void XRay::ClassRegistrator::LoadBaseClassDefinitions()
{
	//#TODO: NYI!
}