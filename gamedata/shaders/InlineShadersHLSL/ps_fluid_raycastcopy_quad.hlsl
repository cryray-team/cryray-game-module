//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\h_fluid_common_render.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
//	TODO: DX10: replace WorldViewProjection with m_WVP
float4 main(PS_INPUT_RAYCAST input) : SV_Target
{
    float edge = edgeTex.Sample(samLinearClamp, float2(input.pos.x/RTWidth,input.pos.y/RTHeight)).r;

#ifdef	RENDER_FIRE
    float4 tex = rayCastTex.Sample(samLinearClamp, float2(input.pos.x/RTWidth,input.pos.y/RTHeight));
	if(edge > 0.f && tex.a > 0.f)
		return Raycast(input);
	else
		return tex;
#else	//	RENDER_FIRE
    float4 tex = rayCastTex.Sample(samLinearClamp, float2(input.pos.x/RTWidth,input.pos.y/RTHeight));
	if(edge > 0.f && tex.a > 0.f)
		return Raycast(input)*DiffuseLight;
	else
		return tex*DiffuseLight;
#endif	//	RENDER_FIRE
}