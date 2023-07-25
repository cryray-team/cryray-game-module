//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

// [ SETTINGS ] [ SKY DEBANDING ]

#define G_DEBANDING_QUALITY		2	// Deband iterations [ 1 = Low, 2 = Normal, 3 = High, 4 = Ultra ]
#define G_DEBANDING_RADIUS		48	// Max pixels to search for banding. Higher values improve smoothness, but also add more noise in some circumstances.
