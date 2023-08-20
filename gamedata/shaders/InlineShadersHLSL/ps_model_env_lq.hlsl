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

struct 	v2p
{
 	float2 	tc0	: TEXCOORD0;	// base
 	float3 	tc1	: TEXCOORD1;	// environment
  	float3	c0	: COLOR0;		// sun
	float  fog	: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
//uniform samplerCUBE 	s_env	;
TextureCube	s_env;		//	Environment for forward rendering

float4 	main	( v2p I )	: SV_Target
{
//	float4	t_base 	= tex2D		(s_base,I.tc0);
//	float4	t_env 	= texCUBE	(s_env,	I.tc1);
	float4	t_base 	= s_base.Sample( smp_base, I.tc0);
	float4	t_env 	= s_env.Sample( smp_rtlinear, I.tc1);

	float3 	base 	= lerp		(t_env.xyz,t_base.xyz,t_base.a);
	float3	light	= I.c0.xyz;
	float3	final 	= light*base*2.f;

	//	Fogging
	final 	= lerp(fog_color.rgb, final, I.fog);

	// out
	return  float4	(final.r,final.g,final.b,t_base.a*I.fog*I.fog);
}
