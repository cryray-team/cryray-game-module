//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef _WATERMOVE_H
#define _WATERMOVE_H

float4	watermove	(float4 P)	{
	float3 	wave1	= float3(0.11f,0.13f,0.07f)*W_POSITION_SHIFT_SPEED	;
	float 	dh	= sin  	(timers.x+dot((float3)P,wave1))			;
			P.y	+= dh * W_POSITION_SHIFT_HEIGHT	;
	return 	P	;
}
float2	watermove_tc	(float2 base, float2 P, float amp)	{
	float2 	wave1	= 	float2	(0.2111f,0.2333f)*amp	;
	float 	angle 	= 	timers.z + dot (P,wave1)	;
	float 	du	= 	sin  	(angle);
	float 	dv	= 	cos  	(angle);
		return	(base + amp*float2(du,dv));
}

float3	waterrefl	(out float amount, float3 P, float3 N)	{
	float3 	v2point	= normalize	(P-eye_position);
	float3	vreflect= reflect	(v2point, N);
	float 	fresnel	= (0.5f + 0.5f*dot(vreflect,v2point));
			amount	= 1.f - fresnel*fresnel;			// 0=full env, 1=no env
	return	vreflect;
}

#endif
