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
#include "ScreenSpaceAddon\h_screenspace_common.hlsl"
#include "ScreenSpaceAddon\h_screenspace_dof.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main(p_screen I) : SV_Target
{
	float3 P = SSFX_get_position(I.tc0.xy, 0.f).xyz;
	float3 img = s_image.Sample(smp_nofilter, I.tc0).rgb;

	return float4(SSFX_DOF( I.tc0, P, img ), 1.f);
}