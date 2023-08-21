//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

//Material table
#define MAT_FLORA 0.15f

// Simple subsurface scattering
float SSS(float3 N, float3 V, float3 L)
{
	float S = saturate(dot(V, -(L + N))) * G_SSS_INTENSITY;
	return S;
}