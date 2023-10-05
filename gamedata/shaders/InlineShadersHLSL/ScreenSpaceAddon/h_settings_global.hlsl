//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Indirect Light Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_IL_INTENSITY				1.f	// Intensity of the indirect ilumination
#define G_IL_SKYLIGHT_INTENSITY		0.3f	// Intensity of the sky light
#define G_IL_COLOR_VIBRANCE			1.5f	// Vibrance of the indirect ilumination

#define G_IL_NOISE					0.25f	// Intensity of the noise applied to the IL

#define G_IL_RANGE 					0.6f	// Radius of the IL sampling [ Recommended 0.3f ~ 1.0f ]
#define G_IL_MAX_DIFFERENCE			10.f	// Maximum distance the sampled color can travel

#define G_IL_WEAPON_LENGTH 			1.5f	// Maximum lenght of the weapon
#define G_IL_WEAPON_RANGE 			0.015f	// Weapons radius for the IL sampling

#define G_IL_DISCARD_SAMPLE_AT		0.1f	// If the intensity of the bounce is lower or equal to this value, discard the sample. ( 1.0f = 100% intensity )
											// The intensity of the bounce will vary depending on the angle of the surfaces and the distance.

//#define G_IL_DEBUG_MODE					// Uncomment if you want to check what the IL is doing

//////////////////////////////////////////////////////////////////////////
//-' Indirect Light Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Fog Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_FOG_HEIGHT					8.f	// Height of the ground fog.
#define G_FOG_HEIGHT_INTENSITY			1.f	// Intensity of the ground fog.
#define G_FOG_HEIGHT_DENSITY			1.3f	// Density of the ground fog.

#define G_FOG_SUNCOLOR_INTENSITY		0.1f	// Intensity of sun color in the fog. [ 0.0f = 0% | 1.0f = 100% ]

//////////////////////////////////////////////////////////////////////////
//-' Fog Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Flora Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_TREES_SPECULAR				2.f	// Trees specular intensity
#define G_GRASS_SPECULAR				3.f	// Grass specular intensity
#define G_SSS_INTENSITY_DETAILS			0.5f	// Subsurface Scattering intensity

//////////////////////////////////////////////////////////////////////////
//-' Flora Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Water Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_SSR_WATER_QUALITY				1		// Quality of the water. ( 0 = low | 1 = medium | 2 = high | 3 = Very High | 4 = Ultra )

#define G_SSR_WATER_WAVES				1.f	// Water waves intensity
#define G_SSR_WATER_REFLECTION			0.5f	// Reflection intensity. ( 1.0f = 100% )
#define G_SSR_WATER_REFRACTION			0.05f	// Intensity of refraction distortion

#define G_SSR_WATER_SKY_REFLECTION		1.f	// Sky reflection factor. ( 1.0f = 100% )
#define G_SSR_WATER_MAP_REFLECTION		1.f	// Objects reflection factor. ( 1.0f = 100% )

#define G_SSR_WATER_TEX_DISTORTION		0.2f	// Water texture distortion.
#define G_SSR_WATER_TURBIDITY			3.f	// Turbidity factor. ( 0.0f = Clear water )

#define G_SSR_WATER_FOG_MAXDEPTH		2.f	// Maximum visibility underwater.

#define G_SSR_WATER_RAIN				0.4f	// Max intensity of rain drops

#define G_SSR_WATER_SPECULAR			6.f	// Sun/Moon specular intensity
#define G_SSR_WATER_SPECULAR_NORMAL		0.2f	// Specular normal intensity. ( You may need to adjust this if you change the value of G_SSR_WATER_WAVES )

#define G_SSR_WATER_CAUSTICS			0.25f	// Caustics intensity
#define G_SSR_WATER_CAUSTICS_SCALE		1.f	// Caustics Size

#define G_SSR_WATER_SOFTBORDER			0.1f	// Soft border factor. ( 0.0f = hard edge )

#define G_SSR_WATER_CHEAPFRESNEL				// Uncomment/comment this if you want to use a faster/accurate fresnel calc

//////////////////////////////////////////////////////////////////////////
//-' Water Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Puddles Settings: --> START
//////////////////////////////////////////////////////////////////////////

// NOTE: Reflection quality is defined by G_SSR_QUALITY ( settings_screenspace_SSR.h )

#define G_PUDDLES_GLOBAL_SIZE				1.f	// Puddles global size. ( This affect distance and puddles size )
#define G_PUDDLES_SIZE						0.8f	// Puddles individual size. ( This only affect puddles size )
#define G_PUDDLES_BORDER_HARDNESS			0.7f	// Border hardness. ( 1.0f = Extremely defined border )
#define G_PUDDLES_TERRAIN_EXTRA_WETNESS		0.15f	// Terrain extra wetness when raining. ( Over time like puddles )
#define G_PUDDLES_REFLECTIVITY				0.4f	// Reflectivity. ( 1.0f = Mirror like )
#define G_PUDDLES_TINT						float3(0.66f, 0.63f, 0.6f) // Puddles tint RGB.

#define G_PUDDLES_RIPPLES							// Comment to disable ripples
#define G_PUDDLES_RIPPLES_SCALE				1.f	// Ripples scale
#define G_PUDDLES_RIPPLES_INTENSITY			1.f	// Puddles ripples intensity
#define G_PUDDLES_RIPPLES_RAINING_INT		0.1f	// Extra ripple intensity when raining ( Affected by rain intensity )
#define G_PUDDLES_RIPPLES_SPEED				1.f	// Puddles ripples movement speed

#define G_PUDDLES_RAIN_RIPPLES_INTENSITY	1.f	// Rain ripples intensity
#define G_PUDDLES_RAIN_RIPPLES_SCALE		1.f	// Rain ripples scale

#define G_PUDDLES_REFRACTION_INTENSITY		1.f	// Refraction intensity

//#define G_PUDDLES_ALLWAYS					// Uncomment to allways render puddles ( Raining or not )

//////////////////////////////////////////////////////////////////////////
//-' Puddles Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' SSR Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_SSR_QUALITY					0		// Quality of the SSR. ( 0 = Very low | 1 = Low | 2 = Medium | 3 = High | 4 = Very High | 5 = Ultra )

#define G_SSR_VERTICAL_SCREENFADE		4.0f	// Vertical fade. ( higher values = sharp gradient )

#define G_SSR_INTENSITY					1.3f	// Global reflection intensity ( 1.0f = 100% ~ 0.0f = 0% )
#define G_SSR_MAX_INTENSITY				0.5f	// Global reflection MAX intensity.
#define G_SSR_SKY_INTENSITY				0.6f	// Sky reflection intensity ( 1.0f = 100% ~ 0.0f = 0% )
#define G_SSR_FLORA_INTENSITY 			0.5f	// Adjust grass and tree branches intensity
#define G_SSR_TERRAIN_BUMP_INTENSITY	0.6f	// Terrain bump intensity ( Lower values will generate cleaner reflections )

#define G_SSR_WEAPON_INTENSITY  		0.5f	// Weapons & arms reflection intensity. ( 1.0f = 100% ~ 0.0f = 0% )
#define G_SSR_WEAPON_MAX_INTENSITY		0.015f	// Weapons & arms MAX intensity.
#define G_SSR_WEAPON_MAX_LENGTH			1.3f	// Maximum distance to apply G_SSR_WEAPON_INTENSITY.
#define G_SSR_WEAPON_RAIN_FACTOR		0.2f	// Weapons reflections boost when raining ( 0 to disable ) ( Affected by rain intensity )

//#define G_SSR_BEEFS_NVGs_ADJUSTMENT		0.5f	// Uncomment to adjust the reflection intensity when 'Beefs Shader Based NVGs' are in use. ( Use only if Beefs NVGs addon is installed )
//#define G_SSR_WEAPON_REFLECT_ONLY_WITH_RAIN		// Uncomment this if you don't want weapon reflections unless is raining
//#define G_SSR_CHEAP_SKYBOX						// Use a faster skybox to improve some performance.

//////////////////////////////////////////////////////////////////////////
//-' SSR Settings: <-- END
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//-' Shadows Settings: --> START
//////////////////////////////////////////////////////////////////////////

#define G_SSS_STEPS						32		// More steps = better quality / poor performance. ( 24 = Low | 32 = Medium | 48 = High | 64 = Ultra )

#define G_SSS_INTENSITY_SHADOWS			1.f	// Shadow general intensity. [ 0.5f = 50% ~ 1.0f = 100% ]
#define G_SSS_DETAILS					0.02f	// Limit detail. Lower values will introduce more details to shadows but also incorrect results.
#define G_SSS_FORCE_FADE				0.5f	// Force shadow to start to fade at [ 0.5f = 50% ~ 1.0f = no fade ]

#define G_SSDO_SCENARY_HARDNESS			1.f	// Scenary shadow hardness. 0.0f to disable scenary shadows.
#define G_SSDO_SCENARY_SHADOW_LENGTH	1.5f	// Scenary maximum shadow length.

#define G_SSDO_GRASS_HARDNESS			1.f	// Grass shadow hardness.
#define G_SSDO_GRASS_SHADOW_LENGTH		0.5f	// Grass maximum shadow length.

#define G_SSDO_WEAPON_LENGTH			1.5f	// Maximum distance to apply weapon factors.
#define G_SSDO_WEAPON_HARDNESS			1.f	// Weapon shadow hardness. 0.0f to disable weapon shadows.
#define G_SSDO_WEAPON_SHADOW_LENGTH		0.09f	// Weapon maximum shadow length.

//////////////////////////////////////////////////////////////////////////
//-' Shadows Settings: <-- END
//////////////////////////////////////////////////////////////////////////