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

uniform Texture2D s_distort;

struct 	v2p
{
 	float2 	tc0	: 	TEXCOORD0;	// base
	float4	c	:	COLOR0;		// diffuse
//	Igor: for additional depth dest
	float4 tctexgen	: TEXCOORD1;
	float   fog : FOG;			// fog
	float4 hpos	: SV_Position;
};


//////////////////////////////////////////////////////////////////////////////////////////
// Pixel

float4 main ( v2p I ) : SV_Target
{
	float4	distort = s_distort.Sample(smp_linear, I.tc0);
	float    factor = distort.a * dot(I.c.rgb,0.33f);
	float4	result = float4	(distort.rgb, factor);

	result.a *= I.fog*I.fog; // ForserX: Port Skyloader fog fix
	return result;
}