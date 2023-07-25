//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	DEFFER_FLAT_H
#define	DEFFER_FLAT_H
	struct	v2p_flat
	{
	#ifdef USE_GRASS_WAVE
		float4	tcdh	: TEXCOORD0;	// Texture coordinates,         w=sun_occlusion
	#else
		float2	tcdh	: TEXCOORD0;	// Texture coordinates
	#endif
		float4	position: TEXCOORD1;	// position + hemi
		float3	N		: TEXCOORD2;	// Eye-space normal        (for lighting)
	#ifdef USE_TDETAIL
		float2	tcdbump	: TEXCOORD3;	// d-bump
	#endif
	#ifdef USE_LM_HEMI
		float2	lmh		: TEXCOORD4;	// lm-hemi
	#endif
		float4	hpos	: SV_Position;
	};

	struct	p_flat
	{
	#ifdef USE_GRASS_WAVE
		float4	tcdh	: TEXCOORD0;	// Texture coordinates,         w=sun_occlusion
	#else
		float2	tcdh	: TEXCOORD0;	// Texture coordinates
	#endif
		float4	position: TEXCOORD1;	// position + hemi
		float3	N		: TEXCOORD2;	// Eye-space normal        (for lighting)
	#ifdef USE_TDETAIL
		float2	tcdbump	: TEXCOORD3;	// d-bump
	#endif
	#ifdef USE_LM_HEMI
		float2	lmh		: TEXCOORD4;	// lm-hemi
	#endif
	};
#endif