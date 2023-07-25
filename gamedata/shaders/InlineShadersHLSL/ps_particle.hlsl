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

struct v2p
{
	float2 tc0	: TEXCOORD0;
	float4 c	: COLOR0;
//	Igor: for additional depth dest
	float4 tctexgen	: TEXCOORD1;
	float  fog : FOG;		// fog
	float4 hpos	: SV_Position;
};

//	Must be less than view near
#define	DEPTH_EPSILON	0.1h
//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main( v2p I ) : SV_Target
{
	float4 result = I.c * s_base.Sample(smp_base, I.tc0);

	//	Igor: additional depth test

	float2 tcProj = I.tctexgen.xy / I.tctexgen.w;

	gbuffer_data gbd = gbuffer_load_data(tcProj, I.hpos);

	float4 _P = float4(gbd.P, gbd.mtl);
	float spaceDepth = _P.z - I.tctexgen.z - DEPTH_EPSILON;
	if (spaceDepth < -2 * DEPTH_EPSILON) spaceDepth = 100000.f; //  Skybox doesn't draw into position buffer
	result.a *= Contrast(saturate(spaceDepth*1.3f), 2.f);
	result.rgb *= Contrast(saturate(spaceDepth*1.3f), 2.f);

	clip(result.a - (0.01f / 255.0f));
	result.w *= I.fog;
	return	result;
}