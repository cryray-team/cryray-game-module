#ifndef CBUFFER_CONSTANTS
#define CBUFFER_CONSTANTS

cbuffer D3D11
{
	uniform sampler smp_jitter;
	
	uniform Texture2D s_ambient_occlusion;
	uniform Texture2D s_ambient_occlusion_temp;
	
	uniform Texture2D s_mask_nm_1;
	uniform Texture2D s_mask_nm_2;
	uniform Texture2D s_mask_nm_3;
	uniform Texture2D s_mask_nm_4;
	uniform Texture2D s_mask_nm_5;
	uniform Texture2D s_mask_nm_6;
	uniform Texture2D s_mask_nm_7;
	uniform Texture2D s_mask_nm_8;
	uniform Texture2D s_mask_nm_9;
	uniform Texture2D s_mask_nm_10;
	
	uniform Texture2D jitter0;
	uniform Texture2D jitter1;
	uniform Texture2D s_rimage;
	uniform Texture2D s_nmap;
	uniform Texture2D s_leaves;
	uniform Texture2D s_bluenoise; //-' Screen Space Addon
	uniform Texture2D s_rainsplash; //-' Screen Space Addon
	uniform Texture2D s_watercaustics; //-' Screen Space Addon
	
	uniform float3 water_intensity; //-'
	
	uniform float3 eye_direction; //-'
	
	uniform float3 L_sun_dir_e;
	
	uniform float3 drops_control;	
	uniform float4 mask_control; 
	
	uniform float3x4 m_sunmask;	// ortho-projection
	
	uniform float3x4 m_v2w; //-' invert(Device.mView)
	
	uniform float4x4 m_texgen;
	
	uniform float4 blur_params;
	
	uniform float4 ssfx_is_underground;
	uniform float4 screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)
	
	uniform float4 shader_param_1;
	uniform float4 shader_param_2;
	uniform float4 shader_param_3;
	uniform float4 shader_param_4;
	uniform float4 shader_param_5;
	uniform float4 shader_param_6;
	uniform float4 shader_param_7;
	uniform float4 shader_param_8;
	
	uniform float4 gloss_params; //-' x - tree, y - grass, z - rain
	
	uniform float4 hemisphere_depth;
	uniform float4 weather_bloom;
	uniform float4 c_sunshafts;
	uniform float4 ogse_c_screen;		// x - fFOV, y - fAspect, z - Zf/(Zf-Zn), w - Zn*tan(fFov/2)
	
	uniform float4 far_near_plane;

	uniform float4x4 m_shadow;
	
	uniform	float4 shading_model;
}
#endif