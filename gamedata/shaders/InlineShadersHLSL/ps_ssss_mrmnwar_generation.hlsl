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

#define NUM_SAMPLES int(11)

//This function is only used in this one file
//that's why it's been moved from common_functions.h

float4 proj2screen(float4 Project)
{
	float4	Screen;
			Screen.x = (Project.x + Project.w) * 0.5f;
			Screen.y = (Project.w - Project.y) * 0.5f;
			Screen.z = Project.z;
			Screen.w = Project.w;
			
	return Screen;
}

float4 main(p_screen I) : SV_Target
{
	float 	sunFar 			= 100.0f / sqrt(1.f - L_sun_dir_w.y * L_sun_dir_w.y);
	float4 	sunProj 		= mul(m_VP, float4(sunFar * L_sun_dir_w+eye_position, 1));
	float4 	sunScreen 		= proj2screen(sunProj)/sunProj.w; // projective 2 screen and normalize
	
	float 	fSign 			= dot(-eye_direction, normalize(L_sun_dir_w));
	float2 	sunVector 		= sunScreen.xy - I.tc0.xy;
	float 	fAspectRatio 	= 1.333f * screen_res.y / screen_res.x;
	
	float 	sunDist 		= saturate(fSign) * saturate(1.f - saturate(length(sunVector * float2(1, fAspectRatio)) * ssss_params.y));
	float2 	sunDir 			= sunVector * ssss_params.x * fSign;
	
	float4 	accum 			= float4(0.f, 0.f, 0.f, 0.f);

	[unroll]
	for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		float4 depth = s_mask_blur.Load(int3((I.tc0.xy + sunDir.xy * i) * screen_res.xy, 0), 0);
		accum += depth*(1 - i/NUM_SAMPLES);
	}
	accum /= NUM_SAMPLES;
	
	float4 	outColor 		= accum * 2.f * float4(sunDist.xxx, 1.f);
			outColor.w 	   += 1.f - saturate(saturate(fSign * 0.1f + 0.9f));
	
	return outColor;
}