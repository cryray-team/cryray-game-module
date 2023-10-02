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
 * @ Version: SCREEN SPACE SHADERS - UPDATE 14.6
 * @ Description: SSDO implementation
 * @ Modified time: 2023-02-07 00:11
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

float calc_ssdo(float4 P, float3 N, float2 tc, float4 pos2d/*, uint iSample*/)
{	
	// Rendering AO till G_SSDO_RENDER_DIST
	if (P.z >= G_SSDO_RENDER_DIST || P.z <= SKY_EPS)
		return 1.f;
	
	// Ao Noise
	float ao_noise = frac(sin(dot(tc, float2(12.f, 78.f))) * 43758.f) * clamp(P.z * 0.05f, G_SSDO_NOISE_MIN, G_SSDO_NOISE_MAX);

	// MAT_FLORA from current pixel?
	bool mat_flora = abs(P.w - MAT_FLORA) <= 0.04f;

	// If MAT_FLORA we use a fake normal to align the hemisphere ( We can't trust much about flora normal )
	N.xyz = mat_flora ? float3(0.f, 0.25f, 0.f) : N.xyz;

	// A factor to adjust some values to only affect weapons ( closer objects )
	float WeaponFactor = 1.f - smoothstep(G_SSDO_WEAPON_LENGTH * 0.5f, G_SSDO_WEAPON_LENGTH, length(P.xyz));
	
	// Var to accumulate the AO
	float occ = 0.f;

	// AO Radius
	float radius = G_SSDO_RADIUS;
	
	// Radius weapon adjustements
	radius *= lerp(1.f, G_SSDO_WEAPON_RADIUS * clamp(P.z * 2.f, 0.2f, 1.f), WeaponFactor);
	
//#ifdef G_SSDO_DETAILED_RADIUS
	float detail_radius = radius * lerp(G_SSDO_DETAILED_RADIUS, G_SSDO_DETAILED_WEAPON_RADIUS, WeaponFactor);
//#endif

	[unroll (G_SSDO_SAMPLE)]
	for (int i = 0; i < G_SSDO_SAMPLE; i++)
	{
		// Use surface normal and add the hemisphere distribution
		float3 sample_rays = (ssfx_hemisphere[i] + N.xyz) * (1.f + ao_noise);

		// Sample position
		float3 occ_pos = P.xyz + sample_rays * radius;
		
		// Sample position to UV
		float2 occ_pos_uv = SSFX_view_to_uv(occ_pos);
		
		// Process valid UV coor
		if (SSFX_is_valid_uv(occ_pos_uv))
		{
			// Sample depth buffer
			float4 sample_pos = SSFX_get_position(occ_pos_uv, /*iSample*/0);
		
			// AO Distance attenuation. To discard incorrect AO and add some attenuation through distance.
			float atte = lerp( 0.1f, 4.f, smoothstep(G_SSDO_WEAPON_LENGTH * 0.5f, G_SSDO_WEAPON_LENGTH, sample_pos.z));

			// Pixel occluded?
			float AO = step(sample_pos.z, occ_pos.z);

			// Detailed Search if there's no AO
//#ifdef G_SSDO_DETAILED_SEARCH
			if (AO < 1)
			{
				// Same has before but with smaller radius
				occ_pos = P.xyz + sample_rays * detail_radius;
				occ_pos_uv = SSFX_view_to_uv(occ_pos); // We don't check for a valid UV coor, is already done and this sample radius is smaller ( in theory )
				sample_pos = SSFX_get_position(occ_pos_uv, /*iSample*/0); // Get Sample
				AO = step(sample_pos.z, occ_pos.z); // Check occlusion
				atte *= saturate(detail_radius + WeaponFactor); // Adjust attenuation for the scenary AO
			}
//#endif

			// Distance between pixel and "occluder"
			float3 Dist = distance(float3(occ_pos_uv.x, occ_pos_uv.y, sample_pos.z), float3(tc.x, tc.y, P.z));

			// Difference attenuation
			AO *= 1.f - smoothstep(0.f, atte, Dist );

			// MAT_FLORA from sample?
			bool sample_mat_flora = abs(gbuf_unpack_mtl(sample_pos.w) - MAT_FLORA) <= 0.04f || mat_flora;
			
			// Value for dot product for full intensity ( Adjust value for WeaponFactor, we want a extremely sensitive AO for weapons )
			float N_a = saturate( WeaponFactor ) * 0.8f;

			// Dot product using current normal and sampled normal to adjust intensity ( Change mode if MAT_FLORA )
			if (!sample_mat_flora)
				AO *= smoothstep(1.f, N_a, dot(gbuf_unpack_normal( sample_pos.xy ), N ));
			else
				AO *= G_SSDO_FLORA_INTENSITY;

			// Discard incorrect occlusion from the sky.
			AO *= !is_sky(sample_pos.z);

			// Accumulate final AO
			occ += AO;
		}
		else
		{
			// Try next step with less radius
			radius *= 0.75f;
		}
	}
	
	// Normalize AO
	occ /= G_SSDO_SAMPLE;
	
	// Invert AO
	occ = 1.f - occ;
	
	// AO intensity 
	occ = pow(occ, G_SSDO_INTENSITY * (1.f - (1.f - G_SSDO_WEAPON_INTENSITY) * WeaponFactor));

	// AO softer
	occ = lerp(occ, sqrt(occ), G_SSDO_SMOOTH);

	// Limit AO maximum occlusion
	occ = clamp(occ, G_SSDO_MAX_OCCLUSION, 1.f);

	// Fadeout using G_SSDO_RENDER_DIST
	occ += smoothstep(G_SSDO_RENDER_DIST * 0.8f, G_SSDO_RENDER_DIST, P.z);

	// Return AO
	return saturate(occ);
}
