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

float4 	main	( p_flat I ) : SV_Target
{
	float4 D 	= s_base.Sample (smp_linear, I.tcdh); //tbase (I.tcdh);
	D.w = (D.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	return D;
}

#else	//	INLINE_ATOC

#ifdef	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_flat I, float4 pos2d : SV_Position )
#else	//	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_flat I )
#endif	//	MSAA_ALPHATEST_HIGH
{
  f_deffer	O;

  // 1. Base texture + kill pixels with low alpha
  
#if !defined(MSAA_ALPHATEST_LOW)

  float4 D = s_base.Sample (smp_linear, I.tcdh);

#if	!( defined(MSAA_ALPHATEST_HIGH))
	clip((D.w-def_aref*0.5f)/(1.f-def_aref*0.5f));
#endif	//	!( defined(MSAA_ALPHATEST_HIGH))
#ifdef 	MSAA_ALPHATEST_HIGH
	float alpha = (D.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	uint mask = alpha_to_coverage ( alpha, pos2d );
#endif	//	MSAA_ALPHATEST_HIGH

#else
  uint mask = 0x0;
  float2 ddx_base = ddx( I.tcdh );
  float2 ddy_base = ddy( I.tcdh );

  float2 pixeloffset = MSAAOffsets[0]*(1.f/16.f);
  
  float2 texeloffset =  pixeloffset.x * ddx_base + pixeloffset.y * ddy_base;
      	
  float4 D = s_base.Sample (smp_linear, I.tcdh+texeloffset);
	
  if( D.w-def_aref >= 0 ) mask |= 0x1;
	
  [unroll] for( int i = 1; i < MSAA_SAMPLES; ++i )
  {
	pixeloffset = MSAAOffsets[i]*(1.f/16.f);
	texeloffset =  pixeloffset.x * ddx_base + pixeloffset.y * ddy_base;
	float4 DI 	= tbase(I.tcdh+texeloffset);
	if( DI.w-def_aref >= 0 ) mask |= ( uint(0x1) << i );
  }
	
  if( mask == 0x0 )
	discard;
#endif

#ifdef	USE_TDETAIL
//	D.rgb	= 2*D.rgb*tex2D	(s_detail, I.tcdbump).rgb;
	D.rgb	= 2.f*D.rgb * s_detail.Sample( smp_base, I.tcdbump ).rgb;
#endif

	// hemi,sun,material
	float 	ms	= xmaterial		;
#ifdef USE_LM_HEMI
//	float4	lm 	= tex2D			(s_hemi, I.lmh);
	float4	lm 	= s_hemi.Sample( smp_rtlinear, I.lmh);
//	float 	h  	= dot			(lm.rgb,1.h/3.h);
	float 	h  	= get_hemi(lm);
#else
	float 	h	= I.position.w	;
#endif

	D *= I.tcdh.z;

  // 2. Standart output
  float4 Ne     = float4	(normalize((float3)I.N.xyz), 					h			);
#if !( defined(MSAA_ALPHATEST_HIGH) || defined(MSAA_ALPHATEST_LOW) )
  O				= pack_gbuffer(
  								Ne,
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f	, 	ms			),
								float4	(D.rgb,											def_gloss	) );
#else
  O				= pack_gbuffer(
  								Ne,
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f	, 	ms			),
								float4	(D.rgb,											def_gloss	),
								mask );
#endif
  return O;
}

#endif	//	INLINE_ATOC
