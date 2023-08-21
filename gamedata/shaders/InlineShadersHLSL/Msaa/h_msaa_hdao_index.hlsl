//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	MSAA_HDAO_I_H
#define	MSAA_HDAO_I_H
	#if (MSAA_SAMPLES == 2)
		#define MSAA_SAMPLE_INDEX (0)
	#elif (MSAA_SAMPLES == 4)
		#define MSAA_SAMPLE_INDEX (3)
	#elif (MSAA_SAMPLES == 8)
		#define MSAA_SAMPLE_INDEX (6)
	#else
		#define MSAA_SAMPLE_INDEX (0)
	#endif
#endif