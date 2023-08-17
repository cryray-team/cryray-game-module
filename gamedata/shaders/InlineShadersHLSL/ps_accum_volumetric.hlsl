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
#include "Headers\shadow.h"

#ifndef ISAMPLE
#define ISAMPLE 0
#endif

struct 	v2p
{
	float3 	lightToPos	: TEXCOORD0;	// light center to plane vector
	float3 	vPos		: TEXCOORD1;	// position in camera space
	float 	fDensity	: TEXCOORD2;	// plane density along Z axis
};

#define	USE_LMAP
#define	USE_LMAPXFORM
#define	USE_SHADOW

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
#ifndef INLINE_MSAA_OPTIMIZATION
float4 main ( v2p I ) : SV_Target
#else
float4 main ( v2p I, uint iSample : SV_SAMPLEINDEX ) : SV_Target
#endif
{
        // ----- shadow
		float4  P4 = float4(I.vPos.x, I.vPos.y, I.vPos.z, 1.f);
        float4	PS	= mul(m_shadow, P4);
		
        float	s	= 1.f;
        #ifdef	USE_SHADOW
                  	s	= shadow_hw(PS);
        #endif

        // ----- lightmap
        float4	lightmap = float4(1.f, 1.f, 1.f, 1.f);
        #ifdef	USE_LMAP
                #ifdef	USE_LMAPXFORM
					PS.x	= dot( P4, m_lmap[0]);
					PS.y	= dot( P4, m_lmap[1]);
                #endif
					lightmap = s_lmap.Sample(smp_rtlinear, PS.xy/PS.w); 
        #endif

	// ----- attenuate
	float rsqr	= dot( I.lightToPos, I.lightToPos); // distance 2 light (squared)
  	float  att 	= saturate( 1.f - rsqr * Ldynamic_pos.w ); // q-linear attenuate

	// ----- noise
	PS.xy /= PS.w;
	float time = timers.z*0.1f;	
	PS.xy /= 3.f;
	PS.x += time;
	//	TODO: DX10: Can use sampler with point mip filter
	float4	t_noise	= s_noise.Sample( smp_linear, PS.xy );
	PS.x -= time;
	PS.y -= time*0.70091f;
	//	TODO: DX10: Can use sampler with point mip filter	
	t_noise *= s_noise.Sample( smp_linear, PS.xy );
	t_noise = t_noise*0.5f+0.5f;

	// out
	float maxIntens = I.fDensity;
	float3	result = maxIntens * s * att;
	result *= lightmap.xyz;
	result *= Ldynamic_color.rgb * t_noise.xyz;

	return  float4( result, 0.f);
}