//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

float NormalizeDepth(float Depth)
{
	return saturate(Depth / 100.f);
}

float4 main(p_screen I) : SV_Target
{
	float4 	scene 		= s_image.Load(int3(I.tc0.xy * screen_res.xy, 0), 0);
	float 	sceneDepth 	= s_position.Load(int3(I.tc0.xy * screen_res.xy, 0), 0).z;
	float 	RESDepth 	= NormalizeDepth(sceneDepth) * 1000.f;
	
	float4 	outColor 	= float4(scene.xyz*(1.f - RESDepth),RESDepth);
	return outColor;
}