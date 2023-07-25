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
float4 main(v2p_aa_AA I) : SV_Target
{
	float4 gasmask_texture = s_gasmask.Sample(smp_nofilter, I.Tex0.xy);

	float2 tc_offset = gasmask_texture.xy * 2.f - 1.f;

	return float4(tc_offset,0.f,gasmask_texture.w);
}