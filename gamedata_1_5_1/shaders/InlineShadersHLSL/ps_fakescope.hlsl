//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_night_vision.hlsl"
#include "Headers\h_common.hlsl"

float getparallax(float d)
{
	return clamp(1 - pow(1 / fakescope_params3.x * fakescope_params3.y * d, fakescope_params3.z), 0, 1);
}

float2 clampMagnitude(float2 v, float l)
{
	return normalize(v) * min(length(v), l);
}

float4 main(p_screen I,  float4 pos2d : SV_POSITION) : SV_Target
{
	if(m_hud_params.x > 0 && fakescope_params3.x > 0)
	{
		float3 image;
		float2 corrected_texturecoords = aspect_ratio_correction(I.tc0);
		
		// get velocity
		float2 eye_velocity = clampMagnitude((mul(m_VP, eye_direction_lerp) * fakescope_params2.y) + (mul(m_VP, eye_position_lerp) * fakescope_params2.z), fakescope_params2.w);
		
		// parallax
		float2 parallax_offset = float2(0.5f + eye_velocity.x, 0.5f - eye_velocity.y);
		float distToParallax = distance(corrected_texturecoords, parallax_offset);
		
		// scope offset
		float2 scope_center = float2(0.5f, 0.5f);
		float distToCenter = distance(corrected_texturecoords, scope_center);
		float4 scope = s_scope.Sample(smp_nofilter, (I.tc0 * float2(1, 0.5625f)));
		
		if(step(distToCenter, fakescope_params3.x) == 1 && m_hud_params.x >= 1.0)
		{
			I.tc0 = (I.tc0 - 0.5) * fakescope_params1.x - 0.5;
			float3 half_res_blur = s_blur_2.Sample(smp_base, I.tc0).rgb;
			
			image.r = s_image.Sample(smp_base, I.tc0 + (float3(-fakescope_params2.x, 0, 0) * distToCenter)).r;
			image.b = s_image.Sample(smp_base, I.tc0 + (float3( fakescope_params2.x, 0, 0) * distToCenter)).b;
			image.g = s_image.Sample(smp_base, I.tc0).g;

			image = lerp(image, half_res_blur, fakescope_params1.y);
			
			image.rgb *= getparallax(distToParallax);
		}
		else
		{	
			image = s_image.Sample(smp_base, I.tc0).xyz;
			float3 half_res_blur = s_blur_2.Sample(smp_base, I.tc0).rgb;
			image = lerp(image, half_res_blur, m_hud_params.x * fakescope_params1.z) * (1 - (m_hud_params.x * (1 - fakescope_params1.w)));
		}
		
		if(m_hud_params.x >= 1.0)
		{
			image = lerp(image, scope, scope.a);
		}
		
		return float4(image, 1.0);
	}
	else
	{
		return float4(s_image.Sample(smp_base, I.tc0).xyz, 1.0);
	}
} 
