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
SMAA neighbour blend pass
------------------------------------------------------------------
References:
https://github.com/iryoku/smaa
------------------------------------------------------------------
Back to the Roots
*/

#include "Headers\common.h"

#define SMAA_HLSL_4_1

#define SMAA_RT_METRICS screen_res.zwxy
#define	SMAA_PRESET_ULTRA

#include "Headers\smaa.h"

Texture2D s_blendtex;

struct p_smaa
{
	float4 hpos			: SV_Position;
	float2 tc0			: TEXCOORD0;        // Texture coordinates         (for sampling maps)
	float4 offset		: TEXCOORD2;
};

float4 main(p_smaa I) : SV_Target
{
	return SMAANeighborhoodBlendingPS(I.tc0, I.offset, s_image, s_blendtex);
}