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
#include "Headers\shadow.h"

#ifndef ISAMPLE
#define ISAMPLE 0
#endif

float3 GetNVNMap(Texture3D s_texture, float2 tc, float time)
{
	//	Unpack NVidia normal map
	float4 water = s_texture.SampleBias( smp_base, float3(tc, time), -3.f) - 0.5f;

	//	Swizzle
	water.xyz = water.wyz;

	//	Renormalize (*2) and scale (*3)
	water.xyz *= 6.f;

	water.y = 0.f;

	return water.xyz;
}

float3 GetWaterNMap( Texture2D s_texture, float2 tc)
{
	//	Unpack normal map
	float4 water = s_texture.Sample( smp_base, tc);
	water.xyz = (water.xzy-0.5f)*2.f;

	water.xyz *= 0.3f;

	water.y = 0.f;

	return water.xyz * 1.2f;
}

#ifdef INLINE_MSAA_OPTIMIZATION
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4	Color	: COLOR, float4 pos2d : SV_Position, uint iSample : SV_SAMPLEINDEX  ) : SV_Target
#else
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4	Color	: COLOR, float4 pos2d : SV_Position ) : SV_Target
#endif
{
#ifdef INLINE_MSAA_OPTIMIZATION
	gbuffer_data gbd = gbuffer_load_data( tc, pos2d, iSample );
#else
	gbuffer_data gbd = gbuffer_load_data( tc, pos2d, ISAMPLE );
#endif

	float4 _P = float4(gbd.P, 1.f);
	float3 _N = gbd.N;
	float3 D = gbd.C; // rgb	//.gloss

	_P.xyz += _N.xyz * 0.15f;
	_N.xyz = normalize(_N.xyz);

	float4 PS = mul(m_shadow, _P);
	float3 WorldP = mul(m_sunmask, _P);
	float3 WorldN = mul(m_sunmask, _N.xyz);
	float shadow_map = shadow_rain(PS, WorldP.xz - WorldN.xz); //-' OldSerpskiStalker fix rain map + engine x2560 r_rain limit
	
	// Read rain projection with some jetter. Also adding pixel normal
	// factor to jitter to make rain strips more realistic.
	float s = smoothstep(0.f, 1.f, saturate(gbd.hemi * 10.f));
	
	//	Apply distance falloff
	float	fAtten = 1.f - smoothstep( 5.f, 250.f, length( _P.xyz ));
	s	*= fAtten*fAtten;
	
	s *= saturate(RainDensity.x);

	float fIsUp = -dot(Ldynamic_dir.xyz, _N.xyz);
	s *= saturate(fIsUp * 10.f + (10.f * 0.5f) + 0.5f);
	fIsUp = max(0, fIsUp);

	float fIsX = WorldN.x;
	float fIsZ = WorldN.z;

	float3 waterSplash = GetNVNMap(s_water3D, WorldP.xz, timers.x * 3.f) * shadow_map;

	float3 tc1 = WorldP / 2.f;

	float fAngleFactor = 1.f- fIsUp;

	fAngleFactor = 0.1f * ceil(10.f * fAngleFactor);

	//	Just slow down effect.
	fAngleFactor *= 0.3f;

	float3 waterFallX = GetWaterNMap(s_waterFall, float2(tc1.z, tc1.y + timers.x * fAngleFactor));
	float3 waterFallZ = GetWaterNMap(s_waterFall, float2(tc1.x, tc1.y + timers.x * fAngleFactor));
	
	float WeaponAttenuation = /*There was a limiter for blocking wetness for weapons in their hands*/
							  1.f * shadow_map;

	float ApplyNormalCoeff = s * WeaponAttenuation; // * gbd.refl; this mul will kill waterfall and splashes

	float3 water = waterSplash * (fIsUp * ApplyNormalCoeff);

	water += waterFallX.yxz * (abs(fIsX) * ApplyNormalCoeff) * shadow_map;
	water += waterFallZ.zxy * (abs(fIsZ) * ApplyNormalCoeff) * shadow_map;

	//	Translate NM to view space
	water.xyz = mul(m_V, water.xyz);

	_N += water.xyz;

	_N = normalize(_N);

	s *= dot(D.xyz, 0.33f);

	return float4(_N, s);
}
