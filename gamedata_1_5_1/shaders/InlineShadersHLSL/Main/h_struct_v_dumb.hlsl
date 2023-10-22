//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	V_DUMB_H
#define	V_DUMB_H
	struct 	v_dumb
	{
		float4	P		:POSITION;	// Clip-space position 	(for rasterization)
	};

	struct 	v2p_dumb
	{
		float4	HPos	:SV_Position;	// Clip-space position 	(for rasterization)
	};
#endif