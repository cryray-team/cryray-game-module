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
	
#ifndef MSAA_ANTIALIASING_ENABLE
float3	calc_ssdo( float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d )
#else
float3	calc_ssdo( float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d, uint iSample )
#endif
{
	int quality = SSAO_QUALITY + 1;
	float3 occ = 0.f;
	float scale = P.z/10.0f * SSDO_RADIUS;
	for (int a = 1.f; a < quality; ++a)
	{
		scale *= a;
		for (int i = 0; i < 7; i++)
		{
			float3 occ_pos_view = P.xyz + (arr[i] + N) * scale;
			float4 occ_pos_screen = proj_to_screen(mul(m_P, float4(occ_pos_view, 1.f)));
			occ_pos_screen.xy /= occ_pos_screen.w;

#ifdef MSAA_ANTIALIASING_ENABLE
		gbuffer_data gbd = gbuffer_load_data_offset( tc, occ_pos_screen.xy, pos2d, iSample ); // this is wrong - need to correct this
#else
		gbuffer_data gbd = gbuffer_load_data_offset( tc, occ_pos_screen.xy, pos2d ); // this is wrong - need to correct this
#endif
			float screen_occ = gbd.P.z;
		
			screen_occ = lerp(screen_occ, 0.f, is_sky(screen_occ));
			float is_occluder = step(occ_pos_view.z, screen_occ);
			float occ_coeff = saturate(is_occluder + saturate(2.1f - screen_occ) + step(SSDO_DISCARD_THRESHOLD, abs(P.z-screen_occ)));
			occ += occ_coeff.xxx;

			// collect also indirect bonuses. Must be discarded:
			// - samples outside testing hemisphere
			// - backfacing samples
			float3 sample_normal = gbd.N.xyz;//tex2Dproj(s_normal, occ_pos_screen);
			float3 sample_diffuse =  gbd.C.xyz;
			occ += (1.f - is_occluder) * saturate(-dot(sample_normal, N)) * sample_diffuse*SSDO_COLOR_BLEEDING; // сила цвета семплов. Дает более цветные тени, но уменьшает интенсивность эффекта в целом. Для компенсации увеличивайте SSDO_BLEND_FACTOR.
		}
	}
	occ /= (7.f * SSAO_QUALITY);
	
	float grass_tuning = 1.f + SSDO_GRASS_TUNING*is_in_range(float3(P.z, 0.360f, 0.365f));
	occ = saturate(occ*grass_tuning);
	
	return (occ + (1.f - occ)*(1.f - SSDO_BLEND_FACTOR));
}