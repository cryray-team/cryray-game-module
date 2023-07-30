//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#define USE_LM_HEMI

#include "Headers\common.h"
#include "Headers\sload.h"
#include "ScreenSpaceAddon\settings_screenspace_ssr.h"
#include "ScreenSpaceAddon\settings_screenspace_puddles.h"
#include "ScreenSpaceAddon\screenspace_common_ripples.h"

#define	USE_4_DETAIL

#if defined(USE_TDETAIL) && defined(USE_4_DETAIL)
#	define	USE_4_BUMP
#endif

#ifdef USE_4_BUMP
f_deffer 	main	( p_bumped I 	)
#else
f_deffer 	main	( p_flat I 		)
#endif
{
	f_deffer	O;

 	// diffuse
	float4 D		= tbase		(I.tcdh);			// IN:  rgb.a
	float4 L 	= s_lmap.Sample( smp_base, I.tcdh);
	float G 	= 0.001f;
#ifdef	USE_TDETAIL
 #ifdef USE_4_DETAIL
	float4	mask= s_mask.Sample ( smp_base, I.tcdh);
	float 	mag	= dot 		(mask,1.f);
			mask= mask/mag	;

  #ifdef USE_4_BUMP
 	 float4	n_Rt = s_dn_r.Sample ( smp_base, I.tcdbump).wzyx;
	 float4	n_Gt = s_dn_g.Sample ( smp_base, I.tcdbump).wzyx;
	 float4	n_Bt = s_dn_b.Sample ( smp_base, I.tcdbump).wzyx;
	 float4	n_At = s_dn_a.Sample ( smp_base, I.tcdbump).wzyx;
	 
 	 float3	n_R = (n_Rt-0.5f)*mask.r; float g_R=n_Rt.w*mask.r;
	 float3	n_G = (n_Gt-0.5f)*mask.g; float g_G=n_Gt.w*mask.g;
	 float3	n_B = (n_Bt-0.5f)*mask.b; float g_B=n_Bt.w*mask.b;
	 float3	n_A = (n_At-0.5f)*mask.a; float g_A=n_At.w*mask.a;

	 float3	mix		= 	n_R+n_G+n_B+n_A;
			mix	   *=	float3(G_SSR_TERRAIN_BUMP_INTENSITY, G_SSR_TERRAIN_BUMP_INTENSITY, 1); // Adjust bump strength
	 float3	N		= 	mul     	(float3x3(I.M1, I.M2, I.M3), mix.xyz);

	// Puddles Implementation - SSS Update 14.6
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/

	// Wetness factor
#ifdef LAUNCHER_OPT_SSR_G_PUDDLES_RIPPLES
	float wetness_f = rain_params.y;
#else
	float wetness_f = 0.f;
#endif

	// Always render puddles if G_PUDDLES_ALLWAYS is defined
	#ifdef G_PUDDLES_ALLWAYS
		wetness_f = 1.f;
	#endif

	// Intensity factor.
	float puddles_f = wetness_f;

	// Size factor. Convert 0 ~ 1 to 1 ~ -0.1f
	float size_f = (1.f - G_PUDDLES_SIZE * 1.1f);

	// Texture mask
	float puddles_mask = 1.f - s_puddles_mask.Sample(smp_base, I.tcdh).r;

	// Puddles perlin
	float puddles = saturate((s_puddles_perlin.Sample(smp_base, I.tcdh * 15.f / G_PUDDLES_GLOBAL_SIZE).r - size_f) * puddles_f);

	// Get normals and transform space to create a slope mask
	float3 N1 = mul(float3x3(I.M1, I.M2, I.M3), float3(0,0,1));
	float3 N2 = mul(m_inv_V, normalize(N1));

	// Slope mask
	float slope = max(N2.x, N2.y);

	// Slope adjustements... Some magic numbers
	slope = saturate((slope - 0.997f) * (102.f + 0.997f));

	// Slope & texture mask defines final puddles intensity
	puddles *= slope * puddles_mask;

	// Puddles border hardness
	puddles = smoothstep(0, saturate(0.3f - G_PUDDLES_BORDER_HARDNESS * 0.3f), puddles);

#ifdef G_PUDDLES_RIPPLES

	float rain_int = saturate(rain_params.x * 1.5f);

	// Base ripples anim speed
	float ripples_anim =  timers.x * ((0.01f + (rain_int * 0.008f)) * G_PUDDLES_RIPPLES_SPEED );
	
	// Base ripples scale
	float ripples_scale = 140.f / G_PUDDLES_RIPPLES_SCALE;

	// Base ripples Normal
	float3 WN0 = s_puddles_normal.Sample(smp_base, I.tcdh * ripples_scale + float2(0.f, ripples_anim));
	float3 WN1 = s_puddles_normal.Sample(smp_base, I.tcdh * ripples_scale - float2(0.33f, ripples_anim));
	float3 ripplesNormal = ((WN0 * 2.f - 1.f) + (WN1 * 2.f - 1.f)) / 2.0f;

	// Rain Ripples
	float3 rainRipples = SSFX_ripples( s_rainsplash, I.tcdh * ripples_scale * 2.8f / G_PUDDLES_RAIN_RIPPLES_SCALE ) * 2.f - 1.f;

	// Rain ripples intensity... ( Rain intensity * Depth fadeout * Intensity )
	rainRipples *= clamp(1.5f - I.position.z * 0.15f, 0.3f, 1.0f) * rain_params.x * 3.0f * G_PUDDLES_RAIN_RIPPLES_INTENSITY;

	// Base ripples Intensity
	ripplesNormal *= (0.05f + rain_int * G_PUDDLES_RIPPLES_RAINING_INT) * G_PUDDLES_RIPPLES_INTENSITY;
	
	// Mix ripples
	ripplesNormal = ( ripplesNormal * 0.666f ) + ( rainRipples * 0.333f );

#else
	float3 ripplesNormal = 0;
#endif

	// Normal Up and ripples for puddles
	float3 MirrorUp	= mul(m_V,  float3(ripplesNormal.x, 1.f, ripplesNormal.y));
	N = lerp(N, MirrorUp, puddles);

	// Refraction Normal
	float3 N_refra = ripplesNormal;

	// Adjust refraction intensity based on wetness and puddle perlin
	N_refra.xy *= 0.15f * smoothstep( 0.6f, 0.8f, wetness_f * puddles ) * G_PUDDLES_REFRACTION_INTENSITY;

	// Detail textures + refraction when needed
	float2 detail_tc = I.tcdbump + N_refra;
	float3 d_R = s_dt_r.Sample ( smp_base, detail_tc) * mask.r;
	float3 d_G = s_dt_g.Sample ( smp_base, detail_tc) * mask.g;
	float3 d_B = s_dt_b.Sample ( smp_base, detail_tc) * mask.b;
	float3 d_A = s_dt_a.Sample ( smp_base, detail_tc) * mask.a;
	float3 dt = d_R + d_G + d_B + d_A;
	D.rgb = 2 * D.rgb * dt;

	// Apply tint
	D.rgb = lerp( D.rgb, D.rgb * G_PUDDLES_TINT, puddles);

	// Limit pudddle reflection.
	puddles = clamp(puddles, 0.f, G_PUDDLES_REFLECTIVITY);

	// Add at the end the extra wetness. Full intensity at 50% of wetness ( rain_params.y )
	puddles = puddles + saturate(rain_params.y * 2.f) * G_PUDDLES_TERRAIN_EXTRA_WETNESS;

	// Add gloss and puddles to the final gloss result
	G = max(g_R + g_G + g_B + g_A, puddles);
#else
	float3	N 	= I.N.xyz	;
#endif

#else
	D.rgb	= 2.f*D.rgb*s_detail.Sample( smp_base, I.tcdbump).rgb;
#endif
 
#else
	float3	N 	= I.N.xyz	;
#endif

	// hemi, sun, material
	float 	ms	= xmaterial	;

  // 2. Standart output
  float4     Ne = float4	(normalize(N), 									D.w			);
  O				= pack_gbuffer( 
								Ne,	// hemi
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f, 	0.95f		),	//
								float4	(D.x,	D.y,	D.z,						G			) );	// OUT: rgb.gloss

  return O;
}
