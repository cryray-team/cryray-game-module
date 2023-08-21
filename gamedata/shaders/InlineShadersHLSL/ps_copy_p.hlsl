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

#ifndef ISAMPLE
#define ISAMPLE 0
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
//	TODO: DX10: move to load instead of sample (will need to provide integer texture coordinates)
#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( float4 tc : TEXCOORD0, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( float4 tc : TEXCOORD0 ) : SV_Target
#endif
{
	//return	tex2Dproj	(s_base,tc);

	//	Perform texture coordinates projection.
	tc.xy /= tc.w;
#ifndef MSAA_ANTIALIASING_ENABLE
	return		s_generic.Sample( smp_nofilter, tc );
#else
#ifdef INLINE_MSAA_OPTIMIZATION
	return		s_generic.Load( int3( tc * screen_res.xy, 0 ), iSample );
#else
	return		s_generic.Load( int3( tc * screen_res.xy, 0 ), ISAMPLE );
#endif
#endif
}