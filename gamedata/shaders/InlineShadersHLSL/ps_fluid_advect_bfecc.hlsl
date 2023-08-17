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

    float3 npos = GetAdvectedPosTexCoords(input);

    float4 r;
    float3 diff = abs( floatVolumeDim.xyz - input.cell0 );

    // Must use regular semi-Lagrangian advection instead of BFECC at the volume boundaries
    if( (diff.x > (floatVolumeDim.x-4.f)) || (diff.y > (floatVolumeDim.y-4.f)) || (diff.z > (floatVolumeDim.z-4.f)) )
    {
       r = Texture_color.SampleLevel( samLinear, npos, 0);
    }
    else
    {
        // Texture_color contains \phi^n; Texture_tempscalar contains \bar{\phi}
        //  (i.e.: the result of 1 forward advection step, followed by a backwards advection step)
        r = 1.5f * Texture_color.SampleLevel( samLinear, npos, 0)
            - 0.5f * Texture_tempscalar.SampleLevel( samLinear, npos, 0);
    }

    r = saturate(r);
    return r*modulate;
}