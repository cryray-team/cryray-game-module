//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	common_cbuffers_h_included
#define	common_cbuffers_h_included

//	Used by dynamic lights and volumetric effects
cbuffer	dynamic_light
{
	float4	Ldynamic_color;	// dynamic light color (rgb1)	- spot/point/sun
	float4	Ldynamic_pos;	// dynamic light pos+1/range(w)	- spot/point
	float4	Ldynamic_dir;	// dynamic light direction		- sun
}

#endif	//	common_cbuffers_h_included