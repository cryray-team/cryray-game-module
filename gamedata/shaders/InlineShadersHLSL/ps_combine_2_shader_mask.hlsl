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
#include "Headers\img_corrections.h"
#include "Headers\dof_weather.h"

float visor_ratio(float s)
{
    float ratio = screen_res.y / screen_res.x; // 0.75 for 4:3 (normalscreen) and 0.625 for 16:10 (widescreen) resolution
    return (s - 0.5f) * ratio + 0.5f;
}

float3 visor_reflection(float3 image, float2 tc)
{
    float4 final = (0.f, 0.f, 0.f, 0.f);
    float2 center = float2(0.5f, 0.5f);

    float x = length(float2(tc.x, visor_ratio(tc.y)) - center);

    if (x < GM_VIS_RADIUS)
        return image;
    else
    {
        float p = saturate((x / GM_VIS_RADIUS - 1.f) * 2.f);

        for (int i = 0; i < GM_VIS_NUM; i++)
        {
            float N = 1.f - p / 0.6f + 0.4f * (i / (GM_VIS_NUM - 1.f)) * p;
            float2 m = (center - tc) * -N + center;
            final += s_image.SampleLevel(smp_rtlinear, m, 0);
        }
        final /= GM_VIS_NUM;
		
		float3 reflected = (image + GM_VIS_INTENSITY * final) / (1.f + GM_VIS_INTENSITY);	
		return reflected;		
	}
} 

float4 main(p_screen I) : SV_Target
{
	//Sample gasmask texture
	float4 gasmask_tex;
	
	if (mask_control.x == 1)
	{	
		gasmask_tex = s_mask_nm_1.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 2)
	{
		gasmask_tex = s_mask_nm_2.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 3)	
	{
		gasmask_tex = s_mask_nm_3.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 4)	
	{
		gasmask_tex = s_mask_nm_4.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 5)	
	{
		gasmask_tex = s_mask_nm_5.Sample(smp_nofilter,I.tc0);
	}	
	else if (mask_control.x == 6)	
	{
		gasmask_tex = s_mask_nm_6.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 7)	
	{
		gasmask_tex = s_mask_nm_7.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 8)	
	{
		gasmask_tex = s_mask_nm_8.Sample(smp_nofilter,I.tc0);
	}	
	else if (mask_control.x == 9)	
	{
		gasmask_tex = s_mask_nm_9.Sample(smp_nofilter,I.tc0);
	}
	else if (mask_control.x == 10)	
	{
		gasmask_tex = s_mask_nm_10.Sample(smp_nofilter,I.tc0);
	}

	//Prepare refracted texcoord
    float2 refr_tc = I.tc0.xy + (gasmask_tex.xy * 2.f - 1.f) * GM_DIST_INT;
	
	//Sample scene with refracted texcoord
	float3 image = s_image.Sample(smp_nofilter, refr_tc.xy).xyz;
	
	float3 original_image = image;
	float3 image_with_nightvision = pp_nightvision(original_image, refr_tc.xy);
	float4 image_with_dof = dof(I.tc0.xy).xyzz;
	float3 vibrance_effect = pp_vibrance(s_image.Sample(smp_nofilter, refr_tc.xy), weather_contrast + 1.f);
	
	// Combine the effects with proper blending factors
	//-' OldSerpskiStalker7777
    float nightvision_weight = 0.5f; // Adjust these weights as needed
    float dof_weight = 0.5f;
    float vibrance_weight = 0.2f;
	
	float3 final_image = lerp(original_image, image_with_nightvision, nightvision_weight);
    final_image = lerp(final_image, image_with_dof, dof_weight);
    final_image += vibrance_weight * vibrance_effect;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//-' TODO: Rework for IWP Mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#ifndef ANOMALY_MODE	
	//if ((is_helmet) || (is_outfit))
	//{
	//	if (!nightvision_enable)
	//	{
			//-' Gamma
	//		image += img_corrections(s_image.Load(int3(refr_tc.xy * screen_res.xy, 0),0));
			//-' Общая гамма, для погоды
	//		image = pp_vibrance(s_image.Sample(smp_nofilter, refr_tc.xy), weather_contrast + 1.f);
	//	}
	//	else
			//-' OldSerpskiStalker
	//		image = pp_nightvision(s_image.Sample(smp_nofilter, refr_tc.xy).xyz, refr_tc.xy);
	//}
#else
	//image = pp_nightvision(s_image.Sample(smp_nofilter, refr_tc.xy).xyz, refr_tc.xy);
	//image = dof(I.tc0.xy).xyzz;
	//image += pp_vibrance(s_image.Sample(smp_nofilter, refr_tc.xy), weather_contrast + 1.f);
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	final_image += (img_corrections(s_image.Load(int3(refr_tc.xy * screen_res.xy, 0),0)));
	
	//Mix gasmask cracks with image
	//image += (gasmask_tex.w * image) * GM_DIFF_INT;
	final_image += gasmask_tex.w * GM_DIFF_INT * final_image;
	
	//Add glass reflection on top
	if (mask_control.z == 1.f)
	{	
		//Prepare aspect-ratio correct TC for attenuation
		float2 vig_tc = I.tc0.xy;
	    vig_tc -= 0.5f;
		vig_tc.x *= screen_res.x / screen_res.y;
		vig_tc += 0.5f;	
		
		//Get refl attenuation
		float refl_att = smoothstep(0.8f, 1.f, distance(vig_tc, float2(0.5f, 0.5f)));
		
		final_image = lerp(final_image, visor_reflection(final_image, refr_tc.xy), refl_att);
	}
	
	//Output
	return float4(final_image, 1.f);
} 
