//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	STATIC_H
#define	STATIC_H
	struct v_static
	{
		float4	Nh		:NORMAL;	// (nx,ny,nz,hemi occlusion)
		float4	T		:TANGENT;	// tangent
		float4	B		:BINORMAL;	// binormal
		int2	tc		:TEXCOORD0;	// (u,v)
	#ifdef	USE_LM_HEMI
		int2	lmh		:TEXCOORD1;	// (lmu,lmv)
	#endif
		float4	P		:POSITION;	// (float,float,float,1)
	};

	struct v_static_color
	{
		float4	Nh		:NORMAL;	// (nx,ny,nz,hemi occlusion)
		float4	T		:TANGENT;	// tangent
		float4	B		:BINORMAL;	// binormal
		int2	tc		:TEXCOORD0;	// (u,v)
	#ifdef	USE_LM_HEMI
		int2	lmh		:TEXCOORD1;	// (lmu,lmv)
	#endif
		float4	color	:COLOR0;	// (r,g,b,dir-occlusion)	//	Swizzle before use!!!
		float4	P		:POSITION;	// (float,float,float,1)
	};
#endif