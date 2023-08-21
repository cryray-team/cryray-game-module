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
//////////////////////////////////////////////////////////////////////////////////////////
#define SMAA_HLSL_4_1

#define SMAA_INCLUDE_VS 1
#define SMAA_RT_METRICS screen_res.zwxy
//////////////////////////////////////////////////////////////////////////////////////////

#include "Headers\h_smaa.hlsl"

//Struct
struct p_smaa
{
	float4 hpos			: SV_Position;
	float2 tc0			: TEXCOORD0;        // Texture coordinates         (for sampling maps)
	float2 pixcoord		: TEXCOORD1;	
	float4 offset[3]	: TEXCOORD2;
};

struct	v2p_smaa
{
	float2 tc0 : TEXCOORD0;	
	float4 HPos : POSITIONT;  	// Clip-space position 	(for rasterization)
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
p_smaa main(v2p_smaa I)
{
	p_smaa O;
	// Transform to screen space (in d3d9 it was done automatically)
	O.hpos.x 	=  (I.HPos.x * screen_res.z * 2.f - 1.f);
	O.hpos.y 	= -(I.HPos.y * screen_res.w * 2.f - 1.f);
	O.hpos.zw 	=	I.HPos.zw;
	
    O.tc0 		= 	I.tc0;
  
	SMAABlendingWeightCalculationVS( I.tc0, O.pixcoord, O.offset );

    return O; 
}