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

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
// Note: this is a float-sphere
#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( float4 tc:TEXCOORD0, float4 pos2d : SV_Position, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( float4 tc:TEXCOORD0, float4 pos2d : SV_Position ) : SV_Target
#endif
{
	float2	tcProj			= tc.xy / tc.w;

	gbuffer_data gbd = gbuffer_load_data( GLD_P(tcProj.xy, pos2d.xy, ISAMPLE) );

	float4	_P				= float4( gbd.P, gbd.mtl );
	float4	_N				= float4( gbd.N, gbd.hemi );

	float3	L2P 	= _P.xyz - Ldynamic_pos.xyz;			// light2point
	float3 	L2P_N 	= normalize	( L2P );					// light2point
	float	rsqr	= dot		( L2P, L2P );				// distance 2 light (squared)
	float  	att 	= saturate	( 1.f - rsqr*Ldynamic_pos.w );// q-linear attenuate
	float  	light	= saturate	( dot( -L2P_N, _N.xyz ) );
	float	hemi 	= saturate	( dot( L2P_N, direction ) );

  // Final color
	return 	float4( Ldynamic_color.xyz * att * light * hemi, 0.f );
}