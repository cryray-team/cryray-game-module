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

#define GAS_MASK_DISTORTION_INTENSITY 0.03

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main(v2p_aa_AA I) : SV_Target
{
	float4 gasmask_texture = s_gasmask_normal.Sample(smp_nofilter, I.Tex0.xy) * GAS_MASK_DISTORTION_INTENSITY;
	
	float4 color = s_image.Sample(smp_rtlinear, I.Tex0.xy + gasmask_texture.xy );
	float4 finalcolor = s_image.Sample(smp_rtlinear, I.Tex0.xy);
	float4 visor_effect = visor_reflect(finalcolor*5.f, I.Tex0.xy).xyzz;

	return lerp(color, finalcolor, abs(color) < Epsilon) + (visor_effect* gasmask_texture.w*5);
}