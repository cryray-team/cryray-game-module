//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	V_AA_H
#define	V_AA_H
	struct	v_aa_AA
	{
		float4 P		:POSITIONT;
		float2 	Tex0	:TEXCOORD0;
		float2	Tex1	:TEXCOORD1;
		float2 	Tex2	:TEXCOORD2;
		float2	Tex3	:TEXCOORD3;
		float2	Tex4	:TEXCOORD4;
		float4	Tex5	:TEXCOORD5;
		float4	Tex6	:TEXCOORD6;
	};

	struct	v2p_aa_AA
	{
		float2 	Tex0	:TEXCOORD0;
		float2	Tex1	:TEXCOORD1;
		float2 	Tex2	:TEXCOORD2;
		float2	Tex3	:TEXCOORD3;
		float2	Tex4	:TEXCOORD4;
		float4	Tex5	:TEXCOORD5;
		float4	Tex6	:TEXCOORD6;
		float4 	HPos	:SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_aa_AA
	{
		float2 	Tex0	:TEXCOORD0;
		float2	Tex1	:TEXCOORD1;
		float2 	Tex2	:TEXCOORD2;
		float2	Tex3	:TEXCOORD3;
		float2	Tex4	:TEXCOORD4;
		float4	Tex5	:TEXCOORD5;
		float4	Tex6	:TEXCOORD6;
	};

	struct	p_aa_AA_sun
	{
		float2 	tc		:TEXCOORD0;
		float2	unused	:TEXCOORD1;
		float2 	LT		:TEXCOORD2;
		float2	RT		:TEXCOORD3;
		float2	LB		:TEXCOORD4;
		float2	RB		:TEXCOORD5;
	};
#endif