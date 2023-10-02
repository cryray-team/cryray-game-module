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

float calc_new_hdao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d)
{
	// Locals
	uint2 u2CenterScreenCoord;
    float2 f2ScreenCoord;
	float2 f2MirrorScreenCoord;
	float fCenterZ;
	float2 f2SampledZ;
	float2 f2Diff;
	float2 f2Compare;
	float fOcclusion = 0.0f;
	int iValley;
	float fDot;
								
	// Compute screen coord, and store off the inverse of the RT Size
	u2CenterScreenCoord = uint2(floor(tc * g_f2RTSize));
	
	// Get the general valley angle, to scale the result by 
	fDot = ValleyAngle(u2CenterScreenCoord);

	// Sample center texel, convert to camera space and add normal
	float fDepth = g_txDepth.Load(int3( u2CenterScreenCoord, 0), 0).z;

        fCenterZ = fDepth + (g_txNormal.Load( int3( u2CenterScreenCoord, 0), 0).x * g_fHDAONormalScale);

	// Loop through each valley
	for( iValley = 0; iValley < NUM_VALLEYS; iValley ++ )
	{
		// Sample depth & convert to camera space
        f2SampledZ.x = g_txDepth.Load( int3( ( u2CenterScreenCoord + g_i2HDAOSamplePattern[iValley] ), 0), 0).z;
		f2SampledZ.y = g_txDepth.Load( int3( ( u2CenterScreenCoord - g_i2HDAOSamplePattern[iValley] ), 0), 0).z;

        // Sample normal and do a scaled add
	        f2SampledZ.x += (g_txNormal.Load( int3( ( u2CenterScreenCoord + g_i2HDAOSamplePattern[iValley]), 0), 0 ).x * g_fHDAONormalScale); 
	        f2SampledZ.y += (g_txNormal.Load( int3( ( u2CenterScreenCoord - g_i2HDAOSamplePattern[iValley]), 0), 0 ).x * g_fHDAONormalScale);

		// Detect valleys 
		f2Diff     = fCenterZ.xx - f2SampledZ;
		f2Compare  = (f2Diff < g_fHDAORejectRadius.xx ) ? (1.0f) : (0.0f);
		f2Compare *= (f2Diff > g_fHDAOAcceptRadius.xx ) ? (1.0f) : (0.0f);
						
		// Accumulate weighted occlusion
		fOcclusion += f2Compare.x * f2Compare.y * g_fHDAOSampleWeights[iValley];	
	}
					
	// Finally calculate the HDAO occlusion value
	fOcclusion /= g_fWeightTotal;
	fOcclusion *= g_fHDAOIntensity * fDot;
	fOcclusion *= fCenterZ < 0.5f ? 0.0f : lerp(0.0f, 1.0f, saturate( fCenterZ - 0.5f));
	fOcclusion = 1.0f - saturate( fOcclusion );
	
	return fOcclusion;
}