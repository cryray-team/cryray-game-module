#pragma once
#include "API/PhysicalGameObject.h"
class CEntity;

namespace XRay
{
	public ref class Entity : public PhysicalGameObject
	{
		CEntity* pNativeObject;
	public:
		Entity(IntPtr InNativeObject);

		property float Health
		{
			float get();
			void set(float value);
		}

		property float MaxHealth
		{
			float get();
			void set(float value);
		}

		property bool IsJumping
		{
			bool get();
		}

		property bool IsFalling
		{
			bool get();
		}

		property bool IsCrouching
		{
			bool get();
		}

		property bool IsSprinting
		{
			bool get();
		}

		property bool IsAlive
		{
			bool get();
		}

		property int Team
		{
			int get();
			void set(int value);
		}

		property int Squad
		{
			int get();
			void set(int value);
		}

		property int Group
		{
			int get();
			void set(int value);
		}

		property float Morale
		{
			float get();
			void set(float value);
		}

		// Will be refactored later
		void ChangeTeam(int team, int squad, int group);
	};
}