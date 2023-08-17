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

float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4 col: COLOR, float4 pos2d : SV_Position ) : SV_Target
{
#if MSAA_SAMPLES
	gbuffer_data gbd0 = gbuffer_load_data( tc.xy, pos2d.xy, 0 );

	
	float3 P0 = gbd0.P;
	float3 N0 = gbd0.N;
	
	float3 P = gbd0.P / float(MSAA_SAMPLES);
	float3 N = gbd0.N / float(MSAA_SAMPLES);

	[unroll] for( int i = 1; i < MSAA_SAMPLES; i++ )
	{
		gbuffer_data gbd = gbuffer_load_data( tc.xy, pos2d.xy, i );

		P += gbd.P / float(MSAA_SAMPLES);
		N += gbd.N / float(MSAA_SAMPLES);
	}
	
	if (all(P==P0 && N==N0))
	   discard;

#endif // #if MSAA_SAMPLES
	return float4(0.f,0.f,0.f,0.f);
}
