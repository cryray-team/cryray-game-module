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

struct 	p_particle
{
	float4 	color	: COLOR0;
	p_flat	base;	
};

#ifdef	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_particle II, float4 pos2d : SV_Position )
#else	//	MSAA_ALPHATEST_HIGH
f_deffer 	main	( p_particle II )
#endif	//	MSAA_ALPHATEST_HIGH
{
  f_deffer	O;
  p_flat	I;	I=II.base;

  // 1. Base texture + kill pixels with low alpha
//  float4 	D 	= 	tex2D		(s_base, I.tcdh);
#if defined(MSAA_ALPHATEST_LOW)
  uint mask = 0x0;
  float2 ddx_base = ddx( I.tcdh );
  float2 ddy_base = ddy( I.tcdh );

  float2 pixeloffset = MSAAOffsets[0]*(1.f/16.f);
  
  float2 texeloffset =  pixeloffset.x * ddx_base + pixeloffset.y * ddy_base;

  float4 	D 	= 	s_base.Sample( smp_base, I.tcdh + texeloffset );
			D	*=	II.color;

  if( D.w-def_aref >= 0 ) mask |= 0x1;

  [unroll] for( int i = 1; i < MSAA_SAMPLES; ++i )
  {
	pixeloffset = MSAAOffsets[i]*(1.f/16.f);
	texeloffset =  pixeloffset.x * ddx_base + pixeloffset.y * ddy_base;
	float4 DI 	= s_base.Sample( smp_base, I.tcdh+ texeloffset );
	DI *=	II.color;
	if( DI.w-def_aref >= 0 ) mask |= ( uint(0x1) << i );
  }
	
  if( mask == 0x0 )
	discard;
#else
  float4 	D 	= 	s_base.Sample( smp_base, I.tcdh);
			D	*=	II.color;

#ifdef 	MSAA_ALPHATEST_HIGH
	float alpha = (D.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	uint mask = alpha_to_coverage ( alpha, pos2d );
#else	//	MSAA_ALPHATEST_HIGH
	clip	(D.w-def_aref);
#endif	//	MSAA_ALPHATEST_HIGH
			
#endif

  // 2. Standart output
  float4		Ne  = float4	(normalize((float3)I.N.xyz)					, I.position.w	);
#ifndef EXTEND_F_DEFFER_INLINE
  O				= pack_gbuffer( 
								Ne,
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f	, xmaterial		),
								float4	(D.xyz,			def_gloss) );		// OUT: rgb.gloss
#else
  O				= pack_gbuffer( 
								Ne,
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f	, xmaterial		),
								float4	(D.xyz,			def_gloss),
								mask );		// OUT: rgb.gloss
#endif								
								
  return O;
}
