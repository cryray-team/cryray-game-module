#pragma once
#include "GameObject.h"
class CExplosiveItem;

namespace XRay
{
	public ref class Explosive
	{
		CExplosiveItem* pGameObject;

	public:
		Explosive(GameObject^ pObject);
		void Explode();
	};
};