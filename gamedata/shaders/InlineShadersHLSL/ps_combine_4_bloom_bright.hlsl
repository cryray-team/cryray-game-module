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

// First bloom pass, bright pixel getting
//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main(v2p_aa_AA I) : SV_Target
{
	static const int2 offset[4] =
		{
			int2(1.f, 1.f),
			int2(1.f, 1.f),
			int2(-1.f, 1.f),
			int2(-1.f, -1.f)
		};

	float3 bloom = 0.f;

	[unroll(4)] for (int i = 0; i < 4; i++)
	{
		float2 bloomuv = offset[i] * screen_res.zw * 2.f;
		float3 tempbloom = s_image.Sample(smp_rtlinear, I.Tex0.xy + bloomuv.xy).rgb;
		tempbloom = max(0.f, tempbloom - bloom_treshold);
		bloom += tempbloom;
	}
	
	bloom = lerp(dot(bloom, LUMINANCE_VECTOR), bloom, bloom_saturation);
	
	return float4(bloom * 0.85f, 1.f);
}