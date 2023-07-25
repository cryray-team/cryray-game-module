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

#ifndef MSAA_ANTIALIASING_ENABLE
float calc_hdao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d)
#else
float calc_hdao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d, uint iSample)
#endif
{ 
	// Locals
	int2   i2ScreenCoord;
	float2 f2ScreenCoord;
	float2 f2MirrorScreenCoord;
	float2 f2TexCoord;
	float2 f2MirrorTexCoord;
	float2 f2InvRTSize;
	float4 f4PosZ;
	float  ZDisp;
	float3 f3D0;
	float3 f3D1;
	float4 f4Diff;
	float4 f4Compare[2];
	float4 f4Occlusion = 0.0f;
	float  fOcclusion = 0.0f;
			
	// Compute integer screen coord, and store off the inverse of the RT Size
	f2InvRTSize	  = (1.f).xx / (g_f2RTSize.xy);
	f2ScreenCoord = tc * (g_f2RTSize.xy);
	i2ScreenCoord = int2( f2ScreenCoord );

	// Get the general valley angle, to scale the result by 
	float fDot = HDAOValleyAngle( i2ScreenCoord );
	
	ZDisp = P.z + g_fHDAOZDispScale * N.z;

	// Sample the center pixel for camera Z
	f2TexCoord = float2( f2ScreenCoord * f2InvRTSize );

	// Loop through each gather location, and compare with its mirrored location
	[unroll]for( int iGather=0; iGather<g_iNumRingGathers; ++iGather )
	{
		/*
		f2MirrorScreenCoord = ( g_f2SSAORingPattern[iGather] + float2( 1.0f, 1.0f ) ) * float2( -1.0f, -1.0f );
		
		f2TexCoord = float2( ( f2ScreenCoord + g_f2SSAORingPattern[iGather] ) * f2InvRTSize );
		f2MirrorTexCoord = float2( ( f2ScreenCoord + ( f2MirrorScreenCoord ) ) * f2InvRTSize );
		
		f4PosZ		  = GatherZSamples( f2TexCoord );
		f4Diff		  = ZDisp.xxxx - f4PosZ;
		f4Compare[0]  = ( f4Diff < g_fSSAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
		f4Compare[0] *= ( f4Diff > g_fSSAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
		
		f4PosZ		  = GatherZSamples( f2MirrorTexCoord );
		f4Diff		  = ZDisp.xxxx - f4PosZ;
		f4Compare[1]  = ( f4Diff < g_fSSAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
		f4Compare[1] *= ( f4Diff > g_fSSAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );

		f4Occlusion.xyzw += g_f4SSAORingWeight[iGather].xyzw * ( f4Compare[0].xyzw * f4Compare[1].zwxy );
		*/
		
		/* 
			OldSerpskiStalker - The comparison algorithm is disabled, it does not work correctly. 
			Instead, the dependence on shading on the weather has been added 
		*/
		
		f4Compare[0] = 1.0f; // Lower bound
		f4Compare[1] = 1.0f;// High bound

		f4Occlusion.xyzw += g_f4SSAORingWeight[iGather].xyzw * (f4Compare[0].xyzw * f4Compare[1].zwxy);
	}
	
	fOcclusion  = dot( f4Occlusion, g_fSSAOIntensity.xxxx ) / ( 2.0f * g_fRingWeightsTotal[g_iNumRings-1] );
	fOcclusion *= fDot;
	fOcclusion *= P.z < 0.5f ? 0.0f : lerp( 0.0f, 1.0f, saturate( P.z - 0.5f ) );
	fOcclusion  = 1.0f - saturate( fOcclusion );
	return fOcclusion.xxxx;
}