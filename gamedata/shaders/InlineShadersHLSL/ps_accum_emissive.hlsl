//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\common.h"

float4 main() : SV_Target
{
#ifdef IWP_MODE
	return float4(1.f,1.f,1.f,1.f)*3.f;
#else
	return float4(1.f,1.f,1.f,1.f)*16.f;
#endif
}
