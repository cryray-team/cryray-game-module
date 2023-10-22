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
#define MBLUR_CLAMP 0.5	
#define MBLUR_MBLUR_NOISE int(2)
#define MBLUR_SAMPLES int(5)  	// число проходов

float4x4 m_current; //Current projection matrix
float4x4 m_previous; //Previous projection matrix
float2 m_blur;	

float4 main(p_screen I) : SV_Target
{
	float3 P = s_position.Sample(smp_nofilter, I.tc0).xyz;

    P.z += (saturate(0.001f - P.z) * 10000.f);
	float4 pos4 = float4(P, 0.01f);

	float4 p_current = mul(m_current, pos4);
	float4 p_previous = mul(m_previous, pos4);
	float2 p_velocity = m_blur * ((p_current.xy / p_current.w) - (p_previous.xy / p_previous.w));
	p_velocity = clamp(p_velocity * 5.f, -MBLUR_CLAMP, +MBLUR_CLAMP);

	float4 final = s_image.Load(int3(I.hpos.xy, 0), 0);
	float samples = 1.f;

	float2 uv = I.hpos.xy * screen_res.zw;

	float dither = saturate(frac(sin(dot(uv, float2(12.9898f, 78.223f))) * 43758.5453f)) * MBLUR_MBLUR_NOISE;

	float mask = saturate(pow(abs(I.tc0.x - 0.5f) * 2.5f, 10.f));

	[unroll] 
	for (int i = -MBLUR_SAMPLES; i <= MBLUR_SAMPLES; ++i)
	{
		float2 coord = uv + p_velocity * (float(i + dither) / 2.f);

		if (coord.x < -0.1f || coord.x > 1.1f || coord.y < -0.1f || coord.y > 1.1f || (p_velocity.x == 0.f &&  p_velocity.y == 0.f))
			break;

		final.rgb += s_image.Sample(smp_rtlinear, coord).rgb * mask;
		samples += mask;
	}

	return final / samples;
}