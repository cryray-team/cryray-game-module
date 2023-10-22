//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\h_fluid_common.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
float4 main( p_fluidsim input ) : SV_Target
{
    if( IsNonEmptyCell(input.texcoords.xyz) )
        return 0.f;

    float3 npos = GetAdvectedPosTexCoords(input);

    return Texture_color.SampleLevel( samLinear, npos, 0) * modulate;
}