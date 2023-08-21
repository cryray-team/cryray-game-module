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
 * @ Version: SCREEN SPACE SHADERS - UPDATE 14
 * @ Description: Main file
 * @ Modified time: 2023-01-18 21:11
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

#define SSFX_READY

#include "Headers\h_common.hlsl"
#include "Headers\h_lmodel.hlsl"
#include "Headers\h_hmodel.hlsl"

#include "ScreenSpaceAddon\h_screenspace_common_noise.hlsl"
#include "ScreenSpaceAddon\h_screenspace_common_ripples.hlsl"

static const float3 ssfx_hemisphere[32] =
{
	float3(-0.134f, 0.044f, -0.825f),	float3(0.045f, -0.431f, -0.529f),	float3(-0.537f, 0.195f, -0.371f),
	float3(0.525f, -0.397f, 0.713f),	float3(0.895f, 0.302f, 0.139f),		float3(-0.613f, -0.408f, -0.141f),
	float3(0.307f, 0.822f, 0.169f),		float3(-0.819f, 0.037f, -0.388f),	float3(0.376f, 0.009f, 0.193f),
	float3(-0.006f, -0.103f, -0.035f),	float3(0.098f, 0.393f, 0.019f),		float3(0.542f, -0.218f, -0.593f),
	float3(0.526f, -0.183f, 0.424f),	float3(-0.529f, -0.178f, 0.684f),	float3(0.066f, -0.657f, -0.570f),
	float3(-0.214f, 0.288f, 0.188f),	float3(-0.689f, -0.222f, -0.192f),	float3(-0.008f, -0.212f, -0.721f),
	float3(0.053f, -0.863f, 0.054f),	float3(0.639f, -0.558f, 0.289f),	float3(-0.255f, 0.958f, 0.099f),
	float3(-0.488f, 0.473f, -0.381f),	float3(-0.592f, -0.332f, 0.137f),	float3(0.080f, 0.756f, -0.494f),
	float3(-0.638f, 0.319f, 0.686f),	float3(-0.663f, 0.230f, -0.634f),	float3(0.235f, -0.547f, 0.664f),
	float3(0.164f, -0.710f, 0.086f),	float3(-0.009f, 0.493f, -0.038f),	float3(-0.322f, 0.147f, -0.105f),
	float3(-0.554f, -0.725f, 0.289f),	float3(0.534f, 0.157f, -0.250f),
};

#ifdef MSAA_ANTIALIASING_ENABLE
	Texture2DMS<float4> s_rimage;
#else
	Texture2D s_rimage;
#endif

uniform float4 sky_color;

TextureCube sky_s0;
TextureCube sky_s1;

static const float2 ssfx_pixel_size = 1.f / screen_res.xy;
static const float ssfx_PI = 3.14159265f;

struct RayTrace
{
	float2 r_pos;
	float2 r_step;
	float2 r_start;
	float r_length;
	float z_start;
	float z_end;
};

bool SSFX_is_saturated(float2 value) { return (value.x == saturate(value.x) && value.y == saturate(value.y)); }

bool SSFX_is_valid_uv(float2 value) { return (value.x >= 0.f && value.x <= 1.f) && (value.y >= 0.f && value.y <= 1.f); }

float2 SSFX_view_to_uv(float3 Pos)
{
	float4 tc = mul(m_P, float4(Pos, 1.f));
	return (tc.xy / tc.w) * float2(0.5f, -0.5f) + 0.5f;
}

float SSFX_calc_SSR_fade(float2 tc, float start, float end)
{
	// Vectical fade
	float ray_fade = saturate(tc.y * 5.f);
	
	// Horizontal fade
	float2 calc_edges = smoothstep(start, end, float2(tc.x, 1.f - tc.x));
	ray_fade *= calc_edges.x * calc_edges.y;

	return ray_fade;
}

float3 SSFX_yaw_vector(float3 Vec, float Rot)
{
	float s, c;
	sincos(Rot, s, c);

	// y-axis rotation matrix
	float3x3 rot_mat = 
	{
		c, 0.f, s,
		0.f, 1.f, 0.f,
		-s, 0.f, c
	};
	return mul(rot_mat, Vec);
}

float SSFX_get_depth(float2 tc, uint iSample : SV_SAMPLEINDEX)
{
	#ifndef MSAA_ANTIALIASING_ENABLE
		return s_position.Sample(smp_nofilter, tc).z;
	#else
		return s_position.Load(int3(tc * screen_res.xy, 0), iSample).z;
	#endif
}

float4 SSFX_get_position(float2 tc, uint iSample : SV_SAMPLEINDEX)
{
	#ifndef MSAA_ANTIALIASING_ENABLE
		return s_position.Sample(smp_nofilter, tc);
	#else
		return s_position.Load(int3(tc * screen_res.xy, 0), iSample);
	#endif
}

float3 SSFX_get_image(float2 tc, uint iSample : SV_SAMPLEINDEX)
{
	#ifndef MSAA_ANTIALIASING_ENABLE
		return s_rimage.Sample(smp_nofilter, tc).rgb;
	#else
		return s_rimage.Load(int3(tc * screen_res.xy, 0), 0).rgb;
	#endif
}

RayTrace SSFX_ray_init(float3 ray_start_vs, float3 ray_dir_vs, float ray_max_dist, int ray_steps, float noise)
{
	RayTrace rt;
	
	float3 ray_end_vs = ray_start_vs + ray_dir_vs * ray_max_dist;
	
	// Ray depth ( from start and end point )
	rt.z_start		= ray_start_vs.z;
	rt.z_end		= ray_end_vs.z;

	// Compute ray start and end (in UV space)
	rt.r_start		= SSFX_view_to_uv(ray_start_vs);
	float2 ray_end	= SSFX_view_to_uv(ray_end_vs);

	// Compute ray step
	float2 ray_diff	= ray_end - rt.r_start;
	rt.r_step		= ray_diff / (float)ray_steps; 
	rt.r_length		= length(ray_diff);
	
	rt.r_pos		= rt.r_start + rt.r_step * noise;
	
	return rt;
}

float3 SSFX_ray_intersect(RayTrace Ray, uint iSample)
{
	float len = length(Ray.r_pos - Ray.r_start);
	float alpha = len / Ray.r_length;
	float depth_ray = (Ray.z_start * Ray.z_end) / lerp(Ray.z_end, Ray.z_start, alpha);
	float depth_scene = SSFX_get_depth(Ray.r_pos, iSample);
	
	return float3(depth_ray.x - depth_scene, depth_scene, len);
}

// Half-way scene lighting
float4 SSFX_get_fast_scenelighting(float2 tc, uint iSample : SV_SAMPLEINDEX)
{
	#ifndef MSAA_ANTIALIASING_ENABLE
		float4 rL = s_accumulator.Sample(smp_nofilter, tc);
		float4 C = s_diffuse.Sample( smp_nofilter, tc );
	#else
		float4 rL = s_accumulator.Load(int3(tc * screen_res.xy, 0), iSample);
		float4 C = s_diffuse.Load(int3( tc * screen_res.xy, 0 ), iSample);
	#endif
	
	#ifdef SSFX_ENHANCED_SHADERS // We have Enhanced Shaders installed
		
		/*float3 hdiffuse = C.rgb + L_ambient.rgb;
		
		rL.rgb += rL.a * SRGBToLinear(C.rgb);
		
		return float4(LinearTosRGB((rL.rgb + hdiffuse) * saturate(rL.rrr * 100)), C.w);*/
		
		float3 result = rL.rgb + rL.a * C.rgb;
		result *= env_color.rgb;
		
		return float4(result, C.w);

	#else
		
		float3 hdiffuse = C.rgb + L_ambient.rgb;

		return float4((rL.rgb + hdiffuse) * saturate(rL.rrr * 100.f), C.w);
		
	#endif
}

// Full scene lighting
float3 SSFX_get_scene(float2 tc, uint iSample : SV_SAMPLEINDEX)
{
	#ifndef MSAA_ANTIALIASING_ENABLE
		float4 rP = s_position.Sample( smp_nofilter, tc );
	#else
		float4 rP = s_position.Load(int3(tc * screen_res.xy, 0), iSample);
	#endif

	if (rP.z <= 0.05f)
		return 0;

	#ifndef MSAA_ANTIALIASING_ENABLE
		float4 rD = s_diffuse.Sample( smp_nofilter, tc );
		float4 rL = s_accumulator.Sample(smp_nofilter, tc);
	#else
		float4 rD = s_diffuse.Load(int3(tc * screen_res.xy, 0), iSample);
		float4 rL = s_accumulator.Load(int3(tc * screen_res.xy, 0), iSample);
	#endif
	
	// Remove emissive materials for now...
	if (length(rL) > 10.f)
		rL = 0.f;

	float3 rN = gbuf_unpack_normal( rP.xy );
	float rMtl = gbuf_unpack_mtl( rP.w );
	float rHemi = gbuf_unpack_hemi( rP.w );

	float3 nw = mul( m_inv_V, rN );
	
	#ifdef SSFX_ENHANCED_SHADERS

		rL.rgb += rL.a * SRGBToLinear(rD.rgb);

		// hemisphere
		float3 hdiffuse, hspecular;
		hmodel(hdiffuse, hspecular, rMtl, rHemi, rD, rP, rN);

		// Final color 
		float3 rcolor = rL.rgb + hdiffuse.rgb;
		return LinearTosRGB(rcolor);

	#else

		// hemisphere
		float3 hdiffuse, hspecular;
		hmodel(hdiffuse, hspecular, rMtl, rHemi, rD.w, rP, rN);

		// Final color
		float4 light = float4(rL.rgb + hdiffuse, rL.w);
		float4 C = rD * light;
		float3 spec = C.www * rL.rgb;
		
		return C.rgb + spec;

	#endif
}

float3 SSFX_calc_sky(float3 dir)
{
	dir = SSFX_yaw_vector(dir, -sky_color.w); // Sky rotation
	
	dir.y = (dir.y - max(cos(dir.x) * 0.65f, cos(dir.z) * 0.65f)) * 2.1f; // Fix perspective
	dir.y -= -0.35f; // Altitude
	
	float3 sky0 = sky_s0.SampleLevel(smp_base, dir, 0).xyz;
	float3 sky1 = sky_s1.SampleLevel(smp_base, dir, 0).xyz;
	
	// Use hemi color or real sky color if the modded executable is installed.
//#ifndef SSFX_MODEXE
//	return saturate(L_hemi_color.rgb * 3.0f) * lerp(sky0, sky1, L_ambient.w);
//#else
	return saturate(sky_color.bgr * 3.f) * lerp(sky0, sky1, L_ambient.w);
//#endif
}

float3 SSFX_calc_env(float3 dir)
{
	float3 env0 = env_s0.SampleLevel(smp_base, dir, 0).xyz;
	float3 env1 = env_s1.SampleLevel(smp_base, dir, 0).xyz;
	
	return env_color.xyz * lerp( env0, env1, env_color.w );
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
// 1.3f water ~ 1.5f glass ~ 1.8f diamond
float SSFX_calc_fresnel(float3 V, float3 N, float ior)
{
	float cosi = clamp(-1.f, 1.f, dot(V, N));
	float etai = 1, etat = ior;
	if (cosi > 0.f)
	{
		etai = ior;
		etat = 1.f;
	}
	// Compute sini using Snell's law
	float sint = etai / etat * sqrt(max(0.f, 1.f - cosi * cosi));
	// Total internal reflection
	if (sint >= 1.f)
		return 1.f;

	float cost = sqrt(max(0.f, 1.f - sint * sint));
	cosi = abs(cosi); 
	float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
	float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));

	return (Rs * Rs + Rp * Rp) / 2.f;
}

static const float2x2 pp_rotation_matrix = { -0.666276f, 0.745705f, -0.745705f, -0.666276f };

float4 SSFX_Blur(float2 uv, float radius)
{
	float3 blur = 0.f;
	radius *= SSFX_gradient_noise_IGN(uv / 2.f * screen_res.xy) * 6.28f;
	
	float2 offset = float2(radius, radius);
	float r = 0.9f;
	
	for (int i = 0; i < 16; i++) 
	{
		r += 1.f / r; 
		offset = mul(offset, pp_rotation_matrix);
		blur += s_image.SampleLevel(smp_rtlinear, uv + (offset * (r - 1.f) * ssfx_pixel_size), 0).rgb;
	}
	float3 image = blur / 16.f;
	
	return float4(image, 1.f);
}