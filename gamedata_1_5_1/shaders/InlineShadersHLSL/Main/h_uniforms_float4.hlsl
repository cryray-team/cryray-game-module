//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef UNIFORMS4_H
#define UNIFORMS4_H
	uniform float4 dir2D; 
	uniform float4 array[61*4];
	uniform float4 static_drops_param;
	uniform float4 consts; // {1/quant,1/quant,diffusescale,ambient}
	uniform float4 c_scale, c_bias, wind, wave;
	uniform float4 nv_color;
	uniform float4 blur_params;
	uniform float4 ogse_c_screen;		// x - fFOV, y - fAspect, z - Zf/(Zf-Zn), w - Zn*tan(fFov/2)
	uniform float4 ssss_params; // x - exposure, y - density, z - sample size, w - radius
	uniform float4 nv_postprocessing;
	uniform float4 pos_decompression_params;
	uniform float4 pos_decompression_params2;
	uniform float4 RainDensity;
	uniform float4 WorldX;
	uniform float4 WorldZ;
	uniform float4 m_lmap[2];
	uniform float4 bloom_params;
	uniform float4 m_hud_params;	// zoom_rotate_factor, secondVP_zoom_factor, NULL, NULL
	uniform float4 m_blender_mode;
	uniform	float4 shading_model;
	uniform	float4 sslr_factor;
	uniform	float4 fake_pbr_params;
	uniform float4 pda_params;
	uniform float4 dof_params; // x - near y - focus z - far w - sky distance
	uniform float4 sbones_array[256.f - 22.f]; //-' h_skin.h
	uniform float4 eye_direction_lerp;
	uniform float4 eye_position_lerp;
	uniform float4 fakescope_params1; // power, inner blur, outer blur, brightness
	uniform float4 fakescope_params2; // chroma abber, fog attack(aim), fog attack(move), fog max travel
	uniform float4 fakescope_params3; // radius, relative fog radius, fog sharpness, scope sway amount
	uniform float4 ssfx_wpn_dof_1;
	uniform float4 shader_param_1;
	uniform float4 shader_param_2;
#endif