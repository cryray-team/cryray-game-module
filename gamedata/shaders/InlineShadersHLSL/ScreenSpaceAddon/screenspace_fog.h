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
 * @ Version: SCREEN SPACE SHADERS - UPDATE 14.4
 * @ Description: 2 Layers fog ( Distance + Height )
 * @ Modified time: 2023-01-30 09:14
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

float SSFX_HEIGHT_FOG(float3 P, float World_Py, inout float3 color)
{
	// Get Sun dir
	float3 Sun = saturate(dot(normalize(Ldynamic_dir.xyz), -normalize(P.xyz)));

	// Apply sun color
	Sun = lerp(fog_color.rgb, Ldynamic_color.rgb, Sun);

	// Distance Fog ( Default Anomaly Fog )
	float fog = saturate(length(P) * fog_params.w + fog_params.x);

	// Height Fog
	float fogheight = smoothstep(G_FOG_HEIGHT, -G_FOG_HEIGHT, World_Py) * G_FOG_HEIGHT_INTENSITY;

	// Add the height fog to the distance fog
	float fogresult = saturate(fog + fogheight * (fog * G_FOG_HEIGHT_DENSITY));

	// Blend factor to mix sun color and fog color. Adjust intensity to.
	float FogBlend = fogheight * G_FOG_SUNCOLOR_INTENSITY;

	// Final fog color
	float3 FOG_COLOR = lerp(fog_color.rgb, Sun, FogBlend);

	// Apply fog to color
	color = lerp(color, FOG_COLOR, fogresult);

	// Return distance fog.
	return fog;
}

float SSFX_FOGGING(float Fog, float World_Py)
{
	// Height fog
	float fog_height = smoothstep(G_FOG_HEIGHT, -G_FOG_HEIGHT, World_Py) * G_FOG_HEIGHT_INTENSITY;

	// Add height fog to default distance fog.
	float fog_extra = saturate(Fog + fog_height * (Fog * G_FOG_HEIGHT_DENSITY));
	
	return 1.f - fog_extra;
}