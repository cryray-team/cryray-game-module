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

#ifdef USE_SSAO
	#include "AO\h_new_ssao_f.hlsl"
	#include "AO\h_new_ssao_m.hlsl"
#endif

#ifdef USE_SSDO_CUSTOM
	#include "AO\h_new_ssdo_custom_f.hlsl"
	#include "AO\h_new_ssdo_custom_m.hlsl"
#endif

#ifdef USE_SSDO_PLUS
	#include "AO\h_new_ssdo_plus_f.hlsl"
	#include "AO\h_new_ssdo_plus_m.hlsl"
	#include "AO\h_new_ssdo_plus_m_settings.hlsl"
#endif

#ifdef USE_GTAO
	#include "AO\h_new_gtao_f.hlsl"
	#include "AO\h_new_gtao_m.hlsl"
#endif

#ifdef USE_HBAO_CUSTOM
	#include "AO\h_new_hbao_f.hlsl"
	#include "AO\h_new_hbao_m.hlsl"
#endif

#ifdef USE_HDAO_LOW
	#include "AO\h_new_hdao_low_f.hlsl"
	#include "AO\h_new_hdao_low_m.hlsl"
#endif

#ifdef USE_HDAO_HIGH
	#include "AO\h_new_hdao_high_f.hlsl"
	#include "AO\h_new_hdao_high_m.hlsl"
#endif

struct	_input
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float2	tcJ	: TEXCOORD1;	// jitter coords
	float4 pos2d : SV_Position;
};

float4 main ( _input I): SV_Target
{
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0, I.pos2d, 0) );

	// Sample the buffers:
	float4	P = float4( gbd.P, gbd.mtl );	// position.(mtl or sun)
	float4	N = float4( gbd.N, gbd.hemi );		// normal.hemi

	float3 occ = 1.f;

#ifdef USE_SSAO
  	occ = calc_ssao(CS_P(P, N, I.tc0, I.tcJ, I.pos2d, 0)).xxx;
#endif	

#ifdef USE_SSDO_CUSTOM
	occ = calc_ssdo(CS_P(P, N, I.tc0, I.tcJ, I.pos2d));
#endif

#ifdef USE_SSDO_PLUS
	occ = calc_ssdo(P, N, I.tc0, I.pos2d).xxx;
#endif

#ifdef USE_GTAO
	occ = calc_gtao(P, N, I.tc0.xy);
#endif

#ifdef USE_HBAO_CUSTOM
	occ = calc_hbao(P.z, N, I.tc0, I.pos2d).xxx;
#endif

#ifdef USE_HDAO_LOW
	occ = calc_hdao(CS_P(P, N, I.tc0, I.tcJ, I.pos2d, 0)).xxx;
#endif

#ifdef USE_HDAO_HIGH	
	occ = calc_new_hdao(CS_P(P, N, I.tc0, I.tcJ, I.pos2d, 0));
#endif
	
	return float4(occ, 0.f);
}