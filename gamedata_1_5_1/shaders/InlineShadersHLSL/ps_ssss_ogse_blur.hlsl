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
#include "Headers\h_wmark.hlsl"

#define SS_LENGTH float(1.f) // длина лучей. Чем больше длина, тем меньше производительность

#if SUN_SHAFTS_QUALITY == 1
	#define num_iter int(35)
#elif SUN_SHAFTS_QUALITY == 2
	#define num_iter int(45)
#elif SUN_SHAFTS_QUALITY == 3
	#define num_iter int(55)
#endif

float4 main(p_screen I): SV_Target
{
	// Prepare some constants
	float	len 				= SS_LENGTH*0.2f;
	float	NUM 				= num_iter * len;

	float2	tc 					= I.tc0.xy;
	// Distance to the sun
	float	sun_dist 			= FARPLANE / (sqrt(1.f - L_sun_dir_w.y * L_sun_dir_w.y));
	// Sun pos
	float3	sun_pos_world 		= sun_dist * L_sun_dir_w + eye_position;
	float4	sun_pos_projected 	= mul(m_VP, float4(sun_pos_world, 1.f));
	float4	sun_pos_screen 		= proj_to_screen(sun_pos_projected)/sun_pos_projected.w;
	// Sun-pixel vector
	float2	sun_vec_screen 		= sun_pos_screen.xy - tc;
	// Calculate filtering effect. Sunshafts must appear only when looking in the sun dir
	float	angle_cos 			= dot(-eye_direction, normalize(L_sun_dir_w));
	float	fAspectRatio 		= 1.333f * screen_res.y / screen_res.x;
	float	ray_fade 			= saturate(saturate(angle_cos)*saturate(1.f - saturate(length(sun_vec_screen * float2(1, fAspectRatio)) / (SS_LENGTH * ssss_params.w))));
		
	float4	depth 				= s_sun_shafts.Load(int3(tc * screen_res.xy, 0), 0);
	// Adjust sampling
	sun_vec_screen *= angle_cos * ssss_params.z * ssss_params.y * len;
	// Sampling image along ray
	float4 accum = depth;
    
	[unroll]
	for (int i = 1; i < NUM; i++)
	{ 
		tc += sun_vec_screen;
		depth = s_sun_shafts.Load(int3(tc * screen_res.xy, 0), 0);
		accum.xyz += depth.xyz * (1.f - i / NUM);
	}
	accum /= NUM;

	float4	Color 		= accum * 2.f * float4(ray_fade.xxx, 1.f);
			Color.w    += (saturate(angle_cos*0.1f + 0.9f) - 0.99999f);
	
	return Color;
}