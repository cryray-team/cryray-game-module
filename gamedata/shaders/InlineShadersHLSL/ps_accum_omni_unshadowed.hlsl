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
								  
float4 main ( float4 tc:TEXCOORD0, float4 pos2d : SV_Position ) : SV_Target
{
	const float bias_mul = 0.999f;

 	// Sample the fat framebuffer:
	float2	tcProj	= tc.xy / tc.w;
	gbuffer_data gbd = gbuffer_load_data( GLD_P(tcProj, pos2d, ISAMPLE) );

	float4 _P = float4( gbd.P,gbd.mtl );
	float4 _N = float4( gbd.N,gbd.hemi );
	float4 _C = float4( gbd.C, gbd.gloss );
	
	float m = xmaterial	;
	
	m = _P.w;
	
	// FLORA FIXES & IMPROVEMENTS - SSS Update 14.2
	// Fix Flora ilumination ( Align normal to light )
	if(abs(m - MAT_FLORA) <= 0.05f)
		_N.rgb = -normalize(_P - Ldynamic_pos.xyz);
	float rsqr;
	float4 light = plight_local( m, _P, _N, _C, Ldynamic_pos, Ldynamic_pos.w, rsqr );
	
#ifdef SSFX_ENHANCED_SHADERS
	return float4(float4( SRGBToLinear(Ldynamic_color.rgb),1.f) * light);
#else
	return float4(Ldynamic_color * light);
#endif
}
