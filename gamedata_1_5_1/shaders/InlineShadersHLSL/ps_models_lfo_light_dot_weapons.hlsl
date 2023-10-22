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

float resize(float input, float factor, float offset)
{
	return (input - 0.5f + offset) / factor + 0.5f - offset;
}

float4 main(float4 pos2d : SV_Position): SV_Target
{
	float factor = screen_res.y / 2160.f;
	factor = factor * (1.f / (m_hud_params.z / 0.45f)) * 0.75f;
	float size = 512.f * factor;
	
	pos2d.x = pos2d.x - (screen_res.x - size) / 2.f;
	pos2d.y = pos2d.y - (screen_res.y - size) / 2.f;
	pos2d.x = resize(pos2d.x, factor, 0);
	pos2d.y = resize(pos2d.y, factor, 0);
	
	return s_base.Load(pos2d.xyz) * m_hud_params.x;
}