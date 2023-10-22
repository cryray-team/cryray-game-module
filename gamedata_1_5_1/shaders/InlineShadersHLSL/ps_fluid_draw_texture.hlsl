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
    if( textureNumber == 1)
        return abs(Texture_color.SampleLevel(samLinear,input.texcoords,0)).xxxx;
    else if( textureNumber == 2)
        return abs(Texture_velocity0.SampleLevel(samLinear,input.texcoords,0));
    else
        return float4(abs(Texture_obstvelocity.SampleLevel(samLinear,input.texcoords,0).xy),
            abs(Texture_obstacles.SampleLevel(samLinear,input.texcoords,0).r),1 );
}