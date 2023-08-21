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

struct	_input
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float4 pos2d : SV_Position;
};

float4 main ( _input I ): SV_Target
{
    float3 result = 0.f;

	[unroll] for (int i = -4; i < 5; i++)
		result += s_ambient_occlusion_temp.Sample(smp_rtlinear, I.tc0.xy + ((blur_params.xy  * (1.f / pos_decompression_params2.xy)) * i));

    return float4(result/9.f, 0.f);
}