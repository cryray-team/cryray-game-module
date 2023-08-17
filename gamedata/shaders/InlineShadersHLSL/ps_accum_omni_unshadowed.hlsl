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
								  
//	TODO: DX10: Move to Load
#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( float4 tc:TEXCOORD0, float4 pos2d : SV_Position, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#else
float4 main ( float4 tc:TEXCOORD0, float4 pos2d : SV_Position ) : SV_Target
#endif
{
	const float bias_mul = 0.999f;

 	// Sample the fat framebuffer:
	float2	tcProj	= tc.xy / tc.w;
	gbuffer_data gbd = gbuffer_load_data( GLD_P(tcProj.xy, pos2d.xy, ISAMPLE) );

	float4 _P = float4( gbd.P,gbd.mtl );
	float4 _N = float4( gbd.N,gbd.hemi );
	float4 _C = float4( gbd.C, gbd.gloss );
	
	float m = xmaterial	;
	
	m = _P.w;
	
	// FLORA FIXES & IMPROVEMENTS - SSS Update 14.2
	// Fix Flora ilumination ( Align normal to light )
	if(abs(m - MAT_FLORA) <= 0.05f)
		_N.rgb = -normalize(_P.xyz - Ldynamic_pos.xyz);
	float rsqr;
	float4 light = plight_local( m, _P.xyz, _N.xyz, _C, Ldynamic_pos.xyz, Ldynamic_pos.w, rsqr );
	
#ifdef SSFX_ENHANCED_SHADERS
	return float4(float4( SRGBToLinear(Ldynamic_color.rgb),1.f) * light);
#else
	return float4(Ldynamic_color * light);
#endif
}
