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
#include "Headers\h_lmodel.hlsl"

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
	float3 _N = s_patched_normal.Sample( smp_nofilter, tc );
#else
#ifndef INLINE_MSAA_OPTIMIZATION
	float3 _N = s_patched_normal.Load(int3( tc * screen_res.xy, 0 ), ISAMPLE );
#else
	float3 _N = s_patched_normal.Load(int3( tc * screen_res.xy, 0 ), iSample);
#endif	
#endif

	return float4( gbuf_pack_normal( _N ), 0.f, 0.f );
}
