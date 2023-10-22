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
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

Texture2D s_image_antirad;
uniform float4 screen_effects_5;

struct AntiradEffectConsts
{
	static const float ConstPowerAntiradBlur = 4.5f; 
    static const float ConstPowerAntiradBlurPower = 300.f; 
};

static float AntiradEffectorGetCoeff()
{
    return screen_effects_5.x;
}

//static const float ... = screen_effects_5.y; //-' Not active
//static const float ... = screen_effects_5.z; //-' Not active
//static const float ... = screen_effects_5.w; //-' Not active

float4 main(v2p_aa_AA I) : SV_Target
{
    float2 center = I.Tex0.xy;
    float4 img = s_image_antirad.Sample(smp_rtlinear, center.xy);
    float4 final = img;

    float ConstAntirad = clamp(AntiradEffectorGetCoeff(), 0.f, 300.f);

    //-' Antirad effector: --/> START
	if (optShaderAntirad)
	{
		float2 uv = center - 0.5f; 

		float vignette = length(uv) * (ConstAntirad / 450);
		vignette = saturate(vignette);

		float2 blur_amount = screen_res.zw * AntiradEffectConsts::ConstPowerAntiradBlur;
		
		float3 blurred_img = final.rgb;
		
		[unroll]
		for(int i = -1; i <= 1; i++) 
		{
			[unroll]
			for(int j = -1; j <= 1; j++) 
			{
				float2 offset = float2(i, j) * blur_amount * smoothstep(0, 3, ConstAntirad / 100) + screen_res.zw * 0.5;
				blurred_img += s_image_antirad.Sample(smp_rtlinear, center + offset).rgb;
			}
		}
			
		blurred_img /= 10; //-' Hack
		
		float3 vignette_color = float3(1.0, 0.3, 0.6); //-' rgb
		float3 vignette_img = lerp(blurred_img, final.rgb, vignette);

		float3 frosty_color = float3(1.0, 0.3, 0.6); //-' rgb
		
		float2 chromatic_aberration = float2(0.005, -0.005);

		float r = s_image_antirad.Sample(smp_rtlinear, center + chromatic_aberration).r * smoothstep(0, 3, ConstAntirad / 100);
		float g = s_image_antirad.Sample(smp_rtlinear, center).g;
		float b = s_image_antirad.Sample(smp_rtlinear, center - chromatic_aberration).b * smoothstep(0, 3, ConstAntirad / 100);

		float3 intermediate_result = lerp(vignette_img, (vignette_img + frosty_color * vignette) * 5, ConstAntirad / AntiradEffectConsts::ConstPowerAntiradBlurPower);
		float3 final_img = lerp(intermediate_result, float3(r, g, b), ConstAntirad / AntiradEffectConsts::ConstPowerAntiradBlurPower);
		
		final.rgb = final_img;
	}
	//-' Antirad effector: --/> END

    return final;
}