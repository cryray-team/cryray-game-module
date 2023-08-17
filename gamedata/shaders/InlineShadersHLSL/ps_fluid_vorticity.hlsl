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
    float4 L = Texture_velocity1.SampleLevel( samPointClamp, LEFTCELL, 0 );
    float4 R = Texture_velocity1.SampleLevel( samPointClamp, RIGHTCELL, 0 );
    float4 B = Texture_velocity1.SampleLevel( samPointClamp, BOTTOMCELL, 0 );
    float4 T = Texture_velocity1.SampleLevel( samPointClamp, TOPCELL, 0 );
    float4 D = Texture_velocity1.SampleLevel( samPointClamp, DOWNCELL, 0 );
    float4 U = Texture_velocity1.SampleLevel( samPointClamp, UPCELL, 0 );

    float4 vorticity = float4(1.f, 1.f, 1.f, 1.f);
    vorticity.xyz = 0.5f * float3( (( T.z - B.z ) - ( U.y - D.y )) ,
                                 (( U.x - D.x ) - ( R.z - L.z )) ,
                                 (( R.y - L.y ) - ( T.x - B.x )) );
	
	vorticity.w = 0.f;
	
    return vorticity;
}