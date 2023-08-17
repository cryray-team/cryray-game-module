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

float4 main(p_screen I) : SV_Target
{
	float4	rain_drops_distortion 	= s_static_drops.Sample(smp_rtlinear, I.tc0.xy);
	float2	texcoord_offset 		= (rain_drops_distortion.xy - (float2(129.f, 131.f) / 255.f)) * def_distort;
	float2	texcoord 				= I.tc0.xy + texcoord_offset * saturate(static_drops_param.x);
	float3 	scene 					= s_image.Sample(smp_rtlinear, texcoord).xyz;

	return float4(scene, 1.f);
}
