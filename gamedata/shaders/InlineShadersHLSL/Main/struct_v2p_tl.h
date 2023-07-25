//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	V2P_TL_H
#define	V2P_TL_H
	struct	v_TL0uv_positiont
	{
		float4	P		: POSITIONT;
		float4	Color	: COLOR; 
	};

	struct	v_TL0uv
	{
		float4	P		: POSITION;
		float4	Color	: COLOR; 
	};

	struct	v2p_TL0uv
	{
		float4	Color	: COLOR;
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_TL0uv
	{
		float4	Color	: COLOR;
	};
	
	struct	v_TL_positiont
	{
		float4	P		: POSITIONT;
		float2	Tex0	: TEXCOORD0;
		float4	Color	: COLOR; 
	};

	struct	v_TL
	{
		float4	P		: POSITION;
		float2	Tex0	: TEXCOORD0;
		float4	Color	: COLOR; 
	};

	struct	v2p_TL
	{
		float2 	Tex0	: TEXCOORD0;
		float4	Color	: COLOR;
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_TL
	{
		float2 	Tex0	: TEXCOORD0;
		float4	Color	: COLOR;
	};

	struct	v_TL2uv
	{
		float4	P		: POSITIONT;
		float2	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float4	Color	: COLOR; 
	};

	struct	v2p_TL2uv
	{
		float2 	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float4	Color	: COLOR;
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_TL2uv
	{
		float2 	Tex0	: TEXCOORD0;
		float2	Tex1	: TEXCOORD1;
		float4	Color	: COLOR;
	};
#endif