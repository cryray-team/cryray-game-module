//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifdef LAUNCHER_OPT_WATER_NEED_REFLECTIONS
	#define NEED_REFLECTIONS
#endif

#ifdef LAUNCHER_OPT_WATER_NEED_SPECULARS
	#define NEED_SPECULARS
#endif

#ifdef LAUNCHER_OPT_WATER_NEED_FOAM
	#define NEED_FOAM
#endif

#include "ps_water.hlsl"