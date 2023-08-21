//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	IMPORT_PARAMS_H
#define	IMPORT_PARAMS_H
	float3 				 drops_control;
	float4 				 mask_control; 
	
	float4				 is_glass_active;
	static bool			 is_helmet = is_glass_active.x;
	static bool			 is_outfit = is_glass_active.y;
	
	float4				 is_nightvision_enable;
	static int			 nightvision_enable = is_nightvision_enable.x; //-' [0..3]
	
	uniform float4 		 contrast_depth;
	static float	     weather_contrast = contrast_depth.x;
	
	float4 				 fog_shaders_values;
	
	//-' import_to_shader_params_cryray_1.xyzw
	//-' x - Выбор блума
	//-' y - Глосс 2218 билд
	//-' z - Наземный туман
	//-' w - АО отладка

	uniform float4 							import_to_shader_params_cryray_1;

	//-' Для блума:
	static const float bloom_type 			= import_to_shader_params_cryray_1.x;
	//-' Для глосса:
	static const bool  build_style_gloss 	= import_to_shader_params_cryray_1.y;
	//-' Для коллизии травы:
	static const bool ground_fog 	    	= import_to_shader_params_cryray_1.z;
	//-' Для отладки АО:
	static const bool  AO_debug 	    	= import_to_shader_params_cryray_1.w;

	//-' import_to_shader_params_cryray_2.xyzw
	//-' x - Выбор качества прозрачности текстур
	//-' y - Блум
	//-' z - Насыщенность блума
	//-' w - ...

	uniform float4 							import_to_shader_params_cryray_2;

	//-' Качество прозрачности текстур:
	static const float tp_textures 			= import_to_shader_params_cryray_2.x;
	static const float def_aref 			= tp_textures / 255.0f; //-' IX-Ray fix
	//-' Блум:
	static const float cr_bloom_t 			= import_to_shader_params_cryray_2.y;
	static const float bloom_treshold 		= cr_bloom_t;
	//-' Насыщенность блума:
	static const float cr_bloom_s	    	= import_to_shader_params_cryray_2.z;
	static const float bloom_saturation 	= cr_bloom_s;
	//-' ...:
	static const bool slot_4 	    	    = import_to_shader_params_cryray_2.w;
	
	#define xshading_model 					float(shading_model.x)
	#define xsslr_factor 					float(sslr_factor.x)
	#define xfake_pbr_params 				float2(fake_pbr_params.xy)
	
	//-' Яркость растительности:
	uniform float4 							import_to_shader_params_cryray_3; //-' xyz = rgb
	
	//-' HDAO CS:
	Texture2D<float> 						s_occ;
	
	//-' Level ID:
	static const int 						m_levelID;
	/*
		enum class Levels 
		{
			jupiter = 1,
			k00_marsh,
			k01_darkscape,
			k02_trucks_cemetery,
			l01_escape, //-' ID = 5
			l02_garbage,
			l03_agroprom,
			l04_darkvalley,
			l05_bar,
			l06_rostok,
			l07_military,
			l08_yantar,
			l09_deadcity,
			l10_limansk,
			l10_radar,
			l10_red_forest,
			l11_hospital,
			l11_pripyat,
			l12_stancia,
			l12_stancia_2,
			l13_generators,
			pripyat,
			zaton,
			jupiter_underground,
			labx8,
			l03u_agr_underground,
			l04u_labx18,
			l08u_brainlab,
			l10u_bunker,
			l12u_control_monolith,
			l12u_sarcofag,
			l13u_warlab,
			fake_start,
			y04_pole
		};
	*/
#endif