//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	V_FILTER_H
#define	V_FILTER_H
	struct	v_filter
	{
		float4	P		: POSITIONT;
		float4 	Tex0	: TEXCOORD0;
		float4	Tex1	: TEXCOORD1;
		float4 	Tex2	: TEXCOORD2;
		float4	Tex3	: TEXCOORD3;
		float4 	Tex4	: TEXCOORD4;
		float4	Tex5	: TEXCOORD5;
		float4 	Tex6	: TEXCOORD6;
		float4	Tex7	: TEXCOORD7;
	};

	struct	v2p_filter
	{
		float4 	Tex0	: TEXCOORD0;
		float4	Tex1	: TEXCOORD1;
		float4 	Tex2	: TEXCOORD2;
		float4	Tex3	: TEXCOORD3;
		float4 	Tex4	: TEXCOORD4;
		float4	Tex5	: TEXCOORD5;
		float4 	Tex6	: TEXCOORD6;
		float4	Tex7	: TEXCOORD7;
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_filter
	{
		float4 	Tex0	: TEXCOORD0;
		float4	Tex1	: TEXCOORD1;
		float4 	Tex2	: TEXCOORD2;
		float4	Tex3	: TEXCOORD3;
		float4 	Tex4	: TEXCOORD4;
		float4	Tex5	: TEXCOORD5;
		float4 	Tex6	: TEXCOORD6;
		float4	Tex7	: TEXCOORD7;
	};
#endif