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

float4 main(v2p_aa_AA I) : SV_Target
{
	static const float weight[11] =
		{
			0.082607f,
			0.080977f,
			0.076276f,
			0.069041f,
			0.060049f,
			0.050187f,
			0.040306f,
			0.031105f,
			0.023066f,
			0.016436f,
			0.011254f
		};

    float3 result = 0.f;

	[unroll] for (int i = -10; i < 11; i++)
		result += s_bloom.Sample(smp_rtlinear, I.Tex0.xy + (direction * i) * screen_res.zw * 9.f).rgb * weight[abs(i)];

    return float4(result, 1.f);
}