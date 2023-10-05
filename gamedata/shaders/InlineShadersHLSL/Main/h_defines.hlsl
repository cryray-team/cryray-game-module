//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef DEFINES_H
#define DEFINES_H
	#define Epsilon 					0.00001f
	#define EPSDEPTH 					0.0001f
	
	#define fWhiteIntensitySQR 			2.89f
	#define def_lum_hrange 				0.7f	// hight luminance range, for build bloom

	#define	LUMINANCE_VECTOR 			float3(0.2125f, 0.7154f, 0.0721f)
	
	#define LIGHT_MASK_SCALE 			1.4f //-' Hozar_2002 fix
	
	#define GRASS_WAVE_FREQ 			0.9f  // частота появления волн
	#define GRASS_WAVE_POWER 			3.f  // яркость волн
	
	#define PCSS_PIXEL 					4
	#define PCSS_STEP 					2
	#define PCSS_PIXEL_MIN 				1.f
	#define PCSS_SUN_WIDTH 				150.f
	
	//-' Gasmask settings
	#define GM_DIST_INT 				0.025f //Refraction intensity
	#define GM_DIFF_INT 				0.25f //Diffuse cracks intensity
	#define GM_VIG_INT 					0.5f //Vignette intensity

	//-' Raindrops settings
	#define GM_DROPS_TURBSIZE 			15.f //Turbulence power
	#define GM_DROPS_TURBSHIFT 			float4(0.35f, 1.f, 0.f, 1.f) //Turbulence offset
	#define GM_DROPS_TURBTIME 			sin(0.1f/3.f) 
	#define GM_DROPS_TURBCOF 			0.33f //Turbulence intensity

	//-' Glass reflections settings
	#define GM_VIS_NUM 					16 //Reflection quality
	#define GM_VIS_RADIUS 				0.45f //Reflection radius
	#define GM_VIS_INTENSITY 			0.5f //Reflection intensity
	
	//-' Для SSDO OGSE:
	#define SSDO_RADIUS 				0.07f					// радиус семплирования в игровых метрах. Чем выше, тем более заметен эффект, но тем больше нежелательных наложений теней
	#define SSDO_DISCARD_THRESHOLD 		1.f		// максимальная разница в глубине пикселей, при которой пиксель еще учитывается в расчете затенения. Уменьшение убирает "шлейф" в некоторых случаях.
	#define SSDO_BLEND_FACTOR 			1.450f
	#define SSDO_COLOR_BLEEDING 		15.f	
	#define SSDO_GRASS_TUNING 			1.5f					// чем больше, тем меньше затеняется трава
	
	// Group Defines
	#define GROUP_TEXEL_DIM				56
	#define GROUP_THREAD_DIM 			32 // 32 * 32 = 1024 threads 
	#define GROUP_TEXEL_OVERLAP			12
	
	// Texture Op Defines
	#define GATHER_THREADS 				784
	#define GATHER_THREADS_PER_ROW 		28
	#define GATHER_PER_THREAD 			1
	
	// ALU Op Defines
	#define ALU_DIM						32
#endif