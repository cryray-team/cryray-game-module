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
//	A full-screen edge detection pass to locate artifacts
VS_OUTPUT_EDGE main( VS_INPUT input )
{
    VS_OUTPUT_EDGE output = (VS_OUTPUT_EDGE)0;
    output.position = float4(input.pos,1.f);

    float2 texelSize = 1.f / float2(RTWidth,RTHeight);
    float2 center = float2( (input.pos.x+1)/2.f , 1.f - (input.pos.y+1.f)/2.f );

    // Eight nearest neighbours needed for Sobel.
    output.textureUV00 = center + float2(-texelSize.x, -texelSize.y);
    output.textureUV01 = center + float2(-texelSize.x,  0.f);
    output.textureUV02 = center + float2(-texelSize.x,  texelSize.y);

    output.textureUV10 = center + float2(0.f, -texelSize.y);
    output.textureUV12 = center + float2(0.f,  texelSize.y);

    output.textureUV20 = center + float2(texelSize.x, -texelSize.y);
    output.textureUV21 = center + float2(texelSize.x,  0.f);
    output.textureUV22 = center + float2(texelSize.x,  texelSize.y);

    return output;
}