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

Texture2D s_image_hit;
uniform float4 screen_effects_1;

static float HitPowerGetCoeff()
{
    return screen_effects_1.x;
}
    
//static const float ... = screen_effects_1.y; //-' Not active
//static const float ... = screen_effects_1.z; //-' Not active
//static const float ... = screen_effects_1.w; //-' Not active

float4 main(v2p_aa_AA I) : SV_Target
{
    float2 center = I.Tex0.xy;
    float4 img = s_image_hit.Sample(smp_rtlinear, center);
    float4 final = img;
    
    static float ConstAmountClamp = 100.f;
    static float ConstHit1 = (HitPowerGetCoeff() / 50.f) * 0.33f;
    static float ConstHit2 = (HitPowerGetCoeff() / 60000.f) * 0.33f;

    //-' Hit effector: --/> START
	if (optShaderHit)
	{
		float lerp_amount = saturate(1.f - length(center - 0.5f));
		float red_boost = lerp(ConstHit1, ConstHit2, lerp_amount);
		
		red_boost = lerp(red_boost, 0.f, (ConstHit1 / ConstAmountClamp));

		final.r = (s_image_hit.Sample(smp_rtlinear, center).r) + red_boost;

		final.g = s_image_hit.Sample(smp_rtlinear, center).g;
		
		final.rgb *= 1.f - (ConstHit1 * saturate(distance(center.xy, float2(0.5f, 0.5f))));
	}
	//-' Hit effector: --/> END

    return final;
}