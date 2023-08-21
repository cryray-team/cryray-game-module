//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	BLOOM_H
#define	BLOOM_H
	struct	v_build
	{
		float4	P		: POSITIONT;
		float2	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float2 	Tex2	: TEXCOORD2;
		float2	Tex3	: TEXCOORD3;
	};

	struct	v2p_build
	{
		float2 	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float2 	Tex2	: TEXCOORD2;
		float2	Tex3	: TEXCOORD3;
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_build
	{
		float2 	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float2 	Tex2	: TEXCOORD2;
		float2	Tex3	: TEXCOORD3;
	};
#endif