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
#include "Headers\h_shadow.hlsl"

float4 main( p_aa_AA_sun I, float4 pos2d : SV_Position ) : SV_Target
{
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc, pos2d, ISAMPLE) );

	float4 	_P	= float4( gbd.P, gbd.mtl );
	float4	_N	= float4( gbd.N, gbd.hemi );
	float4	_C	= float4( gbd.C, gbd.gloss );
	
	// ----- light-model
	float m = xmaterial;

	m = _P.w;

	float4 light = plight_infinity( m, _P, _N, _C, Ldynamic_dir );

	// ----- shadow
	float4 s_sum;
	s_sum.x	= s_smap.Sample( smp_nofilter, I.LT).x;
	s_sum.y = s_smap.Sample( smp_nofilter, I.RT).y;
	s_sum.z	= s_smap.Sample( smp_nofilter, I.LB).z;
	s_sum.w = s_smap.Sample( smp_nofilter, I.RB).w;

	float s = ((s_sum.x+s_sum.y)+(s_sum.z+s_sum.w))*(1.f/4.f);

	return Ldynamic_color * light * s;
}
