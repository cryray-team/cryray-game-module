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

struct PSDrawBoxOut
{
    float4 obstacle : SV_TARGET0;
    float4 velocity : SV_TARGET1;
};

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
PSDrawBoxOut main( p_fluidsim_dyn_aabb input )
{
    PSDrawBoxOut voxel;

    // cells completely inside box = 0.5
	if( (input.clip0.x>BOX_EXPANSION) && (input.clip1.x>BOX_EXPANSION) && 
		(input.clip0.y>BOX_EXPANSION) && (input.clip1.y>BOX_EXPANSION) &&
		(input.clip0.z>BOX_EXPANSION) && (input.clip1.z>BOX_EXPANSION) )
    {
        voxel.obstacle = 0.5f;
        voxel.velocity = 0.f;
        return voxel;
    }
	else	// cells in box boundary = 1.0
    {
        voxel.obstacle = 1.f;
        voxel.velocity = float4(input.velocity,1.f);
        return voxel;
    }
}