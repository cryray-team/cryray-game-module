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
#include "Headers\lmodel.h"

#ifndef ISAMPLE
	#define ISAMPLE 0
#endif

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1 ) : SV_Target
#endif
{
#ifndef MSAA_ANTIALIASING_ENABLE
	float Gloss = s_patched_normal.Sample( smp_nofilter, tc ).a;
#else
#ifndef INLINE_MSAA_OPTIMIZATION
	float Gloss = s_patched_normal.Load(int3( tc * pos_decompression_params2.xy, 0 ), ISAMPLE ).a;
#else
	float Gloss = s_patched_normal.Load(int3( tc * pos_decompression_params2.xy, 0 ), iSample).a;
#endif	
#endif

	float ColorIntencity = 1.f - sqrt(Gloss);
	ColorIntencity = max (ColorIntencity, 0.5f);
	
	return float4( ColorIntencity, ColorIntencity, ColorIntencity, Gloss*0.8f);
}