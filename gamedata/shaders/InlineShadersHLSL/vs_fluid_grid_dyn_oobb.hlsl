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

cbuffer DynOOBBData
{
	float3x4	WorldToLocal;	//	World to local of fog volume
	float3x4	LocalToWorld;	//	Local of fog volume to world
	float4		MassCenter;		//	Center for angular velocity
	float4		OOBBWorldAngularVelocity;
	float4		OOBBWorldTranslationVelocity;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Vertex
v2g_fluidsim_dyn_aabb main( v_fluidsim input)
{
    v2g_fluidsim_dyn_aabb output = (v2g_fluidsim_dyn_aabb)0;

    output.pos = float4(input.position.x, input.position.y, input.position.z, 1.f);
    output.cell0 = float3(input.textureCoords0.x, input.textureCoords0.y, input.textureCoords0.z);

	output.velocity = 0.f;

	{
		output.velocity = OOBBWorldTranslationVelocity;

		float3	r = mul( LocalToWorld, float4(output.cell0,1.f) ) - MassCenter;

		float3 AngularVel = cross( OOBBWorldAngularVelocity.xyz, r );

		output.velocity += AngularVel;

		output.velocity = mul( WorldToLocal, output.velocity );
	}
	
	for (int i=0; i<3; ++i)
	{
		output.clip0[i] = dot( float4(output.cell0,1.f), OOBBClipPlane[i]);
		output.clip1[i] = dot( float4(output.cell0,1.f), OOBBClipPlane[i+3]);
		//	Make box a voxel bigger in each direction
		//	BOX_EXPANSION - voxel diagonal length
		//output.clip0[i] += BOX_EXPANSION; 
		//output.clip1[i] += BOX_EXPANSION; 
	}

    return output;
}