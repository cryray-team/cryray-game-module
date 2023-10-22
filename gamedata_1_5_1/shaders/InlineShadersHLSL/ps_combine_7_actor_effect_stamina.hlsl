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

Texture2D s_image_stamina;
uniform float4 screen_effects_3;

static float StaminaMinusGetCoeff()
{
	return screen_effects_3.x;
}

//static const float ... = screen_effects_3.y; //-' Not active
//static const float ... = screen_effects_3.z; //-' Not active
//static const float ... = screen_effects_3.w; //-' Not active

float4 main(v2p_aa_AA I) : SV_Target
{
	float2 center = I.Tex0.xy;
    float4 img = s_image_stamina.Sample(smp_rtlinear, center);
	float4 final = img;

	static float ConstStaminaMinus = clamp(StaminaMinusGetCoeff(), 0.f, 1.f);

	//-' Stamina effector: --/> START
	if (optShaderStamina)
	{
		float vignette_override = saturate(1.f - length(center - 0.5f) * (1.f - ConstStaminaMinus));
		final.rgb *= vignette_override;
	}	
	//-' Stamina effector: --/> END

	return final;
}
