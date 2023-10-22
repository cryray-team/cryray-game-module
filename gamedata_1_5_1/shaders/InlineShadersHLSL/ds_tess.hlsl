//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"
#include "Headers\h_tess.hlsl"

//if you use ccw then corresponding coefs are w, v, u
//if you use cw then corresponding coefs are u, v, w
[domain("tri")]
v2p_bumped main( HS_CONSTANT_DATA_OUTPUT input, 
             float3 uvw : SV_DomainLocation,
             const OutputPatch<p_bumped, 3> bp )
{
    v2p_bumped output;
	
	//interpolate in screen space
	//uvw = uvw*input.www/dot(uvw,input.www);
	
	float  minc = min( uvw.x, min( uvw.y, uvw.z ) );
	// if the vertex is not on an edge of the original triangle
	[flatten]if (minc!=0.f)
	{
		// if we are at not the centre of the triangle
		[flatten]if( ((1.f/3.f)-minc)> 0.01f )
		{
			// solving for making the smallest uvw component 0.0f as this means the bigger ones sum to 1.0f and 
			// are on the edge of the triangle
			// if vertex gets too close to the edge move it on to the edge to replace to old edge vertex
			// otherwise keep fK at 1.0f to restore the old uvw position
			
			float fK = (1.f/3.f)/((1.f/3.f)-minc);
			fK = minc < 0.1f ? fK : 1.f;
			// update uvw
			uvw = lerp((1.f/3.f).xxx, uvw, fK);
		}
	}

	float u = uvw.x;
	float v = uvw.y;
	float w = uvw.z;

	output.tcdh	    = bp[0].tcdh*w + bp[1].tcdh*v + bp[2].tcdh*u;
	output.position	= bp[0].position*w + bp[1].position*v + bp[2].position*u;

	float3 M1		= bp[0].M1*w + bp[1].M1*v + bp[2].M1*u; output.M1 = M1;
	float3 M2		= bp[0].M2*w + bp[1].M2*v + bp[2].M2*u; output.M2 = M2;
	float3 M3		= bp[0].M3*w + bp[1].M3*v + bp[2].M3*u; output.M3 = M3;
	float3 Normal	= normalize(float3(M1.z, M2.z, M3.z));

	float3 triPos = output.position.xyz;

#ifdef USE_TDETAIL
	output.tcdbump	= bp[0].tcdbump*w + bp[1].tcdbump*v + bp[2].tcdbump*u;
#endif
#ifdef USE_LM_HEMI
	output.lmh		= bp[0].lmh*w + bp[1].lmh*v + bp[2].lmh*u;
#endif

#if TESS_PN
	float3 N[3] =
	{
		float3(bp[0].M1.z, bp[0].M2.z, bp[0].M3.z),
		float3(bp[1].M1.z, bp[1].M2.z, bp[1].M3.z),
		float3(bp[2].M1.z, bp[2].M2.z, bp[2].M3.z)
	};
	
	float3 P[3] = 
	{
		bp[0].position.xyz,
		bp[1].position.xyz,
		bp[2].position.xyz
	};

	ComputePatchVertex(P, N, uvw, input.patch, output.position.xyz, Normal);
#endif

#if TESS_HM
#	ifdef USE_TDETAIL
	ComputeDisplacedVertex(output.position.xyz, Normal, output.tcdh, output.tcdbump);
#	else
	ComputeDisplacedVertex(output.position.xyz, Normal, output.tcdh, 0.f);
#	endif
#endif
	

	[flatten]if( minc==0.f )
		output.position.xyz = triPos;

	
	//output.M1.z = Normal.x;
	//output.M2.z = Normal.y;
	//output.M3.z = Normal.z;
	output.hpos	= mul(m_P, float4(output.position.xyz,1.f));

    return output;
}