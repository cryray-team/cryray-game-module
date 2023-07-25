#include "stdafx.h"
#include "PhysicalGameObject.h"
#include "../xrGame/PhysicsShellHolder.h"
#include "../xrPhysics/PhysicsShell.h"

using XRay::PhysicalGameObject;

PhysicalGameObject::PhysicalGameObject(IntPtr InNativeObject) : XRay::GameObject(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CPhysicsShellHolder, InNativeObject);
}

bool PhysicalGameObject::IsPhysicsEnabled::get()
{
	return pNativeObject->PPhysicsShell()->isEnabled();
}

void PhysicalGameObject::IsPhysicsEnabled::set(bool value)
{
	if (value)
		pNativeObject->PPhysicsShell()->Enable();
	else
		pNativeObject->PPhysicsShell()->Disable();
}

float PhysicalGameObject::Mass::get()
{
	return pNativeObject->PPhysicsShell()->getMass();
}

void PhysicalGameObject::Mass::set(float value)
{
	pNativeObject->PPhysicsShell()->setMass(value);
}