//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef FOG_H
#define FOG_H
#include "Headers\h_common.hlsl"	

static const float min_dist = 0.f; //-' decommissioned

static const float3x3 rotate_y = float3x3
(
	cos(timers.y * 0.75f), 0.f, sin(timers.y * 0.75f),
	0.f, 			    1.f,            0.f,
	-sin(timers.y * 0.75f), 0.f, cos(timers.y * 0.75f)
);

float3 random3(float3 c)
{
    float4 seed = float4(0.f, c) * float4(123.456f, 456.789f, 789.123f, 321.654f);
    seed = frac(sin(seed) * 43758.5453f);
    return (seed.xyz * 2.f - 1.f);
}

float noise(float3 p)
{
    float3 i = floor(p);
    float3 f = frac(p);
    f = f * f * (3.f - 2.f * f);

    float3 rand1 = random3(i);
    float3 rand2 = random3(i + float3(1.f, 0.f, 0.f));
    float3 rand3 = random3(i + float3(0.f, 1.f, 0.f));
    float3 rand4 = random3(i + float3(0.f, 0.f, 1.f));

    float a = dot(rand1, f);
    float b = dot(rand2, f - float3(1.f, 0.f, 0.f));
    float c = dot(rand3, f - float3(0.f, 1.f, 0.f));
    float d = dot(rand4, f - float3(0.f, 0.f, 1.f));

    return (a + b + c + d) * 0.5f * timers.x;
}

float smoothstep_fog_density(float distance, float min_dist, float max_dist, float density)
{
    // Calculate the percentage of distance from min_dist to max_dist
    float dist_percentage = saturate((distance - min_dist) / (max_dist - min_dist));
    
    // Smoothly interpolate the density based on the dist_percentage
    float smooth_density = smoothstep(0.f, 1.f, dist_percentage) * density;

    return smooth_density;
}

float get_height_weight(float3 P, float height_min, float height_max, float weight_min, float weight_max)
{
    float height_weight = 0.f;
    if (P.y < height_min)
    {
        height_weight = weight_min;
    }
    else if (P.y > height_max)
    {
        height_weight = weight_max;
    }
    else
    {
        height_weight = lerp(weight_min, weight_max, (P.y - height_min) / (height_max - height_min));
    }
    return height_weight;
}

float4 get_linear_fog(float3 P)
{
	float sun_intensity = 0.05f;
	
	//get distance
	float distance = length(P.xyz);

	//calculate linear fog
	float fog = saturate(distance*fog_params.w + fog_params.x);
	
	//calculate pseudoscattering!
    float sun = max(saturate(dot(-eye_direction, normalize(L_sun_dir_w))), 0);

    // final color mixing
    float3 final_color = lerp(fog_color.rgb, (L_sun_color.rgb * sun_intensity) + fog_color.rgb, smoothstep(0.3f, 0.7f, pow(sun, 1.5f)));

	return float4(final_color.xyz,fog);
}

float4 get_height_fog(float3 P)
{
    float sun_intensity = 0.05f;
    float height_weight = get_height_weight(P, 0.f, 10.f, 0.f, 1.f);
    float height = lerp(0.f, ground_fog.x, height_weight);
    float density = ground_fog.y;
    float max_dist = ground_fog.z;    
    
    //get world position and distance!
    float3 P_world = mul(m_v2w, float4(P,1.f)).xyz;
    
    // Apply rotation
    float3 rotated_P = mul(rotate_y, P);
    
    float distance = length(rotated_P);
    
    //height falloff (workaround for hard edges)
    float falloff = max(0.f, max(height - P_world.y, height - eye_position.y));
    
    //calculate exponential fog
    float fog_density = smoothstep_fog_density(distance, min_dist, max_dist, density);
    float fog = pow(1.f - exp(-max(0.f, distance - min_dist) / ((max_dist - min_dist))), 2.f)*fog_density;
    
    //add second dynamic layer
    fog += (fog*0.125f) * sin(timers.x*0.75f + P_world.x*0.1f);
    
    //calculate pseudoscattering!
    float sun = max(saturate(dot(-eye_direction, normalize(L_sun_dir_w))), 0.f);
    
    float edge_dist = 0.9f; // расстояние, на котором начинается рассеяние краев
    float edge_falloff = 1.5f; // показатель степени для функции smoothstep
    float edge = smoothstep(0.f, 1.f, (distance - max_dist + edge_dist) / edge_dist);
    
    // final color mixing
    float3 final_color = lerp(fog_color.rgb, (L_sun_color.rgb * sun_intensity) + fog_color.rgb, smoothstep(0.3f, 0.7f, pow(sun, 1.5f)));
    final_color.rgb = lerp(final_color.rgb, fog_color.rgb, edge * pow(edge, edge_falloff));
    
    density += noise(P) * sin(timers.x*0.75f + P_world.xyz*0.1f);
    
    return float4(final_color.xyz, saturate(fog*falloff*ground_fog_enable));
}

float get_height_fog_sky_effect(float3 P)
{
	//our settings
	float height = ground_fog.x;
	float density = ground_fog.y;
	float max_dist = ground_fog.z; 

	//get world position and distance!
	float3 P_world = mul(0.f, float4(P,1.f)).xyz;
	float distance = length(P.xyz);

	//Check if we need height fog
	//height falloff (workaround for hard edges)
	float falloff = max(0.f, max(height - P_world.y, height - eye_position.y));

	//calculate exponential fog
	float fog_density = smoothstep_fog_density(distance, min_dist, max_dist, density);
    float fog = pow(1.f - exp(-max(0.f, distance - min_dist) / ((max_dist - min_dist))), 2.f)*fog_density;
	
	fog += (fog*0.125f) * sin(timers.x*0.75f + P_world.x*0.1f);
	
	return saturate(fog*falloff*ground_fog_enable);
}

float get_height_fog_water(float3 P, float3 P_world)
{
	//our settings	
	int error = 1;
	
	float height = ground_fog.x;
	float density = ground_fog.y;
	float max_dist = ground_fog.z; 

	//get world position and distance!
	float distance = length(P.xyz);

	//Check if we need height fog
	//height falloff (workaround for hard edges)
	float falloff = abs(P_world.y - height);

	//cut ugly fog!!1!
	if (P_world.y > height+0.025f)
		error = 0.f;
	
	//calculate exponential fog
	float fog = pow(1.f - exp(-max(0.f, distance - min_dist) / ((max_dist - min_dist))), 2.f)*density;
	
	//add second dynamic layer
	fog += (fog*0.125f) * sin(timers.x*0.75f);
	return saturate(fog*falloff*error*ground_fog_enable);
}

float3 get_fog_folor(float3 Pos)
{
	const float _distOffset = -150.f;  
    const float _distRange = 500.f;  
    const float _mul = 1.f / _distRange;  
    const float _bias = _distOffset * _mul;  

	float3 w_pos =  mul(m_v2w,float4(Pos,1.f)).xyz;

	float frag_dist = length(w_pos.xyz);  
   	float3 frag_dir = w_pos.xyz / frag_dist;  
   
   	float dot_fragDirSunDir = dot(-L_sun_dir_w.xyz, frag_dir); 
	float disk_size = 3.f;

    float FoggySunDisk = dot_fragDirSunDir;  
    {  
		FoggySunDisk = abs(FoggySunDisk);  
		FoggySunDisk *= FoggySunDisk;  
		FoggySunDisk *= saturate( frag_dist * _mul + _bias );  
		FoggySunDisk = pow(FoggySunDisk, disk_size);
	}  

	return lerp( fog_color, (dot_fragDirSunDir < 0.f ? fog_color : L_sun_color.xyz), FoggySunDisk ); 
}
#endif