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
 	float2 	tc0: 	TEXCOORD0;	
 	float4	c0:		COLOR0;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main ( p_TL I ) : SV_Target
{
	float4 r 	= s_base.Sample( smp_base, I.Tex0 );
	r.rgb		= I.Color.rgb;
	r.a			*= I.Color.a;
	return r;
}