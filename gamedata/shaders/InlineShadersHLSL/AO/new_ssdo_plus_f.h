//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef SSDO_P_F_H
#define SSDO_P_F_H
	#ifndef SSFX_READY
		#include "ScreenSpaceAddon\screenspace_common.h"
	#endif

	#include "ScreenSpaceAddon\settings_screenspace_ao.h"

	// Internal values
	#if SSAO_QUALITY == 3
		#define G_SSDO_SAMPLE 32
	#elif SSAO_QUALITY == 2
		#define G_SSDO_SAMPLE 16
	#elif SSAO_QUALITY == 1
		#define G_SSDO_SAMPLE 8
	#endif
#endif