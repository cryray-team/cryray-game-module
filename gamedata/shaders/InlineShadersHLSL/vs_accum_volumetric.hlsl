//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\common.h"

cbuffer VolumetricLights
{
	float3	vMinBounds;
	float3	vMaxBounds;
	float4	FrustumClipPlane[6];
}

struct v2p
{
	float3 	lightToPos	: TEXCOORD0;		// light center to plane vector
	float3 	vPos		: TEXCOORD1;		// position in camera space
	float 	fDensity	: TEXCOORD2;		// plane density alon Z axis
	float3	clip0		: SV_ClipDistance0;
	float3	clip1		: SV_ClipDistance1;
	float4 	hpos		: SV_Position;
};

v2p main ( float3 P : POSITION )
{
	v2p 		o;
	float4	vPos;
	vPos.xyz 	= lerp( vMinBounds, vMaxBounds, P);	//	Position in camera space
	vPos.w 		= 1.f;
	o.hpos 		= mul(m_P, vPos);		// xform, input in camera coordinates

	o.lightToPos = vPos.xyz - Ldynamic_pos.xyz;
	o.vPos = vPos;

	o.fDensity = 1.f/40.f;

	for (int i=0; i<3; ++i)
	{
		o.clip0[i] = dot( o.hpos, FrustumClipPlane[i]);
		o.clip1[i] = dot( o.hpos, FrustumClipPlane[i+3]);
	}

	return o;
}