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

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel

float4 main( v2p_aa_AA I ):SV_Target
{
	float4 color = s_image.Sample(smp_rtlinear, I.Tex0.xy);
	float4 bloom = s_bloom.Sample(smp_rtlinear, I.Tex0.xy);

	color.rgb = 1.f - (1.f - color.rgb) * (1.f - bloom.xyz);
	return color;
}
