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

// SSS Implementation
#include "ScreenSpaceAddon\screenspace_shadows.h"

#include "Headers\shadow.h"

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( v2p_volume I,  uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( v2p_volume I ) : SV_Target
#endif
{
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc.xy/I.tc.w, I.hpos.xy, ISAMPLE) );

	float4 _P = float4( gbd.P, gbd.mtl );
	float4  _N = float4( gbd.N, gbd.hemi );
	float4 _C = float4( gbd.C, gbd.gloss );
	
	// ----- light-model
	float	m	= xmaterial;

			m 	= _P.w;

	float4	light	= plight_infinity ( m, _P.xyz, _N.xyz, _C, Ldynamic_dir.xyz );

	// ----- shadow
  	float4 	P4 	= float4( _P.x, _P.y, _P.z, 1.f);
	float4 	PS	= mul( m_shadow, P4 );
	float 	s 	= sunmask( P4 );
			s *= shadow( PS );
			
			//s *= SSFX_ScreenSpaceShadows(_P, I.hpos, ISAMPLE);

#ifdef SSFX_ENHANCED_SHADERS // We have Enhanced Shaders installed
	return 	float4( SRGBToLinear(Ldynamic_color.rgb * s.xxx),1.f) * light;
#else
	return 	float4( Ldynamic_color.rgb * light.xxx * s.xxx, 1.f);
#endif
}