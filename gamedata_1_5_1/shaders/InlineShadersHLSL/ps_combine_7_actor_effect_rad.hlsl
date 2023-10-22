//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Author: OldSerpskiStalker7777
//-' When using this shader
//-' Specify copyrights and developments
//-' Respect the work of others

//-' Hozar_2002
//-' [Realization of the radiation effect]
//-' https://bartwronski.com/2016/10/30/dithering-part-three-real-world-2d-quantization-dithering/
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

Texture2D s_image_rad;
uniform float4 screen_effects_2;

struct RadEffectConsts
{
    static const float RadiationEffectSize = 30.f;
    static const float RadiationEffectPower = 5.25f;
	static const float RadiationEffectPowerNoise = 1.6f;
	static const float RadiationEffectPowerInRadZone = 0.055f;
};

static float RadPowerGetCoeff()
{
    return screen_effects_2.x;
}

static float DistToRadZone()
{
    return screen_effects_2.y;
}

//static const float ... = screen_effects_2.z; //-' Not active
//static const float ... = screen_effects_2.w; //-' Not active

float RandomInRange()
{
    return 1.5f + frac(sin(dot(float2(37.2f, 42.5f), float2(13.5f, 17.7f))) * 0.5f + 0.5f);
}

float rand_IGN(float2 vpos, float rnd_offset = 0.f) 
{
	float3 magic = float3(61.11056f, 5.83715f, 5292.9189f);
	return frac(magic.z * frac(dot(vpos.xy + rnd_offset, magic.xy)));
}

float4 generate_green_points(float2 uv, float frequency, float amplitude, float spacing)
{
	float seed = timers.x;

	uv.x *= (screen_res.x * screen_res.w) / (1.2f * RandomInRange());
	uv.x += timers.x * 0.14f;
	
	float2 grid_uv = (round(uv * spacing)) / spacing;

	float factor = smoothstep(0.09f, 0.014f, distance(uv, grid_uv) * spacing);

	float noise_value = rand_IGN(grid_uv, rand(seed) * 0.1f);

    return float4(0.f, noise_value, 0.f, factor);
}

float4 main(v2p_aa_AA I) : SV_Target
{
    float2 center = I.Tex0.xy;
    float4 img = s_image_rad.Sample(smp_rtlinear, center.xy);
	float4 final = img;

    float fRadClamp = clamp(RadPowerGetCoeff(), 0.f, 1.f) * RadEffectConsts::RadiationEffectPower;
	float fDistToRadZone = clamp(DistToRadZone(), 0.f, 20.f) * RadEffectConsts::RadiationEffectPowerInRadZone;
	float3 radiation = float3(0.f, 0.f, 0.f);

	//-' Radiation effector: --/> START
	if (optShaderRadiation)
	{
		float factor;
		float fade = smoothstep(0.2, 0.95, length(center - float2(0.5f, 0.5f)));
		float3 green_points;
		
		float4 (green_points, factor) = generate_green_points(center, 0.85f, 0.9f, RadEffectConsts::RadiationEffectSize);

		radiation.rgb += (smoothstep(0.9f, 0.99f, (green_points)) * (RadEffectConsts::RadiationEffectPowerNoise * (fRadClamp + fDistToRadZone) * fade * factor));

		final.rgb += radiation;
	}
	//-' Radiation effector: --/> END

    return final;
}