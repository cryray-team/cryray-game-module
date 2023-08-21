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
//	Vertex
//	TODO: DX10: replace WorldViewProjection with m_WVP
PS_INPUT_RAYCAST main (VS_INPUT input)
{
    PS_INPUT_RAYCAST output = (PS_INPUT_RAYCAST)0;
    output.pos = float4(input.pos,1.f);
//    output.posInGrid = mul( float4( input.pos.xy*ZNear, 0, ZNear ), InvWorldViewProjection );
	output.posInGrid = mul( InvWorldViewProjection, float4( input.pos.xy*ZNear, 0.f, ZNear ));
    return output;
}