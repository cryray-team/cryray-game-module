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

float4 main(p_screen I) : SV_Target
{
	float4 depth;
	depth.x = s_position.Load(int3((I.tc0.xy + float2(0.f, 1.0f) * screen_res.zw) * screen_res.xy, 0), 0).z;
	depth.y = s_position.Load(int3((I.tc0.xy + float2(1.f, 0.65f) * screen_res.zw) * screen_res.xy, 0), 0).z;
	depth.z = s_position.Load(int3((I.tc0.xy + float2(-1.f, 0.65f) * screen_res.zw) * screen_res.xy, 0), 0).z;

	float4 sceneDepth;
	sceneDepth.x = normalize_depth(depth.x)*is_not_sky(depth.x);
	sceneDepth.y = normalize_depth(depth.y)*is_not_sky(depth.y);
	sceneDepth.z = normalize_depth(depth.z)*is_not_sky(depth.z);

	sceneDepth.w = (sceneDepth.x + sceneDepth.y + sceneDepth.z) * 0.333f;
	
	depth.w = saturate(1.f - sceneDepth.w*1000.f);
	
	float4 Color = float4(depth.w, depth.w, depth.w, sceneDepth.w);	
	return Color;
}