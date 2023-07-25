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
#include "Headers\gauss.h" //gaussian blur

#define MASK_BLUR_SAMPLES int(12)

float4 main(p_screen I) : SV_Target
{
	float4 outColor = float4(0.f,0.f,0.f,0.f);
	for (int i = 1; i < MASK_BLUR_SAMPLES; i++)
	{
		outColor += Gauss(s_mask, I.tc0.xy, i, false);
	}
	outColor /= MASK_BLUR_SAMPLES;
	return outColor;
}