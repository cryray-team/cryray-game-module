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

cbuffer BoxBounds
{
//	float3	boxLBDcorner;
//	float3	boxRTUcorner;
	float4	boxLBDcorner;
	float4	boxRTUcorner;
}

struct PSDrawBoxOut
{
    float4 obstacle : SV_TARGET0;
    float4 velocity : SV_TARGET1;
};

bool PointIsInsideBox(float3 p, float3 LBUcorner, float3 RTDcorner)
{
    return ((p.x > LBUcorner.x) && (p.x < RTDcorner.x)
        &&  (p.y > LBUcorner.y) && (p.y < RTDcorner.y)
        &&  (p.z > LBUcorner.z) && (p.z < RTDcorner.z));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
PSDrawBoxOut main( p_fluidsim input )
{
    PSDrawBoxOut voxel;
    float3 innerboxLBDcorner = boxLBDcorner + 1;
    float3 innerboxRTUcorner = boxRTUcorner - 1;
    // cells completely inside box = 1.0
    if(PointIsInsideBox(input.cell0, innerboxLBDcorner, innerboxRTUcorner))
    {
        voxel.obstacle = 0.5f;
        voxel.velocity = 0.f;
        return voxel;
    }

    // cells in box boundary = 0.5
    if(PointIsInsideBox(input.cell0, boxLBDcorner, boxRTUcorner))
    {
        voxel.obstacle = 1.f;
        //voxel.velocity = float4(obstVelocity.xyz,1);
		voxel.velocity = 0.f;
        return voxel;
    }

	//	Kill texel to preserve other obstacles
	clip(-1);

    return (PSDrawBoxOut)0;
}