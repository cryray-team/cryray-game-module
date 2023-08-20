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
#include "Headers\dof_weather.h"
#include "Headers\fog.h"
// Check Screen Space Shaders modules
#include "ScreenSpaceAddon\screenspace_fog.h"
#include "ScreenSpaceAddon\screenspace_indirect_light.h"

#include "Headers\tonemapping.h"

#ifndef MSAA_ANTIALIASING_ENABLE
	Texture2D	s_distort;
#else
	Texture2DMS<float4>	s_distort;
#endif

struct c2_out
{
	float4	Color : SV_Target;
#ifdef MSAA_ANTIALIASING_ENABLE
	float	Depth : SV_Depth;
#endif
};

c2_out main( v2p_aa_AA I )
{
	c2_out	res;
	res.Color = float4(1.f, 1.f, 1.f, 1.f);

	gbuffer_data gbd = gbuffer_load_data(I.Tex0.xy, I.HPos.xy, 0 );
	
  	float depth = gbd.P.z;
	
#ifdef 	USE_DISTORT
#ifndef MSAA_ANTIALIASING_ENABLE
	float4 	distort	= s_distort.Sample(smp_nofilter, I.Tex0.xy);
#else // MSAA_ANTIALIASING_ENABLE
	float4 	distort	= s_distort.Load( int3( I.Tex0.xy * screen_res.xy, 0 ), 0 );
#endif // MSAA_ANTIALIASING_ENABLE
	float2	offset	= (distort.xy-(127.f/255.f))*def_distort;  // fix newtral offset
	float2	center	= I.Tex0.xy + offset.xy;
#else // USE_DISTORT
	float2	center 	= I.Tex0.xy;
#endif

    float3 img = s_image.Load(int3(center.xy * screen_res.xy, 0),0).xyz;
    float4 bloom = s_bloom.Sample(smp_rtlinear,center.xy);
	
	img = blend_soft(img, bloom.xyz*bloom.w);
		
	if (tnmp_onoff == 1.f)		
		img = Uncharted2ToneMapping(img);
		
#ifdef 	USE_DISTORT
 	float3	blurred	= bloom.xyz*def_hdr	;
			img		= lerp	(img,blurred,distort.z);
#endif
	
	//-' Hozar_2002 fix
	//gbd.P.xyz = float3((center.xy * 2.f - 1.f) * pos_decompression_params.xy, 1.f) * (gbd.P.z < 0.001f ? 10.f : gbd.P.z);
	//img = lerp(img, fog_color, get_height_fog_sky_effect(gbd.P.xyz));
	//-'
	
	img = dof(I.Tex0.xy).xyz;

#ifdef INDIRECT_LIGHT	
	ssfx_il(I.Tex0, I.HPos, gbd.P, gbd.N, img, 0);
#endif

	float4 final = float4(img, 1.f);

	final.xyz = pp_nightvision_combine_2(img.xyz, center.xy);
	
	res.Color = final;
	
#ifdef MSAA_ANTIALIASING_ENABLE
	float4 ptp = mul(m_P, float4(gbd.P, 1.f));
	res.Depth = ptp.w == 0.f ? 1.f : ptp.z / ptp.w;
#endif
	
	res.Color.xyz = pp_vibrance(res.Color.xyz, weather_contrast + 1.f);
	res.Color.xyz = img_corrections(res.Color.xyz);
	
	return res;
}
