//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	SHADOWS_H
#define	SHADOWS_H
	struct	v_shadow_direct_aref
	{
		float4	P		: POSITION;		// (float,float,float,1)
		int4	tc		: TEXCOORD0;	// (u,v,frac,???)
	};

	struct	v_shadow_direct
	{
		float4	P		: POSITION;		// (float,float,float,1)
	};

	struct	v2p_shadow_direct_aref
	{
		float2	tc0		: TEXCOORD1;	// Diffuse map for aref
		float4	hpos	: SV_Position;	// Clip-space position         (for rasterization)
	};

	struct	v2p_shadow_direct
	{
		float4	hpos	: SV_Position;		// Clip-space position         (for rasterization)
	};

	struct	p_shadow_direct_aref
	{
		float2	tc0		: TEXCOORD1;	// Diffuse map for aref
	};
#endif