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

#ifdef ISAMPLE
#	undef ISAMPLE
#endif // #ifdef ISAMPLE

#define ISAMPLE 0

struct	_input
{
	float4	tc0		: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float2	tcJ		: TEXCOORD1;	// jitter coords
	float4	pos2d	: SV_Position;
};

float4 main ( _input I ) : SV_Target0
{
	//gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0.xy, I.pos2d * 2, ISAMPLE) );
	//	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0.xy + 0.5f / pos_decompression_params2.xy, I.pos2d * 2, ISAMPLE) );
	//	TODO: move it to C++ code to save maths in PS
	//	??? Why we move x and y in the same direction???
	gbuffer_data gbd0 = gbuffer_load_data( GLD_P(I.tc0.xy + 0.5f * screen_res.zw, I.pos2d * 2, ISAMPLE) );
	gbuffer_data gbd1 = gbuffer_load_data( GLD_P(I.tc0.xy - 0.5f * screen_res.zw, I.pos2d * 2, ISAMPLE) );
	gbuffer_data gbd2 = gbuffer_load_data( GLD_P(I.tc0.xy + 0.5f * float2(+screen_res.z, -screen_res.w), I.pos2d * 2, ISAMPLE) );
	gbuffer_data gbd3 = gbuffer_load_data( GLD_P(I.tc0.xy + 0.5f * float2(-screen_res.z, +screen_res.w), I.pos2d * 2, ISAMPLE) );

	gbuffer_data gbd = gbd0;
	if (gbd1.P.z < gbd.P.z) gbd = gbd1;	
	if (gbd2.P.z < gbd.P.z) gbd = gbd2;	
	if (gbd3.P.z < gbd.P.z) gbd = gbd3;

//	gbd.P.z = (gbd0.P.z + gbd1.P.z + gbd2.P.z + gbd3.P.z)  * 0.25f;

	float4	P = float4( gbd.P, gbd.mtl );	// position.(mtl or sun)

//	SSAO_OPT_DATA == 2	for half-res data
//	SSAO_OPT_DATA == 1	for full-res data

	gbd = gbuffer_load_data( GLD_P(I.tc0.xy, I.pos2d, ISAMPLE) );
	return  float4(gbd.P.zzzz);
}