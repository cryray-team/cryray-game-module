//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

// [ SETTINGS ] [ AMBIENT OCCLUSION ]

#define G_SSDO_RENDER_DIST 				150.f 	// Max rendering distance.
	
#define G_SSDO_RADIUS 					0.4f	// AO radius, higher values means more occlusion coverage with less detail and intensity. ( Recommended 0.1 ~ 0.5 )
#define G_SSDO_INTENSITY 				7.f 	// General AO intensity.

#define G_SSDO_FLORA_INTENSITY 			1.f 	// Flora intensity. [ 1.0f = 100% ~ 2.0f = 200% ]

#define G_SSDO_NOISE_MIN 				0.2f	// Minimum noise intensity.
#define G_SSDO_NOISE_MAX 				1.f 	// Maximum noise intensity. ( Noise increase intensity through distance )

#define G_SSDO_MAX_OCCLUSION			0.f 	// Maximum obscurance for a pixel. 0 = full black
#define G_SSDO_SMOOTH 					1.3f 	// AO softer. Highly occluded pixels will be more affected than low occluded ones.

#define G_SSDO_WEAPON_LENGTH 			2.f 	// Maximum distance to apply G_SSDO_WEAPON_RADIUS, G_SSDO_WEAPON_INTENSITY, etc.
#define G_SSDO_WEAPON_RADIUS 			0.02f	// Weapon radius, higher values means more occlusion coverage with less details.
#define G_SSDO_WEAPON_INTENSITY			0.4f	// Weapon intensity. [ 1.0f = 100% ]

//#define G_SSDO_DETAILED_SEARCH					// Add an extra check to the AO search. By default more detailed. ( 20% of G_SSDO_RADIUS for Scenary and 50% for weapons )
#define G_SSDO_DETAILED_RADIUS			0.2f	// Detailed search radius. [ 1.0f = 100% ]
#define G_SSDO_DETAILED_WEAPON_RADIUS	0.5f	// Detailed serach radius for weapons. [ 1.0f = 100% ]