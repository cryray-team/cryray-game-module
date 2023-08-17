//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

/**
 * @ Version: SCREEN SPACE SHADERS - UPDATE 11.3
 * @ Description: SSS implementation
 * @ Modified time: 2022-09-27 09:10
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

#include "ScreenSpaceAddon\screenspace_common.h"

float SSFX_ScreenSpaceShadows(float4 P, float2 tc, uint iSample)
{
	// Light vector
	float3 L_dir = mul(m_V, float4(-normalize(L_sun_dir_w), 0.f)).xyz;

	// Material conditions...
	bool mat_flora = abs(P.w - MAT_FLORA) <= 0.02f;
	bool mat_terrain = abs(P.w - 0.95f) <= 0.02f;

	// Weapons mask. Depth below or equal to 1.2
	bool weapon_mask = P.z >= 1.2f;

	// Weapon Factor.
	float pLen = length(P.z);
	float wpn_f = smoothstep(G_SSDO_WEAPON_LENGTH * 0.75f, G_SSDO_WEAPON_LENGTH, pLen);

	//wpn_f = saturate(wpn_f + mat_terrain);
	
	// Adjust Settings for weapons and scene.
	float ray_hardness	= lerp(10.f * G_SSDO_WEAPON_HARDNESS, 15.f * G_SSDO_SCENARY_HARDNESS, wpn_f);
	float ray_detail	= clamp(pLen * 0.003f, G_SSS_DETAILS, 10.f);
	float ray_thick		= (0.3f - wpn_f * 0.23f) + (pLen * 0.001f * wpn_f);
	float ray_len		= lerp(G_SSDO_WEAPON_SHADOW_LENGTH, G_SSDO_SCENARY_SHADOW_LENGTH, wpn_f);

	// Grass overwrite. ( Sadly some weapons use MAT_FLORA, let's remove low depth values from the overwrite )
	if (mat_flora && weapon_mask)
	{
		ray_hardness = G_SSDO_GRASS_HARDNESS * 5.f;
		ray_len = G_SSDO_GRASS_SHADOW_LENGTH;
	}

	if (ray_hardness <= 0.f)
		return 1.f;

	RayTrace sss_ray = SSFX_ray_init(P.xyz, L_dir.xyz, ray_len, G_SSS_STEPS, SSFX_noise(tc));

	float shadow = 0.f;
	float2 fade_len = float2(1.f, 1.f) + sss_ray.r_step.xy;

	[unroll (G_SSS_STEPS)]
	for (int i = 0; i < G_SSS_STEPS; i++)
	{
		// Break the march if ray go out of screen...
		if (!SSFX_is_valid_uv(sss_ray.r_pos))
			break;

		// Sample current ray pos ( x = difference | y = sample depth | z = current ray len )
		float3 depth_ray = SSFX_ray_intersect(sss_ray, iSample);
		
		// Check depth difference
		float diff = depth_ray.x;
		
		// No Sky
		diff *= depth_ray.y > SKY_EPS;

		// Disable weapons at some point to avoid wrong shadows on the ground
		if (weapon_mask)
			diff *= depth_ray.y >= 1.f;

		// Negative: Ray is closer to the camera ( not occluded )
		// Positive: Ray is beyond the depth sample ( occluded )
		if (diff > ray_detail && diff < ray_thick)
		{
			shadow += (fade_len.x - depth_ray.z) * (1.f - smoothstep( G_SSS_STEPS * G_SSS_FORCE_FADE, G_SSS_STEPS + 1, i));
		}

		// Step the ray
		sss_ray.r_pos += sss_ray.r_step;
	}

	// Calc shadow and return.
	return 1.f - saturate(shadow * (1.f / G_SSS_STEPS) * ray_hardness) * G_SSS_INTENSITY;
}