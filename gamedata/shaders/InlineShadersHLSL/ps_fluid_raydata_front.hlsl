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
float4 main(PS_INPUT_RAYDATA_FRONT input) : SV_Target
{
    float4 output;
//    float sceneZ = sceneDepthTex.SampleLevel( samLinearClamp, float2(input.pos.x/RTWidth, input.pos.y/RTHeight),0).r;
    float sceneZ = sceneDepthTex.SampleLevel( samLinearClamp, float2(input.pos.x/RTWidth, input.pos.y/RTHeight),0).z;
	if ( sceneZ < Z_EPSILON ) sceneZ = Z_MAX;
		

    if( sceneZ < input.depth )
    {
        // If the scene occludes intersection point we want to kill the pixel early in PS
        return OCCLUDED_PIXEL_RAYVALUE;
    }
    // We negate input.posInGrid because we use subtractive blending in front faces
    //  Note that we set xyz to 0 when rendering back faces
    output.xyz = -input.posInGrid;
    output.w = input.depth;
    return output;
}