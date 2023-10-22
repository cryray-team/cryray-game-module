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
//	Vertex
v2g_fluidsim_clip main( v_fluidsim input)
{
    v2g_fluidsim_clip output = (v2g_fluidsim_clip)0;

    output.pos = float4(input.position.x, input.position.y, input.position.z, 1.f);
    output.cell0 = float3(input.textureCoords0.x, input.textureCoords0.y, input.textureCoords0.z);
    output.texcoords = float3( (input.textureCoords0.x)/(textureWidth),
                              (input.textureCoords0.y)/(textureHeight), 
                              (input.textureCoords0.z+0.5f)/(textureDepth));

    float x = output.texcoords.x;
    float y = output.texcoords.y;
    float z = output.texcoords.z;

    // compute single texel offsets in each dimension
    float invW = 1.f/textureWidth;
    float invH = 1.f/textureHeight;
    float invD = 1.f/textureDepth;

    output.LR = float2(x - invW, x + invW);
    output.BT = float2(y - invH, y + invH);
    output.DU = float2(z - invD, z + invD);

//	float3	clip0				: SV_ClipDistance0;
//	float3	clip1				: SV_ClipDistance1;
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