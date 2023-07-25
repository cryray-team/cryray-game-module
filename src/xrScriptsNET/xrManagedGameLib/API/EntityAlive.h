#pragma once
#include "PhysicalGameObject.h"
class CEntityAlive;

namespace XRay
{
	public ref class EntityAlive : public PhysicalGameObject
	{
		CEntityAlive* pNativeObject;

	public:
		EntityAlive(IntPtr InNativeObject);
	};
}