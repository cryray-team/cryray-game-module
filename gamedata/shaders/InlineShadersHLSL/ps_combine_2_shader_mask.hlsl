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
	
	float3 final_image = image;
	
	//Mix gasmask cracks with image
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
