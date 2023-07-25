#include "stdafx.h"
#include "WeatherFX.h"
#include "..\xrGame\Level.h"
#include "..\xrEngine\Environment.h"
#include "..\xrEngine\IGame_Persistent.h"

using XRay::WeatherFX;

bool WeatherFX::Enable::get()
{
    if (g_pGamePersistent != nullptr)
		return g_pGamePersistent->Environment().bWFX;

	return false;
}

void WeatherFX::Create(::System::String^ str)
{
    if (g_pGamePersistent != nullptr) 
	{
		string128 WetNameStr = { 0 };
		ConvertDotNetStringToAscii(str, WetNameStr);
        g_pGamePersistent->Environment().SetWeatherFX(WetNameStr);
	}
}

void WeatherFX::Stop()
{ 
	if (g_pGamePersistent != nullptr) 
		g_pGamePersistent->Environment().StopWFX(); 
}