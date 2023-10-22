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

Texture2D s_image_medkit;
uniform float4 screen_effects_4;

struct MedkitEffectConsts
{
	static const float ConstPowerMedkitBlur = 4.5f; 
};

static float HealthEffectorGetCoeff()
{
    return screen_effects_4.x;
}

//static const float ... = screen_effects_4.y; //-' Not active
//static const float ... = screen_effects_4.z; //-' Not active
//static const float ... = screen_effects_4.w; //-' Not active

float4 main(v2p_aa_AA I) : SV_Target
{
    float2 center = I.Tex0.xy;
    float4 img = s_image_medkit.Sample(smp_rtlinear, center.xy);
    float4 final = img;

    float uv = center.x - 0.5f;
    static float ConstMedkit = clamp(HealthEffectorGetCoeff(), 0.f, 300.f);

    //-' Medkit effector: --/> START
	if (optShaderMedkit)
	{
		float vignette = length(uv) * (ConstMedkit / 450.f);
		vignette = saturate(vignette);

		float2 blur_amount = screen_res.zw * MedkitEffectConsts::ConstPowerMedkitBlur;
		
		float3 blurred_img = final.rgb;
		
		[unroll]
		for(int i = -1; i <= 1; i++) 
		{
			[unroll]
			for(int j = -1; j <= 1; j++) 
			{
				float2 offset = float2(i, j) * blur_amount * smoothstep(0.f, 3.f, ConstMedkit / 100.f) + screen_res.zw * 0.5f;
				blurred_img += s_image_medkit.Sample(smp_rtlinear, center + offset).rgb;
			}
		}
			
		blurred_img /= 10.f; //-' Hack
		
		float3 vignette_color = float3(0.f, 0.5f, 0.8f);
		float3 vignette_img = lerp(blurred_img, final.rgb, vignette);

		float3 frosty_color = float3(0.f, 0.5f, 0.8f);
		float3 final_img = lerp(vignette_img, vignette_img + frosty_color * vignette, ConstMedkit / 300.f);

		final.rgb = final_img;
	}
	//-' Medkit effector: --/> END

    return final;
}