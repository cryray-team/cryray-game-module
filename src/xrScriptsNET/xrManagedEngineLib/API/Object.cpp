#include "stdafx.h"
#include "Object.h"

#include "ClassRegistrator.h"

void XRay::Object::shedule_update(UInt32 deltaTime)
{

}

XRay::Object::Object(IntPtr InNativeObject)
	: NativeObject(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CObject, InNativeObject);
	
	AddVirtualMethod("shedule_update");
}

XRay::Object::Object() : Object(XRay::ClassRegistrator::GetFactoryTarget())
{

}
