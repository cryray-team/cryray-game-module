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

uniform Texture2D s_distort;

struct 	v2p
{
 	float2 	tc: 		TEXCOORD0;	// base & distort
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main ( v2p I ) : SV_Target
{
	float2 	distort	= s_distort.Sample( smp_rtlinear, I.tc);
	float2	offset	= (distort.xy-0.5f)*def_distort;
	float3	image 	= s_base.Sample( smp_rtlinear, I.tc + offset);

	// out
	return  float4	(image,1.f);					// +mov
}
