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

#define SS_DUST_SPEED float(0.4f)		// скорость частиц пыли 
#define SS_DUST_INTENSITY float(2.f)	// яркость пылинок
#define SS_DUST_DENSITY float(1.f)		// плотность частиц пыли 
#define SS_DUST_SIZE float(0.7f)			// размер пылинок

Texture2D jitter_hq;

sampler smp_jitter;

float4 main(p_screen I) : SV_Target
{	
	// Dist to the sun
	float	sun_dist 			= FARPLANE / (sqrt(1.f - L_sun_dir_w.y * L_sun_dir_w.y));
	// Sun pos
	float3	sun_pos_world 		= sun_dist * L_sun_dir_w + eye_position;
	float4	sun_pos_projected 	= mul(m_VP, float4(sun_pos_world, 1.f));
	float4	sun_pos_screen 		= proj_to_screen(sun_pos_projected) / sun_pos_projected.w;
	// sun-pixel vector
	float2	sun_vec_screen 		= normalize(sun_pos_screen.xy - I.tc0.xy);
	// smooth shafts
	float4	cSunShafts 			= s_sun_shafts.Sample(smp_rtlinear, I.tc0.xy);
			cSunShafts 		   *= 0.5f;
			cSunShafts 		   += s_sun_shafts.Sample(smp_rtlinear, I.tc0.xy + sun_vec_screen.yx * screen_res.zw) * 0.25f;
			cSunShafts 		   += s_sun_shafts.Sample(smp_rtlinear, I.tc0.xy - sun_vec_screen.yx * screen_res.zw) * 0.25f;
	float3 img 					= s_image.Sample(smp_rtlinear, I.tc0.xy).xyz;

	float dust_size = 8.f/SS_DUST_SIZE;
	float3 jit;
	float2 jtc = I.tc0.xy;
	float2 sun_dir_e = L_sun_dir_e.xy;
	sun_dir_e /= sin(ogse_c_screen.y);
	sun_dir_e *= ogse_c_screen.x;
	
	jtc.x += sun_dir_e.x;
	jtc.y -= sun_dir_e.y;
	jtc.x = (jtc.x > 1.f)?(jtc.x - 1.f):jtc.x;
	jtc.y = (jtc.y < 0.f)?(1.f - jtc.y):jtc.y;
	
	jit.x = jitter0.Sample(smp_jitter, float2(jtc.x, jtc.y + timers.x*0.01f*SS_DUST_SPEED) * dust_size).x;
	jit.y = jitter1.Sample(smp_jitter, float2(jtc.x + timers.x*0.01f*SS_DUST_SPEED, jtc.y) * dust_size).y;
	jit.z = jitter_hq.Sample(smp_jitter, jtc).x;
	jit.z = saturate(jit.z + SS_DUST_DENSITY - 1.f);
	
	float dust = saturate(jit.x*jit.y*jit.z);
	float len = length(dust);
	dust *= SS_DUST_INTENSITY;
	
	float dep = s_position.Load(int3(I.tc0.xy * screen_res.xy, 0), 0).z;
	if (dep < 0.00001f)
		dep = SKY_DEPTH;
	
	dust = lerp(0.f, dust, (1.f - saturate(dep * 0.2f)) * (1.f - saturate(is_sky(dep))));
	dust += 1.f;
	
	float3 Color =  img + cSunShafts.xyz * L_sun_color.xyz * (1.f - img) * ssss_params.x * dust;

	return float4(Color, 1.f);
}