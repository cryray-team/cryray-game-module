#pragma once
#include "../xrCore/clsid.h"
#include "../xrEngine/std_classes.h"

namespace XRay
{
	public enum class EngineClassIDs : unsigned long long
	{
		HudManager = CLSID_HUDMANAGER,
		GameLevel = CLSID_GAME_LEVEL,
		GamePersistence = CLSID_GAME_PERSISTANT,
		//RenderTarget = CLSID_RENDER,
		GameObject = CLSID_OBJECT
	};
}