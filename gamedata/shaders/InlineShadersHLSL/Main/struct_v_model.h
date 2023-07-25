//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	V_MODEL_H
#define	V_MODEL_H
	struct	v_model
	{
		float4	P		: POSITION;		// (float,float,float,1)
		float3	N		: NORMAL;		// (nx,ny,nz)
		float3	T		: TANGENT;		// (nx,ny,nz)
		float3	B		: BINORMAL;		// (nx,ny,nz)
		float2	tc		: TEXCOORD0;	// (u,v)
	};
#endif