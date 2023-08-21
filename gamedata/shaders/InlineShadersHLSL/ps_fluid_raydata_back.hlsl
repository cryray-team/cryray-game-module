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
float4 main(PS_INPUT_RAYDATA_BACK input) : SV_Target
{
    float4 output;
//    float sceneZ = sceneDepthTex.SampleLevel( samLinearClamp, float2(input.pos.x/RTWidth, input.pos.y/RTHeight),0).r;
    float sceneZ = sceneDepthTex.SampleLevel( samLinearClamp, float2(input.pos.x/RTWidth, input.pos.y/RTHeight),0).z;
	if ( sceneZ < Z_EPSILON ) sceneZ = Z_MAX;
    
    // This value will only remain if no fragments get blended on top in the next pass (front-faces)
    //  which would happen if the front faces of the box get clipped by the near plane of the camera
    output.xyz = NEARCLIPPED_PIXEL_RAYPOS;
    output.w = min(input.depth, sceneZ);
    return output;
}