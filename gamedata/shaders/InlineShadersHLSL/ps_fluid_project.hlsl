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
    if( IsBoundaryCell(input.texcoords.xyz) )
        return GetObstVelocity(input.texcoords.xyz);

    float pCenter = Texture_pressure.SampleLevel( samPointClamp, input.texcoords, 0 ).r; 
    float pL = Texture_pressure.SampleLevel( samPointClamp, LEFTCELL, 0 ).r;
    float pR = Texture_pressure.SampleLevel( samPointClamp, RIGHTCELL, 0 ).r;
    float pB = Texture_pressure.SampleLevel( samPointClamp, BOTTOMCELL, 0 ).r;
    float pT = Texture_pressure.SampleLevel( samPointClamp, TOPCELL, 0 ).r;
    float pD = Texture_pressure.SampleLevel( samPointClamp, DOWNCELL, 0 ).r;
    float pU = Texture_pressure.SampleLevel( samPointClamp, UPCELL, 0 ).r;

    float4 velocity = float4(1.f, 1.f, 1.f, 1.f);
    float3 obstV = float3(0.f,0.f,0.f);
    float3 vMask = float3(1.f,1.f,1.f);
    float3 vLeft = GetObstVelocity(LEFTCELL).xyz;
    float3 vRight = GetObstVelocity(RIGHTCELL).xyz;
    float3 vBottom = GetObstVelocity(BOTTOMCELL).xyz;
    float3 vTop = GetObstVelocity(TOPCELL).xyz;
    float3 vDown = GetObstVelocity(DOWNCELL).xyz;
    float3 vUp = GetObstVelocity(UPCELL).xyz;
    float3 v = float3(1.f, 1.f, 1.f);

    if( IsBoundaryCell(LEFTCELL.xyz) )  { pL = pCenter; obstV.x = vLeft.x; vMask.x = 0; }
    if( IsBoundaryCell(RIGHTCELL.xyz) ) { pR = pCenter; obstV.x = vRight.x; vMask.x = 0; }
    if( IsBoundaryCell(BOTTOMCELL.xyz) ){ pB = pCenter; obstV.y = vBottom.y; vMask.y = 0; }
    if( IsBoundaryCell(TOPCELL.xyz) )   { pT = pCenter; obstV.y = vTop.y; vMask.y = 0; }
    if( IsBoundaryCell(DOWNCELL.xyz) )  { pD = pCenter; obstV.z = vDown.z; vMask.z = 0; }
    if( IsBoundaryCell(UPCELL.xyz) )    { pU = pCenter; obstV.z = vUp.z; vMask.z = 0; }

    v = ( Texture_velocity1.SampleLevel( samPointClamp, input.texcoords, 0 ).xyz -
                 (0.5f*modulate*float3( pR - pL, pT - pB, pU - pD )) );

    velocity.xyz = (vMask * v) + obstV;
	velocity.w = 1.f;
	
    return velocity;
}