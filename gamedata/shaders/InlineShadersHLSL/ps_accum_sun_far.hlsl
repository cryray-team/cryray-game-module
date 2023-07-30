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

// Check Screen Space Shaders modules & addons
#include "ScreenSpaceAddon\check_screenspace.h"

#include "Headers\shadow.h"

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( v2p_volume I,  uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( v2p_volume I ) : SV_Target
#endif
{
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc.xy/I.tc.w, I.hpos, ISAMPLE) );

	float4 _P = float4( gbd.P, gbd.mtl );
	float4  _N = float4( gbd.N, gbd.hemi );
	float4 _C = float4( gbd.C, gbd.gloss );
	
	// ----- light-model
	float	m	= xmaterial;

			m 	= _P.w;

	float4	light	= plight_infinity ( m, _P, _N, _C, Ldynamic_dir );

	// ----- shadow
  	float4 	P4 	= float4( _P.x, _P.y, _P.z, 1.f);
	float4 	PS	= mul( m_shadow, P4 );
	
	// SHADOWS FIXES - SSS Update 14.7
	// New far edge fading code
	float s = shadow( PS ); // Far Shadows
	float shadows = sunmask( P4 ); // Clouds. Don't fade clouds.

	// Fade UV
	float2 FadeUV = PS.xy / PS.w;

	// Fade calc
	float4 fade = smoothstep(0.f, 0.1f, float4(FadeUV.x, 1.f - FadeUV.x, FadeUV.y, 1.f - FadeUV.y));
	float f = fade.x * fade.y * fade.z * fade.w;
	s += ( 1.f - s ) * ( 1.f - f );

	// Add Shadows
	shadows *= s;

#ifdef SSFX_ENHANCED_SHADERS
	return blend( float4( SRGBToLinear( Ldynamic_color.rgb * shadows.xxx ), 1.f ) * light, I.tc );
#else
	return blend( Ldynamic_color * light * shadows, I.tc );
#endif
}
