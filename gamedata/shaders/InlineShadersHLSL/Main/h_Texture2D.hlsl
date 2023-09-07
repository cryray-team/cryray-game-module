//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#ifdef MSAA_ANTIALIASING_ENABLE
	Texture2DMS<float4>	s_position;	// rgb.a = diffuse.gloss
	Texture2DMS<float4>	s_diffuse;	// rgb.a = diffuse.gloss
	Texture2DMS<float4>	s_accumulator;      	// rgb.a = diffuse.specular		
	Texture2DMS<float4>	s_generic;
	Texture2DMS<float4>	s_vollight;
	Texture2DMS<float4> s_patched_normal;
	Texture2DMS<float4>	s_ext;
#else
	Texture2D	s_position;	//	smp_nofilter or Load
	Texture2D	s_diffuse;	// rgb.a = diffuse.gloss
	Texture2D	s_accumulator;      	// rgb.a = diffuse.specular
	Texture2D   s_generic;
	Texture2D 	s_vollight;
	Texture2D	s_patched_normal;
	Texture2D   s_ext;
#endif
	Texture2D 	s_scope;				// dltx mod
	
	Texture2D 	s_base;					//	smp_base
	Texture2D 	s_bump;             	//
	Texture2D 	s_bumpX;                //
	Texture2D 	s_detail;               //
	Texture2D 	s_detailBump;           //	
	Texture2D 	s_detailBumpX;          //	Error for bump detail
	Texture2D 	s_hemi;             	//

	Texture2D 	s_mask;             	//

	Texture2D 	s_dt_r;                	//
	Texture2D 	s_dt_g;                	//
	Texture2D 	s_dt_b;                	//
	Texture2D 	s_dt_a;                	//

	Texture2D 	s_dn_r;                	//
	Texture2D 	s_dn_g;                	//
	Texture2D 	s_dn_b;                	//
	Texture2D 	s_dn_a;                	//
	
	Texture2D	s_lmap;					// 2D/???cube projector lightmap
	
//////////////////////////////////////////////////////////////////////////////////////////
// Combine phase                                //
	
	Texture2D 	s_static_drops;
	
	Texture2D	s_bloom;	//
	Texture2D	s_image;	// used in various post-processing
	Texture2D	s_tonemap;	// actually MidleGray / exp(Lw + eps)

	//Other
	Texture2D 	s_blur_2;
	Texture2D 	s_blur_4;
	Texture2D 	s_blur_8;

	Texture2D 	s_bloom_new;
	
	Texture2D	s_nmap0;	// Screen Space Addon
	Texture2D	s_bluenoise; // Screen Space Addon
	Texture2D	s_rainsplash; // Screen Space Addon
	Texture2D	s_watercaustics; // Screen Space Addon
	
	Texture2D 	s_lens_dirt; //shaders\\lens_dirt
	Texture2D 	s_noise_1; //shaders\\noise\\noise_tex
	Texture2D 	s_lut_atlas; //shaders\\lut_atlas
	
	//Main gasmask textures
	Texture2D 	s_mask_nm_1;
	Texture2D 	s_mask_nm_2;
	Texture2D 	s_mask_nm_3;
	Texture2D 	s_mask_nm_4;
	Texture2D 	s_mask_nm_5;
	Texture2D 	s_mask_nm_6;
	Texture2D 	s_mask_nm_7;
	Texture2D 	s_mask_nm_8;
	Texture2D 	s_mask_nm_9;
	Texture2D 	s_mask_nm_10;

	//Vignette masks
	Texture2D 	s_mask_v_1;
	Texture2D 	s_mask_v_2;
	Texture2D 	s_mask_v_3;

	//Condensation droplets texture
	Texture2D 	s_mask_droplets;
	
	Texture2D   s_ambient_occlusion;
	Texture2D 	s_ambient_occlusion_temp;
	
	Texture2D 	s_gasmask;
	Texture2D   s_gasmask_normal;
	
	Texture2D 	s_lut_1;
	Texture2D 	s_lut_2;
	Texture2D 	s_lut_3;
	Texture2D 	s_lut_4;
	Texture2D 	s_lut_5;
	
	Texture2D 	s_vp2;
	
	Texture2D 	s_pdascreen;
	
	Texture2D 	s_base0;
	Texture2D 	s_base1;
	Texture2D 	s_noise;
	
	Texture2D 	s_grad0;
	Texture2D 	s_grad1;
	
	Texture2D 	s_mask_blur; // smoothed mask
	
	Texture2D   s_sunshafts;
	Texture2D 	s_sun_shafts; // current sunshafts texture
	
	Texture2D	jitter0;
	Texture2D 	jitter1;
	Texture2D 	jitterMipped;
	
	Texture2D 	RT_dof;
	
	Texture2D	s_nmap;
	Texture2D	s_leaves;
	Texture2D	s_waterFall;
	Texture2D 	s_water2D;
	
	Texture2D 	s_puddles_normal;
	Texture2D 	s_puddles_perlin;
	Texture2D 	s_puddles_mask;
	Texture2D 	s_pbr;
	
	Texture2D 	jitter4;
	Texture2D 	s_half_depth;
#endif