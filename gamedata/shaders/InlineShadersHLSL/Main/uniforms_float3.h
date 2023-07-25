//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef UNIFORMS3_H
#define UNIFORMS3_H
	uniform float3 view_shadow_proj;
	uniform float3 L_sun_color;
	uniform float3 L_sun_dir_w;
	uniform float3 eye_position;
	uniform float3 L_sun_dir_e;
	uniform float3 eye_direction;
	uniform float3 dof_kernel;	// x,y - resolution pre-scaled z - just kernel size
	uniform float3 water_intensity;
#endif