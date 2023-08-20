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
#include "Headers\waterconfig.h"

#define POWER    0.5f

uniform Texture2D s_distort;

struct v2p
{
	float2 tbase	: TEXCOORD0;
	float2 tdist0	: TEXCOORD1;
	float2 tdist1	: TEXCOORD2;
	float4	tctexgen: TEXCOORD3;
	float4 hpos		: SV_Position;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main( v2p I ) : SV_Target
{
	float4	t_base	= s_base.Sample( smp_base, I.tbase  );

	float2	t_d0	= s_distort.Sample( smp_base, I.tdist0 );
	float2	t_d1	= s_distort.Sample( smp_base, I.tdist1 );
	float2	distort	= (t_d0+t_d1)*0.5f;                      // average
	float2	zero	= float2( 0.5f, 0.5f );
	float2	faded	= lerp( distort, zero, t_base.a );

	//	Igor: additional depth test
	float	alphaDistort;
	float2 PosTc = I.tctexgen.xy/I.tctexgen.z;
	gbuffer_data gbd = gbuffer_load_data( PosTc, I.hpos );
	float4 _P = 	float4( gbd.P, gbd.mtl );
	float 	waterDepth = _P.z-I.tctexgen.z;
	alphaDistort = saturate(5.f*waterDepth);
	faded 	= lerp  ( zero, faded, alphaDistort);

	float2	faded_bx2	= (faded*2.f-1.f)*W_DISTORT_POWER;
	float	faded_dot	= dot        (float3(faded_bx2,0.f),0.75f        );
	float	alpha		= 0.5f        ;


	faded	= faded*POWER - 0.5f*POWER + 0.5f        ;

        // out
	//	Igor: need for alpha water
	return  float4( faded, 0.f, alpha );
}
