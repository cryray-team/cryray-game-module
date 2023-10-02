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
#include "ScreenSpaceAddon\h_screenspace_sky_debanding.hlsl"

#ifndef ISAMPLE
#define ISAMPLE 0
#endif

float4 main(v2p_aa_AA I) : SV_Target
{
	gbuffer_data gbd	= gbuffer_load_data(GLD_P(I.Tex0.xy, I.HPos.xy, ISAMPLE));
	float4 ldr_color 	= s_image.Sample(smp_rtlinear, I.Tex0);	

  	float depth 		= gbd.P.z;
	
	if (depth <= SKY_EPS)
		ldr_color.rgb = ssfx_debanding(ldr_color.rgb, I.Tex0);

	return ldr_color;
}