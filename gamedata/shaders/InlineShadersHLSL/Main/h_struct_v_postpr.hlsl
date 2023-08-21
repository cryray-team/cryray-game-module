//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	PSTR_H
#define	PSTR_H
	struct	v_postpr
	{
		float4	P		: POSITIONT;
		float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
		float2	Tex1	: TEXCOORD1;	// base2 (duality)
		float2	Tex2	: TEXCOORD2;	// base  (noise)
		float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
		float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
	};

	struct	v2p_postpr
	{
		float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
		float2	Tex1	: TEXCOORD1;	// base2 (duality)
		float2	Tex2	: TEXCOORD2;	// base  (noise)
		float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
		float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
		float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
	};

	struct	p_postpr
	{
		float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
		float2	Tex1	: TEXCOORD1;	// base2 (duality)
		float2	Tex2	: TEXCOORD2;	// base  (noise)
		float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
		float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
	};
#endif