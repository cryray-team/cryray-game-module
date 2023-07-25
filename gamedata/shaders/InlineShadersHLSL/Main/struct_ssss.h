//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	SSSS_H
#define	SSSS_H
	struct p_screen
	{
		float4          hpos 	: SV_Position;
		float2          tc0		: TEXCOORD0;        // Texture coordinates         (for sampling maps)
	};

	struct v2p_screen
	{
		float2 tc0 : TEXCOORD0;
		float4 HPos : POSITIONT;  	// Clip-space position 	(for rasterization)
	};

	struct v_ssss
	{
		float4 P : POSITIONT;
		float2 tc0	: TEXCOORD0;
	};

	struct v2p_ssss
	{
		float2 tc0 : TEXCOORD0;
		float4 HPos : SV_Position;	// Clip-space position 	(for rasterization)
	};
#endif