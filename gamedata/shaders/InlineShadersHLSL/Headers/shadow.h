//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	SHADOW_H
#define SHADOW_H

#include "Headers\common.h"

Texture2D s_smap : register(ps,t0);		// 2D/cube shadowmap

SamplerComparisonState smp_smap;	//	Special comare sampler
sampler smp_jitter;

static const float KERNEL = 0.6f;

float modify_light( float light )
{
   return ( light > 0.7f ? 1.f : lerp( 0.f, 1.f, saturate( light / 0.7f ) ) ); 
}

//////////////////////////////////////////////////////////////////////////////////////////
// hardware + PCF
//////////////////////////////////////////////////////////////////////////////////////////
float sample_hw_pcf (float4 tc,float4 shift)
{
	static const float 	ts = KERNEL / float(SMAP_size);

	tc.xyz 	/= tc.w;
	tc.xy 	+= shift.xy * ts;

	return s_smap.SampleCmpLevelZero( smp_smap, tc.xy, tc.z).x;
}

float shadow_hw( float4 tc )
{
  	float	s0		= sample_hw_pcf( tc, float4( -1.f, -1.f, 0.f, 0.f) );
  	float	s1		= sample_hw_pcf( tc, float4( +1.f, -1.f, 0.f, 0.f) );
  	float	s2		= sample_hw_pcf( tc, float4( -1.f, +1.f, 0.f, 0.f) );
  	float	s3		= sample_hw_pcf( tc, float4( +1.f, +1.f, 0.f, 0.f) );

	return	(s0+s1+s2+s3)/4.f;
}

//PCSS shadows
static const float2 poissonDisk[32] = {
	float2(0.0617981f, 0.07294159f),
	float2(0.6470215f, 0.7474022f),
	float2(-0.5987766f, -0.7512833f),
	float2(-0.693034f, 0.6913887f),
	float2(0.6987045f, -0.6843052f),
	float2(-0.9402866f, 0.04474335f),
	float2(0.8934509f, 0.07369385f),
	float2(0.1592735f, -0.9686295f),
	float2(-0.05664673f, 0.995282f),
	float2(-0.1203411f, -0.1301079f),
	float2(0.1741608f, -0.1682285f),
	float2(-0.09369049f, 0.3196758f),
	float2(0.185363f, 0.3213367f),
	float2(-0.1493771f, -0.3147511f),
	float2(0.4452095f, 0.2580113f),
	float2(-0.1080467f, -0.5329178f),
	float2(0.1604507f, 0.5460774f),
	float2(-0.4037193f, -0.2611179f),
	float2(0.5947998f, -0.2146744f),
	float2(0.3276062f, 0.9244621f),
	float2(-0.6518704f, -0.2503952f),
	float2(-0.3580975f, 0.2806469f),
	float2(0.8587891f, 0.4838005f),
	float2(-0.1596546f, -0.8791054f),
	float2(-0.3096867f, 0.5588146f),
	float2(-0.5128918f, 0.1448544f),
	float2(0.8581337f, -0.424046f),
	float2(0.1562584f, -0.5610626f),
	float2(-0.7647934f, 0.2709858f),
	float2(-0.3090832f, 0.9020988f),
	float2(0.3935608f, 0.4609676f),
	float2(0.3929337f, -0.5010948f),
};

#if SUN_QUALITY == 0
	#define PCSS_NUM_SAMPLES uint(12)
#elif SUN_QUALITY == 1
	#define PCSS_NUM_SAMPLES uint(20)
#elif SUN_QUALITY == 2
	#define PCSS_NUM_SAMPLES uint(32)
#endif

float shadow_pcss( float4 tc )
{
	// - Small modification to fix flickering and black squares.
	// - Added a extra performance option with lower SUN_QUALITY settings.
	// - Extended the blocker search from 3x3 to 4x4 for better results.
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/

	tc.xyz /= tc.w;

#if SUN_QUALITY == 2 // Blocker search ( Penumbra ) and filter
	int3 uv = int3(tc.xy * float(SMAP_size), 0.f);
	float zBlock = tc.z - 0.0001f;
	float avgBlockerDepth = 0.f;
	float blockerCount = 0.f;

	[unroll] 
	for( int row = -PCSS_PIXEL; row <= PCSS_PIXEL; row += PCSS_STEP )
	{
		[unroll] 
		for( int col = -PCSS_PIXEL; col <= PCSS_PIXEL; col += PCSS_STEP )
		{
			float shadowMapDepth = s_smap.Load( uv, int2( col, row ) ).x;
			float b1 = ( shadowMapDepth < zBlock ) ? 1.f : 0.f;
			blockerCount += b1;
			avgBlockerDepth += shadowMapDepth * b1;
		}
	}

	if (blockerCount < 1.f)
		return 1.f;

	avgBlockerDepth /= blockerCount;
	float fRatio = saturate( ( ( tc.z - avgBlockerDepth ) * PCSS_SUN_WIDTH ) / avgBlockerDepth );
	fRatio *= fRatio;
	fRatio = max(PCSS_PIXEL_MIN, fRatio * float(PCSS_PIXEL)) / float(SMAP_size);

	float s = 0.f;
	[unroll] 
	for( uint i = 0; i < PCSS_NUM_SAMPLES; ++i )
	{
		float2 offset = poissonDisk[i] * fRatio;
		s += s_smap.SampleCmpLevelZero( smp_smap, tc.xy + offset, tc.z ).x;
	}
	
	return s / PCSS_NUM_SAMPLES;
#else // No blocker search ( Penumbra ), just filter
	float fRatio = max(PCSS_PIXEL_MIN, 0.5f * float(PCSS_PIXEL)) / float(SMAP_size);

	float s = 0.f;
	[unroll] 
	for( uint i = 0; i < PCSS_NUM_SAMPLES; ++i )
	{
		float2 offset = poissonDisk[i] * fRatio;
		s += s_smap.SampleCmpLevelZero( smp_smap, tc.xy + offset, tc.z ).x;
	}
	
	return s / PCSS_NUM_SAMPLES;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////
//	D24X8+PCF
//////////////////////////////////////////////////////////////////////////////////////////

float4 test (float4 tc, float2 offset)
{
	tc.xyz 	/= tc.w;
	tc.xy 	+= offset;
	return s_smap.SampleCmpLevelZero( smp_smap, tc.xy, tc.z).x;
}

float 	shadowtest_sun 	(float4 tc, float4 tcJ)			// jittered sampling
{
	float4	r;
	const 	float 	scale 	= (0.7f/float(SMAP_size));

	float2 	tc_J	= frac(tc.xy/tc.w*SMAP_size/4.f )*0.5f;
	float4	J0		= jitter0.Sample(smp_jitter,tc_J)*scale;

	const float k = 0.5f/float(SMAP_size);
	r.x 	= test 	(tc, J0.xy+float2(-k,-k)).x;
	r.y 	= test 	(tc, J0.wz+float2( k,-k)).y;
	r.z		= test	(tc,-J0.xy+float2(-k, k)).z;
	r.w		= test	(tc,-J0.wz+float2( k, k)).x;

	return	dot(r,1.f/4.f);
}

float shadow_hw_hq( float4 tc )
{
    return shadow_pcss(tc);
}

float shadow( float4 tc ) 
{
	return shadow_pcss(tc);
}

//////////////////////////////////////////////////////////////////////////////////////////
// testbed

float shadow_rain(float4 tc, float2 tcJ)			// jittered sampling
{
	float4	r;

	const 	float 	scale 	= (4.f/float(SMAP_size));

	float4	J0 	= jitter0.Sample( smp_linear, tcJ )*scale;
	float4	J1 	= jitter1.Sample( smp_linear, tcJ )*scale;

	r.x 	= test 	(tc,J0.xy).x;
	r.y 	= test 	(tc,J0.wz).y;
	r.z		= test	(tc,J1.xy).z;
	r.w		= test	(tc,J1.wz).x;
	return	dot(r,1.f/4.f);
}

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef  USE_SUNMASK
float sunmask( float4 P )
{
	float2 		tc	= mul( m_sunmask, float4(P.xyz, 1.f) ).xy;		//
	return 		s_lmap.SampleLevel( smp_linear, tc, 0 ).w;	//Hemi map - ambient occlusion	
}
#else
float sunmask( float4 P ) { return 1.f; }		// 
#endif
//////////////////////////////////////////////////////////////////////////////////////////

#endif