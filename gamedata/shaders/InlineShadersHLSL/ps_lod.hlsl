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

#include "Headers\common.h"
#include "Headers\sload.h"

struct 	vf
{
	float3	position	: TEXCOORD0;
 	float2	tc0			: TEXCOORD1;	// base0
 	float2	tc1			: TEXCOORD2;	// base1
	float4	af			: COLOR1;		// alpha&factor
	float4	hpos		: SV_Position;
};

void sample_Textures( inout float4 D, inout float4 H, float2 tc1, float2 tc0, float4 af, float2 pixeloffset )
{
  float2 ddx_tc0 = ddx( tc0 );
  float2 ddy_tc0 = ddy( tc0 );
  float2 ddx_tc1 = ddx( tc1 );
  float2 ddy_tc1 = ddy( tc1 );

  float4 D1 	= s_base.Sample( smp_base, tc1 + pixeloffset.x * ddx_tc1 + pixeloffset.y * ddy_tc1 );
  float4 D0 	= s_base.Sample( smp_base, tc0 + pixeloffset.x * ddx_tc0 + pixeloffset.y * ddy_tc0 );
  float4 H0 	= s_hemi.Sample( smp_linear, tc0 + pixeloffset.x * ddx_tc0 + pixeloffset.y * ddy_tc0 );	H0.xyz 	=  H0.rgb*2-1;	// expand
  float4 H1 	= s_hemi.Sample( smp_linear, tc1 + pixeloffset.x * ddx_tc1 + pixeloffset.y * ddy_tc1 );	H1.xyz 	=  H1.rgb*2-1;	// expand

  D 	= lerp		(D0,D1,af.w);		D.w	*= af.z;	// multiply alpha
  H 	= lerp		(H0,H1,af.w);		H.w	*= af.x;	// multiply hemi
}

#ifdef	INLINE_ATOC

float4 	main	( vf I ) : SV_Target
{
	float4 D;
	float4 H;
	sample_Textures(D, H, I.tc1, I.tc0, I.af, float2(0.f,0.f) );
	D.w = (D.w-(96.f/255.f)*0.5f)/(1.f-(96.f/255.f)*0.5f);
	return D.w;
}

#else	//	INLINE_ATOC

#ifdef	MSAA_ALPHATEST_HIGH
f_deffer 	main	( vf I, float4 pos2d : SV_Position )
#else	//	MSAA_ALPHATEST_HIGH
f_deffer 	main	( vf I )
#endif	//	MSAA_ALPHATEST_HIGH
{
  f_deffer	O;

  // 1. Base texture + kill pixels with low alpha
//  float4 D0 	= tex2D		(s_base, I.tc0);
//  float4 D1 	= tex2D		(s_base, I.tc1);
//  float4 H0 	= tex2D		(s_hemi, I.tc0);	H0.xyz 	=  H0.rgb*2-1;	// expand
//  float4 H1 	= tex2D		(s_hemi, I.tc1);	H1.xyz 	=  H1.rgb*2-1;	// expand
  float4 D;
  float4 H;
  
#if !defined(MSAA_ALPHATEST_LOW)
  sample_Textures(D, H, I.tc1, I.tc0, I.af, float2(0.f,0.f));

#if	!( defined(MSAA_ALPHATEST_HIGH))
	clip		(D.w-(96.f/255.f))  ;
#endif	//	!( defined(MSAA_ALPHATEST_HIGH) )
#ifdef 	MSAA_ALPHATEST_HIGH
	float alpha = (D.w-(96.f/255.f)*0.5f)/(1.f-(96.f/255.f)*0.5f);
	uint mask = alpha_to_coverage ( alpha, pos2d );
#endif	//	MSAA_ALPHATEST_HIGH

#else
  uint mask = 0x0;
  
  float2 pixeloffset = MSAAOffsets[0]*(1.f/16.f);
  
  sample_Textures(D, H, I.tc1, I.tc0, I.af, pixeloffset );

  if( D.w-(96.f/255.f) >= 0 ) mask |= 0x1;
  
  [unroll] for( int i = 1; i < MSAA_SAMPLES; ++i )
  {
	float4 DI;
	float4 HI;
	pixeloffset = MSAAOffsets[i]*(1.f/16.f);
	sample_Textures(DI, HI, I.tc1, I.tc0, I.af, pixeloffset );
	if( DI.w-(96.f/255.f) >= 0 ) mask |= ( uint(0x1) << i );
  }
	
  if( mask == 0x0 )
	discard;
      	
#endif
  
  float3 N 	= normalize	(H.xyz);

//  D.xyz = float3(1,0,0);

  // 2. Standart output
#ifndef EXTEND_F_DEFFER_INLINE
  O				= pack_gbuffer(
								float4	(N					, H.w		),
								float4 	(I.position + N*def_virtualh/2.f	, 0.f		),
								float4	(D.x,	D.y,	D.z			, def_gloss	) );		// OUT: rgb.gloss
#else
  O				= pack_gbuffer(
								float4	(N					, H.w		),
								float4 	(I.position + N*def_virtualh/2.f	, 0.f		),
								float4	(D.x,	D.y,	D.z			, def_gloss	),
								mask );		// OUT: rgb.gloss
#endif

  return O;
}

#endif	//	INLINE_ATOC