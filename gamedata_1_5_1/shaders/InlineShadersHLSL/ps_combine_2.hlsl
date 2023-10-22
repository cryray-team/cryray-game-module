//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"
#include "Headers\h_dof_weather.hlsl"
#include "Headers\h_fog.hlsl"
// Check Screen Space Shaders modules
#include "ScreenSpaceAddon\h_screenspace_fog.hlsl"
#include "ScreenSpaceAddon\h_screenspace_indirect_light.hlsl"

#include "Headers\h_tonemapping.hlsl"

Texture2D s_distort;

struct c2_out
{
	float4	Color : SV_Target;
};

c2_out main( v2p_aa_AA I )
{
	c2_out	res;
	res.Color = float4(0.f, 0.f, 0.f, 0.f);

	gbuffer_data gbd = gbuffer_load_data(I.Tex0, I.HPos.xy, 0 );
	
  	float depth = gbd.P.z;
	
#ifdef 	USE_DISTORT
	float4 	distort	= s_distort.Sample(smp_nofilter, I.Tex0);
	
	float2	offset	= (distort.xy-(127.f/255.f))*def_distort;  // fix newtral offset
	float2	center	= I.Tex0 + offset;
#else
	float2	center 	= I.Tex0;
#endif

    float3 img = s_image.Load(int3(center.xy * screen_res.xy, 0), 0).xyz;
	//img *= (float3)dof(I.Tex0.xy);
    float4 bloom = s_bloom.Sample(smp_rtlinear,center);
	
	img = blend_soft(img, bloom.xyz*bloom.w);
		
	if (tnmp_onoff == 1.f)		
		img = Uncharted2ToneMapping(img);
		
#ifdef 	USE_DISTORT
 	float3	blurred	= bloom.xyz*def_hdr.xxx	;
			img		= lerp	(img,blurred,distort.z);
#endif
	
	//-' Hozar_2002 fix
	//gbd.P.xyz = float3((center.xy * 2.f - 1.f) * pos_decompression_params.xy, 1.f) * (gbd.P.z < 0.001f ? 10.f : gbd.P.z);
	//img = lerp(img, fog_color, get_height_fog_sky_effect(gbd.P.xyz));
	//-'
	
	img = (float3)dof(I.Tex0.xy);

#ifdef INDIRECT_LIGHT	
	ssfx_il(I.Tex0, I.HPos, gbd.P, gbd.N, img, 0);
#endif

	float4 final = float4(img, 1.f);

	final.rgb = pp_nightvision_combine_2(img, center);
	
	res.Color = final;
	
	res.Color.rgb = pp_vibrance(res.Color.rgb, weather_contrast + 1.f);
	res.Color.rgb = img_corrections(res.Color.rgb);
	
	return res;
}
