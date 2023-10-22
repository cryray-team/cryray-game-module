//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

/*
------------------------------------------------------------------
SMAA edge detection pass
------------------------------------------------------------------
References:
https://github.com/iryoku/smaa
------------------------------------------------------------------
Back to the Roots
*/

#include "Headers\h_common.hlsl"

#define SMAA_HLSL_4_1

#define SMAA_RT_METRICS screen_res.zwxy

#define	SMAA_PRESET_ULTRA
#define EDGE_DETECT_COLOR

#include "Headers\h_smaa.hlsl"

//Struct
struct p_smaa
{
	float4 hpos			: SV_Position;
	float2 tc0			: TEXCOORD0;        // Texture coordinates         (for sampling maps)
	float4 offset[3]	: TEXCOORD1;
};

float4 main(p_smaa I) : SV_Target
{
#if defined(EDGE_DETECT_COLOR)
	return float4(SMAAColorEdgeDetectionPS(I.tc0, I.offset, s_image), 0.f, 0.f);
#else
	return float4(SMAALumaEdgeDetectionPS(I.tc0, I.offset, s_image), 0.f, 0.f);
#endif
}