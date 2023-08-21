//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#if ( defined(MSAA_ALPHATEST_HIGH) || defined(MSAA_ALPHATEST_LOW) ) 
#define EXTEND_F_DEFFER_INLINE
#endif

#include "Headers\h_common.hlsl"
#include "Headers\h_sload.hlsl"

#ifdef	INLINE_ATOC
float4 	main	( p_bumped I ) : SV_Target
{
	surface_bumped 	S 		= sload 	(I);
	S.base.w = (S.base.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	return S.base;
}
#else	//	INLINE_ATOC
#ifdef	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_bumped I, float4 pos2d : SV_Position )
#else	//	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_bumped I )
#endif	//	MSAA_ALPHATEST_HIGH
{
	f_deffer		O;

#if !defined(MSAA_ALPHATEST_LOW)
	surface_bumped 	S 		= sload 	(I);

#if	!( defined(MSAA_ALPHATEST_HIGH) )
	clip					(S.base.w-def_aref);
#endif	//	!( defined(MSAA_ALPHATEST_HIGH) )
#ifdef 	MSAA_ALPHATEST_HIGH
	float alpha = (S.base.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	uint mask = alpha_to_coverage ( alpha, pos2d );
#endif	//	MSAA_ALPHATEST_HIGH

#else	//	!defined(MSAA_ALPHATEST_LOW)
	uint mask = 0x0;
	
	surface_bumped 	S 		= sload 	(I,MSAAOffsets[0]*(1.f/16.f));
	
	if( S.base.w-def_aref >= 0 ) mask |= 0x1;
	
	[unroll] for( int i = 1; i < MSAA_SAMPLES; ++i )
	{
		surface_bumped 	SI 		= sload 	(I,MSAAOffsets[i]*(1.f/16.f));
		if( SI.base.w-def_aref >= 0 ) mask |= ( uint(0x1) << i );
	}
	
	if( mask == 0x0 )
		discard;
#endif	//	!defined(MSAA_ALPHATEST_DX10_1)

	// FLORA FIXES & IMPROVEMENTS - SSS Update 14
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/

	// Normal from texture and force Y vector to generate a nice fake SSS and specular
	// Sun works extremly well with this fake normal, but point lights and others need a fix later on 'accum_base.ps' and 'accum_omni_unshadowed.ps'
	float3 Ne	= mul(m_V, float3(S.normal.x, 1, S.normal.y));
	Ne = normalize(Ne);

	// Material value ( MAT_FLORA )
	float 	ms	= 0.15f;

	// Fake gloss
	S.gloss = G_TREES_SPECULAR * saturate(S.base.g - (S.base.r + S.base.b) * 0.54f);
	// -----------------------------------------------------------------------

#ifdef USE_LM_HEMI
	float4	lm 	= s_hemi.Sample( smp_rtlinear, I.lmh);
	float 	h  	= get_hemi(lm);
#else
	float 	h	= I.position.w	;
#endif

#ifndef EXTEND_F_DEFFER_INLINE
	O = pack_gbuffer(
						float4	(Ne,										h),
  						float4 	(I.position.xyz + Ne*S.height*def_virtualh, ms),
  						float4	(S.base.rgb,								S.gloss) );
#else
	O = pack_gbuffer(
						float4	(Ne,										h),
  						float4 	(I.position.xyz + Ne*S.height*def_virtualh, ms),
  						float4	(S.base.rgb,								S.gloss),
  						mask );
#endif

	return 	O	;
}

#endif //	INLINE_ATOC