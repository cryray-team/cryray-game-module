//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\common.h"
#include "ps_fxaa.hlsl"

float4 main(v2p_aa_AA I) : SV_Target
{
	float2 rcpFrame = screen_res.zw;
	FxaaTex tex = { smp_rtlinear, s_image};

	return FxaaPixelShader(I.Tex0.xy,
            FxaaFloat4(0.f, 0.f, 0.f, 0.f),		// FxaaFloat4 fxaaConsolePosPos,
            tex,							// FxaaTex tex,
            tex,							// FxaaTex fxaaConsole360TexExpBiasNegOne,
            tex,							// FxaaTex fxaaConsole360TexExpBiasNegTwo,
            rcpFrame,							// FxaaFloat2 fxaaQualityRcpFrame,
            FxaaFloat4(0.f, 0.f, 0.f, 0.f),		// FxaaFloat4 fxaaConsoleRcpFrameOpt,
            FxaaFloat4(0.f, 0.f, 0.f, 0.f),		// FxaaFloat4 fxaaConsoleRcpFrameOpt2,
            FxaaFloat4(0.f, 0.f, 0.f, 0.f),		// FxaaFloat4 fxaaConsole360RcpFrameOpt2,
            0.35f,									// FxaaFloat fxaaQualitySubpix,
            0.166f,									// FxaaFloat fxaaQualityEdgeThreshold,
            0.f,								// FxaaFloat fxaaQualityEdgeThresholdMin,
            0.f,									// FxaaFloat fxaaConsoleEdgeSharpness,
            0.f,									// FxaaFloat fxaaConsoleEdgeThreshold,
            0.f,									// FxaaFloat fxaaConsoleEdgeThresholdMin,
            FxaaFloat4(0.f, 0.f, 0.f, 0.f)		// FxaaFloat fxaaConsole360ConstDir,
            );
	
}
