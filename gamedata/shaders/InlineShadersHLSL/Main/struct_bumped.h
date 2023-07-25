//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	BUMPED_H
#define	BUMPED_H
	struct v2p_bumped
	{
		float2	tcdh	: TEXCOORD0;	// Texture coordinates
		float4	position: TEXCOORD1;	// position + hemi
		float3	M1		: TEXCOORD2;	// nmap 2 eye - 1
		float3	M2		: TEXCOORD3;	// nmap 2 eye - 2
		float3	M3		: TEXCOORD4;	// nmap 2 eye - 3
	#ifdef USE_TDETAIL
		float2	tcdbump	: TEXCOORD5;	// d-bump
	#endif
	#ifdef USE_LM_HEMI
			float2	lmh	: TEXCOORD6;	// lm-hemi
	#endif
		float4	hpos	: SV_Position;
	};

	struct p_bumped
	{
		float2	tcdh	: TEXCOORD0;	// Texture coordinates
		float4	position: TEXCOORD1;	// position + hemi
		float3	M1		: TEXCOORD2;	// nmap 2 eye - 1
		float3	M2		: TEXCOORD3;	// nmap 2 eye - 2
		float3	M3		: TEXCOORD4;	// nmap 2 eye - 3
	#ifdef USE_TDETAIL
		float2	tcdbump	: TEXCOORD5;	// d-bump
	#endif
	#ifdef USE_LM_HEMI
			float2	lmh	: TEXCOORD6;	// lm-hemi
	#endif
	};
#endif