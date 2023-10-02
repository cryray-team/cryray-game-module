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
	//-' Параметр def_aref
	//-' def_aref.x - Регулирует прозрачность травы
	//-' def_aref.y - Регулирует прозрачность травы листвы у деревьев
	//-' def_aref.z - const тени
	//-' def_aref.w - const партиклы
	uniform float4 		 def_aref; //-' x - grass, y - trees
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Параметр img_rgb
	//-' img_rgb.x - красный канал
	//-' img_rgb.y - зеленый канал
	//-' img_rgb.z - синий канал
	//-' img_rgb.w - Debug 
	uniform float4 		 img_rgb; //-' Debug, r, g, b : 0.0 0.0 0.0, on/off
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Anomaly params:
	uniform float3 		 drops_control;	
	uniform float4 		 mask_control; 
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Параметр is_glass_active проверяет,
	//-' есть ли в головном уборе или броне стекло:
	uniform float4		 is_glass_active;
	static bool			 is_helmet = is_glass_active.x;
	static bool			 is_outfit = is_glass_active.y;
	//-' zw параметры не активны
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Проверка на включенный ПНВ:
	uniform float4		 is_nightvision_enable;
	static int			 nightvision_enable = is_nightvision_enable.x; //-' [0..3]
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Контраст глубины для погоды:
	uniform float4 		 contrast_depth;
	static float	     weather_contrast = contrast_depth.x;
	////////////////////////////////////////////////////////////////////////////////
	
	//-' Задает параметры для наземного тумана:
	//-' ground_fog.x - Высота тумана
	//-' ground_fog.y - Плотность тумана
	//-' ground_fog.z - Максимальная дистанция
	//-' ground_fog.w - (Не поддерживается)
	uniform float4 		 ground_fog;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' graphic_options_1.xyzw
	//-' graphic_options_1.x - Выбор блума
	//-' graphic_options_1.y - Глосс 2218 билд
	//-' graphic_options_1.z - Наземный туман
	//-' graphic_options_1.w - АО отладка

	uniform float4 							graphic_options_1;

	//-' Для блума:
	static const float bloom_type 			= graphic_options_1.x;
	//-' Для глосса:
	static const bool build_style_gloss 	= graphic_options_1.y;
	//-' Для коллизии травы:
	static const float ground_fog_enable 	= graphic_options_1.z;
	//-' Для отладки АО:
	static const bool AO_debug 	    		= graphic_options_1.w;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' graphic_effects_1.xyzw
	//-' graphic_effects_1.x - ...
	//-' graphic_effects_1.y - Блум
	//-' graphic_effects_1.z - Насыщенность блума
	//-' graphic_effects_1.w - Яркость растительности

	uniform float4 							graphic_effects_1;

	//-' ...:
	//static const float  					= graphic_effects_1.x;
	//-' Блум:
	static const float bloom_treshold 		= graphic_effects_1.y;
	//-' Насыщенность блума:
	static const float bloom_saturation 	= graphic_effects_1.z;
	//-' Яркость растительности:
	static const float hemi_flora 	    	= graphic_effects_1.w;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' screen_effects_1.xyzw
	//-' screen_effects_1.x - (Накопляемый параметр) -> Урон по ГГ
	//-' screen_effects_1.y - (Накопляемый параметр) -> Убывание стамины ГГ
	//-' screen_effects_1.z - (Накопляемый параметр) -> Эффект лечения антирадом
	//-' screen_effects_1.w - (Накопляемый параметр) -> Эффект лечения аптечкой
	
	//uniform float4 							screen_effects_1;
	
	//-' Урон по ГГ:
	//static const float hit_power			= screen_effects_1.x;
	//-' Убывание стамины ГГ:
	//static const float stamina_minus		= screen_effects_1.y;
	//-' Эффект лечения антирадом:
	//static const float antirad_effector		= screen_effects_1.z;
	//-' Эффект лечения аптечкой:
	//static const float health_effector		= screen_effects_1.w;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' screen_effects_2.xyzw
	//-' screen_effects_2.x - (Накопляемый параметр) -> Эффект накопления радиации
	//-' screen_effects_2.y - (Дистанция до зоны радиации) -> Эффект радиации
	//-' z - ...
	//-' w - ...
	
	//uniform float4 							screen_effects_2;
	
	//-' Эффект накопления радиации:
	//static const float radiation_effect		= screen_effects_2.x;
	//-' Усиливание эффекта от расстояния до зоны радиации:
	//static const float dist_to_rad_zone		= screen_effects_2.y;
	//-' ...:
	//static const float 					= screen_effects_2.z;
	//-' ...:
	//static const float 					= screen_effects_2.w;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' graphic_options_2.xyzw
	//-' graphic_options_2.x - Опция шейдерного эффекта хита
	//-' graphic_options_2.y - Опция шейдерного эффекта стамины
	//-' graphic_options_2.z - Опция шейдерного эффекта радиации
	//-' graphic_options_2.w - Опция шейдерного эффекта лечения аптечкой
	
	uniform float4 							graphic_options_2;
	
	//-' Получаемый хит в данный момент:
	static const bool optShaderHit			= graphic_options_2.x;
	//-' Убывание выносливости:
	static const bool optShaderStamina		= graphic_options_2.y;
	//-' Мерцание от радиации:
	static const bool optShaderRadiation	= graphic_options_2.z;
	//-' Опция шейдерный эффект лечения от аптечки:
	static const bool optShaderMedkit		= graphic_options_2.w;
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	//-' graphic_options_3.xyzw
	//-' graphic_options_3.x - Опция шейдерного эффекта лечения антирадом
	//-' graphic_options_3.y - ...
	//-' graphic_options_3.z - ...
	//-' graphic_options_3.w - ...
	
	uniform float4 							graphic_options_3;
	
	//-' Опция шейдерный эффект лечения от антирада:
	static const bool optShaderAntirad		= graphic_options_3.x;
	//-' ...:
	//static const float 					= graphic_options_3.y;
	//-' ...:
	//static const float 					= graphic_options_3.z;
	//-' ...:
	//static const float 					= graphic_options_3.w;
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