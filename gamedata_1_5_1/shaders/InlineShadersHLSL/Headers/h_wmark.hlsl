//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef  WMARK_H
#define  WMARK_H
#include "Headers\h_common.hlsl"

#define	NORMAL_SHIFT	0.007f
#define	MIN_SHIFT		0.003f
#define	MAX_SHIFT		0.011f
#define RANGE			100.f

float4 	wmark_shift 	(float3 pos, float3 norm)
{
	float3	P 	= 	pos;
	float3 	N 	= 	norm;
	float3	sd 	= 	eye_position-P;
	float 	d 	= 	length(sd);
	float 	w 	= 	min(d/RANGE,1.f);
	float 	s 	= 	lerp(MIN_SHIFT,MAX_SHIFT,d);
		P	+=	N.xyz*NORMAL_SHIFT;
		P	-=	normalize(eye_direction + normalize(P-eye_position)) * s;
	return	float4	(P,1.f);
}
#endif