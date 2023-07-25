#pragma once
#include "API/GameObject.h"

class CPhysicsShellHolder;

namespace XRay
{
	// CPhysicsShellHolder
	public ref class PhysicalGameObject : public GameObject
	{
		CPhysicsShellHolder* pNativeObject;
	public:

		PhysicalGameObject(IntPtr InNativeObject);

		property bool IsPhysicsEnabled
		{
			bool get();
			void set(bool value);
		}

		property float Mass
		{
			float get();
			void set(float value);
		}
	};
}