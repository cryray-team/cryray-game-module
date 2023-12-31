//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

struct 	v2p
{
 	float2 	tc0	: TEXCOORD0;	// base
  	float3	c0	: COLOR0;
	float	fog	: FOG;
};


//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main( v2p I ) : SV_Target
{
	float4	t_base = s_base.Sample( smp_base, I.tc0 );
	float3	final 	= t_base*I.c0*2.f;

	//	Fogging
	final 	= lerp(fog_color, final, I.fog);

	// out
	return  float4	(final, t_base.a*I.fog*I.fog);
}
