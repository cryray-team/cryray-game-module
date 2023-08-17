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

#ifdef USE_SSAO
	#include "AO\new_ssao_f.h"
	#include "AO\new_ssao_m.h"
#endif

#ifdef USE_SSDO_CUSTOM
	#include "AO\new_ssdo_custom_f.h"
	#include "AO\new_ssdo_custom_m.h"
#endif

#ifdef USE_SSDO_PLUS
	#include "AO\new_ssdo_plus_f.h"
	#include "AO\new_ssdo_plus_m.h"
	#include "AO\new_ssdo_plus_m_settings.h"
#endif

#ifdef USE_GTAO
	#include "AO\new_gtao_f.h"
	#include "AO\new_gtao_m.h"
#endif

#ifdef USE_HBAO_CUSTOM
	#include "AO\new_hbao_f.h"
	#include "AO\new_hbao_m.h"
#endif

#ifdef USE_HDAO_LOW
	#include "AO\new_hdao_low_f.h"
	#include "AO\new_hdao_low_m.h"
#endif

#ifdef USE_HDAO_HIGH
	#include "AO\new_hdao_high_f.h"
	#include "AO\new_hdao_high_m.h"
#endif

struct	_input
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float2	tcJ	: TEXCOORD1;	// jitter coords
	float4 pos2d : SV_Position;
};

#ifndef MSAA_ANTIALIASING_ENABLE
float4 main ( _input I): SV_Target
#else
float4 main ( _input I , uint iSample : SV_SAMPLEINDEX ): SV_Target
#endif
{
#ifdef OVERRIDE_MSAA_ISAMPLE
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0.xy, I.pos2d.xy, 0) );
#else	
	gbuffer_data gbd = gbuffer_load_data( GLD_P(I.tc0.xy, I.pos2d.xy, ISAMPLE) );
#endif	

	// Sample the buffers:
	float4	P = float4( gbd.P, gbd.mtl );	// position.(mtl or sun)
	float4	N = float4( gbd.N, gbd.hemi );		// normal.hemi

	float3 occ = float3(1.f, 1.f, 1.f);

#ifdef USE_SSAO
  	occ = calc_ssao(CS_P(P.xyz, N.xyz, I.tc0.xy, I.tcJ.xy, I.pos2d, ISAMPLE)).xxx;
#endif	

#ifdef USE_SSDO_CUSTOM
	occ = calc_ssdo(CS_P(P.xyz, N.xyz, I.tc0.xy, I.tcJ.xy, I.pos2d, ISAMPLE)).xxx;
#endif

#ifdef USE_SSDO_PLUS
#ifdef OVERRIDE_MSAA_ISAMPLE
  	occ = calc_ssdo(P.xyz, N.xyz, I.tc0.xy, I.pos2d, 0).xxx;
#else
	occ = calc_ssdo(P.xyz, N.xyz, I.tc0.xy, I.pos2d, ISAMPLE).xxx;
#endif
#endif

#ifdef USE_GTAO
	occ = calc_gtao(P.xyz, N.xyz, I.tc0.xy).xxx;
#endif

#ifdef USE_HBAO_CUSTOM
	occ = calc_hbao(P.z, N.xyz, I.tc0.xy, I.pos2d).xxx;
#endif

#ifdef USE_HDAO_LOW
	occ = calc_hdao(CS_P(P, N, I.tc0, I.tcJ, I.pos2d, ISAMPLE)).xxx;
#endif

#ifdef USE_HDAO_HIGH	
	occ = calc_new_hdao(CS_P(P, N, I.tc0, I.tcJ, I.pos2d, ISAMPLE)).xxx;
#endif
	
	return float4(occ, 1.f);
}