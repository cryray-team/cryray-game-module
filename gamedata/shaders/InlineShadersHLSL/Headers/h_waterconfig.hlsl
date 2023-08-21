//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef _WATERCONFIG_H
#define _WATERCONFIG_H

//настройки для: (1)
//waterdistortion
//waterdistortion2

#define W_POSITION_SHIFT_HEIGHT (1.f/60.f)	//(1.f/100.f)	//amplitude /50 - small, /10 - large
#define W_POSITION_SHIFT_SPEED	(25.f)		//(25.0f)
#define W_DISTORT_BASE_TILE_0	(1.f)		//(1.0f)
#define W_DISTORT_BASE_TILE_1	(1.1f)		//(1.1f)
#define W_DISTORT_AMP_0		(+0.15f)	//(+0.15f)
#define W_DISTORT_AMP_1		(+0.55f)	//(-0.30f)
#define W_DISTORT_POWER		(1.f)		//(1.0f)

#endif
