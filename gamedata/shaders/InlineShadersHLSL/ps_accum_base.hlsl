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
#include "Headers\shadow.h"

//////////////////////////////////////////////////////////////////////////////////////////
// This is the basic primitive used by convex, volumetric lights
// for example spot-lights, one face of the omni lights, etc.
//////////////////////////////////////////////////////////////////////////////////////////
// following options are available to configure compilation:
// USE_LMAP
// USE_LMAPXFORM
// USE_SHADOW
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main( p_volume I, float4 pos2d : SV_Position, uint iSample : SV_SAMPLEINDEX  ) : SV_Target
#else
float4 main( p_volume I, float4 pos2d : SV_Position ) : SV_Target
#endif
{
	float2	tcProj			= I.tc.xy / I.tc.w;

	gbuffer_data gbd = gbuffer_load_data(GLD_P(tcProj.xy, pos2d.xy, ISAMPLE));

	float4 _P				= float4( gbd.P, gbd.mtl );
	float4 _N				= float4( gbd.N, gbd.hemi );
	float4 _C				= float4( gbd.C, gbd.gloss );
	
	float 	m	= xmaterial	;
			m 	= _P.w		;
	
	// FLORA FIXES & IMPROVEMENTS - SSS Update 14.2
	// Fix Flora ilumination ( Align normal to light )
	if (abs(m - MAT_FLORA) <= 0.05f)
		_N.rgb = -normalize(_P.xyz - Ldynamic_pos.xyz);
	
        // ----- light-model
      	float        		rsqr;
        float4        		light   = plight_local( m, _P.xyz, _N.xyz, _C, Ldynamic_pos.xyz, Ldynamic_pos.w, rsqr );

        // ----- shadow
		float4          		P4      = float4( _P.x, _P.y, _P.z, 1.f);
        float4         		PS      = mul( m_shadow, P4 );
        float         		s		= 1.f;
        #ifdef  USE_SHADOW
							s		= shadow( PS );
        #endif

        // ----- lightmap
        float4         lightmap = 1.f;
        #ifdef        USE_LMAP
                #ifdef         USE_LMAPXFORM
							PS.x	= dot( P4, m_lmap[0] );
							PS.y	= dot( P4, m_lmap[1] );
                #endif

				//	Can use linear with mip point
				lightmap = s_lmap.SampleLevel(smp_rtlinear, PS.xy / PS.w, 0 );        //
        #endif

#ifdef SSFX_ENHANCED_SHADERS
		return  float4(SRGBToLinear(Ldynamic_color.rgb * lightmap.rgb * s.xxx),1.f) * light;
#else
		return  float4( Ldynamic_color * light * s * lightmap);
#endif
}
