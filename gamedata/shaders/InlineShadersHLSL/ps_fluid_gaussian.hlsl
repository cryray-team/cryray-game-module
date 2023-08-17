//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\fluid_common.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
float4 main( p_fluidsim input ) : SV_Target
{
    if( IsNonEmptyCell(input.texcoords.xyz) )
        return 0.f;

	float dist = length( input.cell0.xyz - center.xyz ) * size;

    float4 result = float4(1.f, 1.f, 1.f, 1.f);

	result.rgb = splatColor.rgb + length(splatColor.rgb)*sin(splatColor.rgb*10.f+input.cell0.xyz*5.f)*0.2f;
	result.a = exp( -dist*dist/0.5f );

    return result;
}