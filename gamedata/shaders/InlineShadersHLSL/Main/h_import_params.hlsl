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
	uniform float3 		 drops_control;
	uniform float4 		 mask_control; 
	
	uniform float4		 is_glass_active;
	static bool			 is_helmet = is_glass_active.x;
	static bool			 is_outfit = is_glass_active.y;
	
	uniform float4		 is_nightvision_enable;
	static int			 nightvision_enable = is_nightvision_enable.x; //-' [0..3]
	
	uniform float4 		 contrast_depth;
	static float	     weather_contrast = contrast_depth.x;
	
	uniform float4 		 fog_shaders_values;
	
	////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' import_to_shader_params_cryray_2.xyzw
	//-' x - Выбор качества прозрачности текстур
	//-' y - Блум
	//-' z - Насыщенность блума
	//-' w - Яркость растительности

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
	//-' Яркость растительности:
	static const float cr_hemi_flora		= import_to_shader_params_cryray_2.w;
	static const float hemi_flora 	    	= cr_hemi_flora; //-' Only x
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' import_to_shader_params_cryray_3.xyzw
	//-' x - Урон по ГГ
	//-' y - Убывание стамины
	//-' z - ...
	//-' w - ...
	
	uniform float4 							import_to_shader_params_cryray_3;
	
	//-' Получаемый хит в данный момент:
	static const float cr_hit_power_factor  = import_to_shader_params_cryray_3.x;
	static const float hit_power			= cr_hit_power_factor;
	//-' Убывание выносливости:
	static const float cr_stamina_minus_f   = import_to_shader_params_cryray_3.y;
	static const float stamina_minus		= cr_stamina_minus_f;
	//-' ...:
	static const float cr_slot_3_3			= import_to_shader_params_cryray_3.z;
	static const float slot_3_3				= cr_slot_3_3;
	//-' ...:
	static const float cr_slot_4_3			= import_to_shader_params_cryray_3.w;
	static const float slot_4_3				= cr_slot_4_3;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' import_to_shader_params_cryray_4.xyzw
	//-' x - Опция шейдерного хита по ГГ
	//-' y - Опция шейдерного эффекта стамины ГГ
	//-' z - ...
	//-' w - ...
	
	uniform float4 							import_to_shader_params_cryray_4;
	
	//-' Получаемый хит в данный момент:
	static const float cr_optShaderHit  	= import_to_shader_params_cryray_4.x;
	static const float ShaderHit			= cr_optShaderHit;
	//-' Убывание выносливости:
	static const float cr_optShaderStamina  = import_to_shader_params_cryray_4.y;
	static const float ShaderStamina		= cr_optShaderStamina;
	//-' ...:
	static const float cr_slot_3_4			= import_to_shader_params_cryray_4.z;
	static const float slot_3_4				= cr_slot_3_4;
	//-' ...:
	static const float cr_slot_4_4			= import_to_shader_params_cryray_4.w;
	static const float slot_4_4				= cr_slot_4_4;
	////////////////////////////////////////////////////////////////////////////////
	
	#define xshading_model 					float(shading_model.x)
	#define xsslr_factor 					float(sslr_factor.x)
	#define xfake_pbr_params 				float2(fake_pbr_params.xy)
	
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