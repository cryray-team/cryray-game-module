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

//	TODO: DX10: move to load instead of sample (will need to provide integer texture coordinates)
#define EPS	(0.9f/255.0)
#define CLIP_THRESHOLD	(1.0f/255.0)

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( p_TL I, float4 pos2d : SV_Position, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( p_TL I, float4 pos2d : SV_Position ) : SV_Target
#endif
{
  	// Sample the fat framebuffer:
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.Tex0, pos2d, ISAMPLE) );

	float4 NH = float4( gbd.N, gbd.hemi );
  	float L = NH.w * dot( Ldynamic_dir, (float3)NH ) + EPS; // Use hemisphere as approximation of max light

	clip(L-CLIP_THRESHOLD);

  	return	float4( L, L, L, L );
}

