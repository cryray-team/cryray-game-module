/**
 * @ Version: SCREEN SPACE SHADERS - UPDATE 14.4
 * @ Description: 2 Layers fog ( Distance + Height )
 * @ Modified time: 2023-01-30 09:14
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

#include "screenspace_addon\settings_screenspace_FOG.h"

#include "shadow.h"

/*
float SSFX_HEIGHT_FOG(float3 P, float World_Py, inout float3 color)
{
	// Get Sun dir
	float3 Sun = saturate(dot(normalize(Ldynamic_dir), -normalize(P)));

	// Apply sun color
	Sun = lerp(fog_color, Ldynamic_color.rgb, Sun);

	// Distance Fog ( Default Anomaly Fog )
	float fog = saturate(length(P) * fog_params.w + fog_params.x);

	// Height Fog
	float fogheight = smoothstep(G_FOG_HEIGHT, -G_FOG_HEIGHT, World_Py) * G_FOG_HEIGHT_INTENSITY;

	// Add the height fog to the distance fog
	float fogresult = saturate(fog + fogheight * (fog * G_FOG_HEIGHT_DENSITY));

	// Blend factor to mix sun color and fog color. Adjust intensity to.
	float FogBlend = fogheight * G_FOG_SUNCOLOR_INTENSITY;

	// Final fog color
	float3 FOG_COLOR = lerp(fog_color, Sun, FogBlend);

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
	
	return 1.0f - fog_extra;
}
*/

#define STEP_COUNT 30

float4 level_bound_min;
float4 level_bound_max;

// hash function              
float hash(float n)
{
    return frac(cos(n) * 114514.1919);
}

#define __height 8
//#define __density 8

float3 get_bound_dist(float3 pos, float3 dir) {
	float3 box_bound_min = level_bound_min.xyz;
	float3 box_bound_max = level_bound_max.xyz;
	box_bound_max.y = __height;
	
	float3 max_plane_in = (box_bound_max - pos) / dir;
	float3 min_plane_in = (box_bound_min - pos) / dir;
	float3 far_plane_in = max(max_plane_in, min_plane_in);
	
	return min(min(far_plane_in.x, far_plane_in.y), far_plane_in.z);
}

// 3d noise function
float hash3Df (in float3 x)
{
    float3 q = frac(x);
    float3 p = x - q;
    float3 f = smoothstep(0.0, 1.0, q);
        
    float n = p.x + p.y * 10.0 + p.z * 100.0;
    
    return lerp(
        lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
            lerp(hash(n + 10.0), hash(n + 11.0), f.x), f.y),
        lerp(lerp(hash(n + 100.0), hash(n + 101.0), f.x),
            lerp(hash(n + 110.0), hash(n + 111.0), f.x), f.y), f.z);
}

float GetBorderAtten(float3 tc, float att = 0.005f)
{
    float3 borderDist = 1.0f - abs(tc * 2.0f - 1.0f);
    float3 borderAtten = saturate(borderDist / att);
    return borderAtten.x * borderAtten.y * borderAtten.z;
}

float Raymarch(float3 P) {
	float fogheight = 0.0;
	
	//float4 P4 = float4(P.x, P.y, P.z, 1.0f);
    //float4 PS      = mul( m_shadow, P4 );

	//float fog = 0.f;//saturate(length(P) * fog_params.w + fog_params.x);
	float3 box_bound_min = level_bound_min.xyz;
	float3 box_bound_max = level_bound_max.xyz;
	box_bound_max.y = __height;//g_fog.x + 0.001f;

	float3 rayPos = eye_position;
	float3 endPos = mul(/*m_v2w*/m_inv_V, float4(P, 1.0)).xyz;
	float3 deltaPos = endPos - rayPos;
	float deltaLen = length(deltaPos);
	deltaPos /= max(0.0001f, deltaLen);
	
	float max_dist = get_bound_dist(rayPos, deltaPos);
	deltaLen = min(deltaLen, max_dist);
	endPos = deltaLen * deltaPos + rayPos;
	
	//if(all(endPos < box_bound_min) || all(endPos > box_bound_max)) {
	//	return 0.0f;
	//}
	
	max_dist = get_bound_dist(endPos, -deltaPos);
	deltaPos *= min(deltaLen, max_dist) / STEP_COUNT;

    for (int i = 0; i < STEP_COUNT; i++)
    {
        endPos -= deltaPos * lerp(0.8, 1.2, hash(sin(dot(endPos, float3(145.4, 543.2, 153.5)) * timers.x)));
        float facture = hash3Df((endPos + float3(0.5, 0.0, 0.5) * (timers.x / 0.5)) * shader_param_1.z) * saturate(shader_param_1.w);
        float4 PS = mul(m_shadow, float4(mul(m_V, float4(endPos + float3(0, 0.05, 0), 1)).xyz, 1));
		float factor = GetBorderAtten(PS.xyz/PS.w);
        float3 max_factor = smoothstep(box_bound_max, box_bound_max - 10, endPos);
        float3 min_factor = smoothstep(box_bound_min, box_bound_min + 3, endPos);
        facture *= min_factor.x * min_factor.y * min_factor.z * max_factor.x * max_factor.y * max_factor.z;

		float s = shadow(PS);

		s = lerp(1, s, factor);

        float density = smoothstep(shader_param_7.x, shader_param_7.y, length(endPos - eye_position));

        fogheight += facture * smoothstep(__height, __height - 1.0f, endPos.y) * saturate(shader_param_5.x) * density * s;
    }
	
    return fogheight / STEP_COUNT;
}

float3 SSFX_HEIGHT_FOG(float3 P/*, float World_Py, inout float3 color*/)
{
    return Raymarch(P);
}

float SSFX_FOGGING(float Fog, float World_Py)
{
	// Height fog
	float fog_height = smoothstep(G_FOG_HEIGHT, -G_FOG_HEIGHT, World_Py) * G_FOG_HEIGHT_INTENSITY;

	// Add height fog to default distance fog.
	float fog_extra = saturate(Fog + fog_height * (Fog * G_FOG_HEIGHT_DENSITY));
	
	return 1.0f - fog_extra;
}