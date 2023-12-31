//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

/**
* @ Version: SCREEN SPACE SHADERS - UPDATE 14
* @ Description: Water implementation
* @ Modified time: 2023-01-18 17:54
* @ Author: https://www.moddb.com/members/ascii1457
* @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
*/
					
#include "Headers\h_common.hlsl"
#include "Headers\h_lmodel.hlsl"
#include "Headers\h_fog.hlsl"

//-' Screen space functions
#include "ScreenSpaceAddon\h_screenspace_water.hlsl"
#include "ScreenSpaceAddon\h_screenspace_fog.hlsl"

struct vf
{
	float2	tbase		: TEXCOORD0;	// base
	float4	tnorm0		: TEXCOORD1;	// nm0
	float3	position_w	: TEXCOORD2;	// nm1
	float3	M1			: TEXCOORD3;
	float3	M2			: TEXCOORD4;
	float3	M3			: TEXCOORD5;
	float3	v2point_w	: TEXCOORD6;
	float4	tctexgen	: TEXCOORD7;
	float3  p_world		: TEXCOORD8;
	float4	c0			: COLOR0;
	float	fog			: FOG;
	float4	hpos		: SV_Position;
};

float4 main( vf I ) : SV_Target
{
	// 3d view space pos reconstruction math
	float3 Pv = float3(I.tctexgen.z * (I.hpos.xy * pos_decompression_params.zw - pos_decompression_params.xy), I.tctexgen.z);

	// Normal Textures
	float3	n0	= s_nmap0.Sample(smp_base, I.tnorm0.xy);
	float3	n1	= s_nmap0.Sample(smp_base, I.tnorm0.zw);
	float3	basenormal	= (((n0 * 2.f - 1.f) + (n1 * 2.f - 1.f)) / 2.f);
	basenormal += float3(0.1f, 0.1f, 0.1f); // Offset the normal to "center" the textures again
		
	// Load Data
	float2 PosTc = I.tctexgen.xy / I.tctexgen.z;
	gbuffer_data gbd = gbuffer_load_data( PosTc, I.hpos.xy );
	float4 _P = float4( gbd.P, 1.f );

	// Trick to check if rain can fall on the water, gloss include the rain_apply_gloss
	float rain_cover = step(0.035f * (-0.4f + rain_params.x), gbd.gloss - 0.04f );
		
	// Rain intensity. Fadeout and rain cover
	float RainInt = clamp( rain_cover * rain_params.x, 0.f, G_SSR_WATER_RAIN ) * (1.f - smoothstep(20.f, 30.f, _P.z));
		
	// Ripples normal
	basenormal += lerp(float3(0.f, 0.f, 1.f), SSFX_ripples( s_rainsplash, I.tbase ) * 2.f - 1.f, RainInt);
		
	// Water wave intensity
	float3 Navg = normalize(float3(basenormal.x * G_SSR_WATER_WAVES, basenormal.y * G_SSR_WATER_WAVES, basenormal.z));
			
	// Refractions normal ( Used by shadows and refraction )
	float2 N_Scr = normalize(float3(basenormal.x * G_SSR_WATER_REFRACTION, basenormal.y * G_SSR_WATER_REFRACTION, basenormal.z));
		
	// Discard refractions from things above the water ( Like weapons )
	float Refraction_Discard = Pv.z < SSFX_get_depth(PosTc + N_Scr, 0.f);

	// Screen UV + Discard
	float2 Refraction_UV = N_Scr * Refraction_Discard;

	// Shadows to do some tricks
	float3 accumulator = s_accumulator.Sample(smp_nofilter, PosTc + Refraction_UV).xyz; //-' Hozar_2002 fix

	float water_shadows = saturate(max(accumulator.x, max(accumulator.y, accumulator.z)) * LIGHT_MASK_SCALE); //-' Hozar_2002 fix	
	
	float3	Nw	= mul (float3x3(I.M1, I.M2, I.M3), Navg);
			Nw	= normalize (Nw);
		
	float3 v2point	= normalize (I.v2point_w);
	float3 vreflect = reflect(v2point, Nw.xyz);

	float3 eyedir = normalize(Pv);
	float3 Nv = normalize(mul(m_V, Nw));

	// Some vars to put our reflection
	float3 refl_ray, refl_skybox;
	float3 reflection;
	float3 ssr_hit_uv = 0.f;
			
#ifdef NEED_REFLECTIONS	
	// Blue Noise & Normal for noise
	float3 NN = normalize(float3(basenormal.x * 0.15f, basenormal.y * 0.15f, basenormal.z));
	float blue_noise = s_bluenoise.Sample(smp_linear, I.tbase * float2(0.05f, 0.05f) + NN).b * 1.5f;
		
	// Compute reflection bounce
	float3 wreflect = reflect(eyedir, Nv);

	// Don't trace rays which face the camera. Still worth to avoid the rays mess when you look down.
	float edgeFade = step(dot(-eyedir, wreflect), 0);
		
	// Trace a ray
	if (edgeFade > 0.02f)
		ssr_hit_uv = SSFX_ssr_water_ray(Pv, wreflect, blue_noise, 0);

	// Get current Skybox
	refl_skybox = SSFX_calc_sky(vreflect) * G_SSR_WATER_SKY_REFLECTION;

	if (all(ssr_hit_uv.xy))
	{
		// Get reflection pixel from scene screen
		refl_ray = SSFX_get_image(ssr_hit_uv.xy, 0);

		// Adjust reflection intensity using ssr_hit_uv.y and edgeFade
		float ray_fade = ssr_hit_uv.y * 5.f;
			
		// Reflection fog fadeout
		float fog = 1.f - saturate(( length(float3(Pv.x,Pv.y,ssr_hit_uv.z)) * fog_params.w + fog_params.x) * 1.4f);
			
		float refl_power = saturate(ray_fade * edgeFade * fog);
			
		// Fallback to Skybox
		reflection = lerp(refl_skybox, refl_ray * G_SSR_WATER_MAP_REFLECTION, refl_power);
	}
	else
	{
		// No reflection data, we use only refl_skybox
		reflection = refl_skybox;
	}
#else
	reflection = L_hemi_color.rgb;
#endif

	// Fresnel
#ifndef G_SSR_WATER_CHEAPFRESNEL
	float fresnel		 = SSFX_calc_fresnel(eyedir, Nv, 1.33f); 
	float fresnel_amount = saturate(fresnel * 1.5f);
#else
	float fresnel		 = saturate (dot(vreflect, v2point));
	float fresnel_amount = pow(fresnel, 3.f);
#endif

	// Normal for the base texture...
	float2 N_Tex = normalize(float3(basenormal.x * G_SSR_WATER_TEX_DISTORTION, basenormal.y * G_SSR_WATER_TEX_DISTORTION, basenormal.z));

	// Get Position with Refraction discard
	float3 _P2 = SSFX_get_position(PosTc + Refraction_UV, 0.f);

	// 3d view space pos reconstruction math
	_P2 = float3( _P2.z * ( I.hpos.xy * pos_decompression_params.zw - pos_decompression_params.xy ), _P2.z);

	// Bottom of the water to world space ( Project the caustics and water fog )
	float3 w_b = mul(m_inv_V, float4(_P2 , 1.f));

	// Water Surface to world space
	float3 w_s = mul(m_inv_V, Pv );

	// Caustics offset anim
	float4 CausticsT = float4( timers.x * 0.1f, 0.f, -timers.x * 0.07f, 0.2f );

	// Fake Caustics
	float3 Ca0 = saturate(s_watercaustics.Sample(smp_base, w_b.xz * (0.19f / G_SSR_WATER_CAUSTICS_SCALE) + CausticsT.xy + N_Scr ) - 0.1f );
	float3 Ca1 = saturate(s_watercaustics.Sample(smp_base, w_b.xz * (0.11f / G_SSR_WATER_CAUSTICS_SCALE) + CausticsT.zw + N_Scr ) - 0.1f );
	float3 CA = min(Ca0, Ca1) * water_shadows * G_SSR_WATER_CAUSTICS; // Caustics if light hit the water

	// Water Depth
	float waterDepth = w_s.y - w_b.y + eye_position.y;

	// Water Fog
	float waterFog = exp(waterDepth) - 1.f;

	// Screen buffer
	float3 screen = SSFX_get_image(PosTc + Refraction_UV, 0);

	// Base texture * base color
	float3 base_tex = s_base.Sample(smp_base, (w_s.xz + eye_position.xz) * 0.1f + N_Tex).rgb;

	// Texture * base color
	base_tex *= I.c0.rgb;

	// Mix refraction ( 100% clear water ) & base texture ( Turbidity )
	float3 turbidity = lerp(base_tex, screen, smoothstep(G_SSR_WATER_FOG_MAXDEPTH, -G_SSR_WATER_TURBIDITY, waterFog));
		
	// Let's start the accumulation... First the water result and reflection.
	float3 acc = lerp(turbidity, reflection, fresnel_amount * G_SSR_WATER_REFLECTION);
		
	// Caustics. Fade through water fog
	acc = acc + CA * smoothstep(G_SSR_WATER_FOG_MAXDEPTH + 0.5f, 0.f, waterFog) * saturate(waterFog * 3.f);
			
	// Specular
	float3 Nf = normalize(float3(Nw.x, Nw.y * G_SSR_WATER_SPECULAR_NORMAL, Nw.z)); // flatten the water normal to get better specular
	float3 spec = L_sun_color.rgb * pow(abs(dot(normalize(v2point + L_sun_dir_w), Nf)), 512.f) * G_SSR_WATER_SPECULAR;
	acc += spec * water_shadows;
	
	// Fogging
	float fogging = SSFX_FOGGING(1.f - I.fog, w_s.y);
	
	acc = lerp(fog_color, acc, fogging);
	
	float height_fog = get_height_fog_water(_P.xyz, I.p_world.xyz);
	acc = lerp(acc, fog_color, height_fog);
	
	// Soft border
	float border_alpha = smoothstep( 0.f , G_SSR_WATER_SOFTBORDER, waterDepth);
	
	// Foam
	float4	leaves	= s_leaves.Sample( smp_base, I.tbase);
			leaves.rgb *= water_intensity.r;
	float	calc_cos = 1.f;//-dot(float3(I.M1.z, I.M2.z, I.M3.z),  normalize(v2point));
	float	calc_depth = saturate(waterDepth*calc_cos);
	float	fLeavesFactor = smoothstep(0.025f, 0.05f, calc_depth );
			fLeavesFactor *= smoothstep(0.1f, 0.075f, calc_depth );
	acc		= lerp(acc, leaves, leaves.a*fLeavesFactor);
	border_alpha		= lerp(border_alpha, leaves.a, leaves.a*fLeavesFactor);

	// Done
	return  float4(acc, fogging * fogging * border_alpha);
}