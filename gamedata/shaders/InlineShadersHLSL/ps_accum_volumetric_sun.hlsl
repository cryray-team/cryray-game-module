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

#define RAY_PATH 2.0f
#define JITTER_TEXTURE_SIZE	64.0f

#if SUN_SHAFTS_QUALITY == 1
	#define RAY_SAMPLES	15
#elif SUN_SHAFTS_QUALITY == 2
	#define RAY_SAMPLES	25
#elif SUN_SHAFTS_QUALITY == 3
	#define RAY_SAMPLES	30
#endif

#include "Headers\shadow.h"

#ifdef INLINE_MSAA_OPTIMIZATION
float4 	main (v2p_volume  I, uint iSample : SV_SAMPLEINDEX  ) : SV_Target
#else
float4 	main (v2p_volume  I) : SV_Target
#endif
{
	float2	tc  = I.tc.xy/I.tc.w;
	float4	pos2d = I.hpos;
	
	gbuffer_data gbd = gbuffer_load_data( GLD_P(tc.xy, pos2d.xy, ISAMPLE) );
	float3	P = gbd.P;

	// Variable ray length, variable step dencity, use jittering
	float4	J0 	= jitter0.Sample( smp_jitter, tc*screen_res.x*1.0/JITTER_TEXTURE_SIZE );
	float	coeff = (RAY_SAMPLES - 1*J0.x)/(RAY_SAMPLES*RAY_SAMPLES);		
	float3	direction = P*coeff;

	float	depth = P.z;
	float	deltaDepth = direction.z;
	
	float4	current	= mul (m_shadow,float4(P,1.f));
	float4	delta 	= mul (m_shadow, float4(direction,0.f));

	float	res = 0.f;
	float	max_density = sun_shafts_intensity.x;
	float	density = max_density/RAY_SAMPLES;

	// SUN VOLUMETRIC FIXES - SSS Update 14.7
	// - Added weapon fade
	// - Discard out of bound shadow map sampling
	// - Fade volumetric edge
	// https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders/

	// Volumetric offset when used with SSS extended shadows.
	current.z += 0.001f;

	// Fade Weapon
	density *= saturate(depth * 0.8f);

	if (depth < 0.0001f)
		res = max_density;

	[unroll (RAY_SAMPLES)]
	for ( int i=0; i<RAY_SAMPLES; ++i )
	{
		// Don't sample out of bounds
		if (current.x >= 0.f && current.x <= 1.f && current.y >= 0.f && current.y <= 1.f)
		{
			if (depth > 0.1f)
				res += density*sample_hw_pcf(current, float4(0.f,0.f,0.f,0.f));
		}

		depth -= deltaDepth;
		current -= delta;
	}

	float fSturation = -Ldynamic_dir.z;

	//	Normalize dot product to
	fSturation = 0.5f*fSturation+0.5f;
	//	Map saturation to 0.2..1
	fSturation = 0.80f*fSturation+0.20f;

	res *= fSturation;

	// Use fog near/far to fade the sun rays
	float fog = saturate(length(P) * fog_params.w + fog_params.x);

#ifdef USE_STRICT_GAMMA_CORRECTION
	res = SRGBToLinear(res);
	float3 SunColor = SRGBToLinear(Ldynamic_color.rgb);
#else
	float3 SunColor = Ldynamic_color.rgb;
#endif

	// Fade from "res" to "max_density" using squared fog to get a nice curve
	float3 result = lerp(SunColor * max_density * fSturation, SunColor * res, 1.f - fog * fog);

	return float4(result, 1);
}

