#include "stdafx.h"
#include "Environment.h"
#include "../xrEngine/Environment.h"
#include "../xrEngine/IGame_Persistent.h"

XRay::MEnvironment::MEnvironment(::System::IntPtr InNativeLevel)
{
	CAST_TO_NATIVE_OBJECT(CEnvironment, InNativeLevel);
}


void XRay::MEnvironment::ChangeGameTime(float fValue)
{
	if (!g_pGamePersistent)
		return;

	auto& Env = g_pGamePersistent->Environment();
	Env.SetGameTime(Env.NormalizeTime((Env.GetGameTime() + fValue)), Env.fTimeFactor);
}


