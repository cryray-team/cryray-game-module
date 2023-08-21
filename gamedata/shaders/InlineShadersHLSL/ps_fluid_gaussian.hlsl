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

	float dist = length( input.cell0 - center ) * size;
//	float dist = length( input.cell0 - center ) * size / 2;
    float4 result;
//    result.rgb = splatColor;    // + sin(splatColor.rgb*10.0+cell*5.0)*0.2;
	result.rgb = splatColor + length(splatColor)*sin(splatColor.rgb*10.f+input.cell0*5.f)*0.2f;
//	result.a = exp( -dist*dist/(0.05) );
//	result.a = exp( -dist*dist );
	result.a = exp( -dist*dist/0.5f );

    return result;
}