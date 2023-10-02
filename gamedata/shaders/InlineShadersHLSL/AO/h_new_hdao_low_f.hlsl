//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef HDAO_L_F_H
#define HDAO_L_F_H
	static float g_fHDAOZDispScale 		  = 0.1f;
	static float g_fSSAORejectRadius      = 0.43f;
	static float g_fSSAOIntensity         = 1.5f;
	static float g_fSSAOAcceptRadius      = -0.0005f;
	static float g_fSSAOAcceptAngle       = 0.98f; // Used by the ValleyAngle function to determine shallow valleys
	
#if SSAO_QUALITY == 3
	static const int g_iNumRingGathers = NUM_RING_2_GATHERS;
	static const int g_iNumRings 	   = RING_2;
	static const int g_iNumNormals     = NUM_RING_0_GATHERS;
#elif SSAO_QUALITY == 2
	static const int g_iNumRingGathers = NUM_RING_1_GATHERS;
	static const int g_iNumRings 	   = RING_1;
	static const int g_iNumNormals     = NUM_RING_0_GATHERS;
#elif SSAO_QUALITY == 1
	static const int g_iNumRingGathers = NUM_RING_0_GATHERS;
	static const int g_iNumRings 	   = RING_0;
	static const int g_iNumNormals     = 0;
#endif
	
	static const int2 g_i2HDAONormalLoadPattern[NUM_NORMAL_LOADS] = 
	{
		{0, -9},
		{6, -6},
		{10, 0},
		{8, 9},
	};
	
	// Ring sample pattern
	static const float2 g_f2SSAORingPattern[NUM_RING_2_GATHERS] = 
	{
		// Ring 0
		{1.f, -1.f},
		{0.f, 1.f},
		
		// Ring 1
		{0.f, 3.f},
		{2.f, 1.f},
		{3.f, -1.f},
		{1.f, -3.f},
		
		// Ring 2
		{1.f, -5.f},
		{3.f, -3.f},
		{5.f, -1.f},
		{4.f, 1.f},
		{2.f, 3.f},
		{0.f, 5.f},
	};
	
	// Ring weights
	static const float4 g_f4SSAORingWeight[NUM_RING_2_GATHERS] = 
	{
		// Ring 0 (Sum = 5.30864)
		{1.00000f, 0.50000f, 0.44721f, 0.70711f},
		{0.50000f, 0.44721f, 0.70711f, 1.00000f},
		
		// Ring 1 (Sum = 6.08746)
		{0.30000f, 0.29104f, 0.37947f, 0.40000f},
		{0.42426f, 0.33282f, 0.37947f, 0.53666f},
		{0.40000f, 0.30000f, 0.29104f, 0.37947f},
		{0.53666f, 0.42426f, 0.33282f, 0.37947f},
		
		// Ring 2 (Sum = 6.53067)
		{0.31530f, 0.29069f, 0.24140f, 0.25495f},
		{0.36056f, 0.29069f, 0.26000f, 0.30641f},
		{0.26000f, 0.21667f, 0.21372f, 0.25495f},
		{0.29069f, 0.24140f, 0.25495f, 0.31530f},
		{0.29069f, 0.26000f, 0.30641f, 0.36056f},
		{0.21667f, 0.21372f, 0.25495f, 0.26000f},	
	};
	
	static const float g_fRingWeightsTotal[RING_2] =
	{
		5.30864f,
		11.39610f,
		17.92677f,
	};
	
	//-' OldSerpskiStalker, disabled:
	float4 GatherZSamples( float2 f2TexCoord )
	{
		float4 f4Ret;

		float2 f2InvRTSize = (1.f).xx / screen_res.xy;
		f4Ret = g_txDepth.GatherBlue( smp_nofilter, f2TexCoord );	
		f4Ret += g_fHDAOZDispScale * g_txNormal.GatherRed( smp_nofilter, f2TexCoord );
		  
		return f4Ret;
	}
	
	float HDAOValleyAngle( uint2 u2ScreenCoord )
	{
		float3 f3N1 = float3(0.f , 0.f, 0.f);
		float3 f3N2 = float3(0.f, 0.f, 0.f);
		float fDot;
		float fSummedDot = 0.f;
		int2 i2MirrorPattern;
		int2 i2OffsetScreenCoord;
		int2 i2MirrorOffsetScreenCoord;

		float3 N = gbuf_unpack_normal( g_txNormal.Load( int3( u2ScreenCoord, 0), 0 ).xy );

		for( int iNormal=0; iNormal<NUM_NORMAL_LOADS; iNormal++ )
		{
			i2MirrorPattern = g_i2HDAONormalLoadPattern[iNormal] * int2( -1, -1 );
			i2OffsetScreenCoord = u2ScreenCoord + g_i2HDAONormalLoadPattern[iNormal];
			i2MirrorOffsetScreenCoord = u2ScreenCoord + i2MirrorPattern;
			
			// Clamp our test to screen coordinates
			i2OffsetScreenCoord = ( i2OffsetScreenCoord > ( g_f2RTSize - float2( 1.0f, 1.0f ) ) ) ? ( g_f2RTSize - float2( 1.0f, 1.0f ) ) : ( i2OffsetScreenCoord );
			i2MirrorOffsetScreenCoord = ( i2MirrorOffsetScreenCoord > ( g_f2RTSize - float2( 1.0f, 1.0f ) ) ) ? ( g_f2RTSize - float2( 1.0f, 1.0f ) ) : ( i2MirrorOffsetScreenCoord );
			i2OffsetScreenCoord = ( i2OffsetScreenCoord < 0 ) ? ( 0 ) : ( i2OffsetScreenCoord );
			i2MirrorOffsetScreenCoord = ( i2MirrorOffsetScreenCoord < 0 ) ? ( 0 ) : ( i2MirrorOffsetScreenCoord );

			f3N1.xy  = g_txNormal.Load( int3( i2OffsetScreenCoord, 0), 0 ).xy;
			f3N1.xyz = gbuf_unpack_normal( f3N1.xy );
			f3N2.xy  = g_txNormal.Load( int3( i2MirrorOffsetScreenCoord, 0), 0 ).xy;				
			f3N2.xyz = gbuf_unpack_normal( f3N2.xy );
			
			fDot = dot( f3N1, N );
			
			fSummedDot += ( fDot > g_fSSAOAcceptAngle ) ? ( 0.0f ) : ( 1.0f - ( abs( fDot ) * 0.25f ) );

			fDot = dot( f3N2, N );
			
			fSummedDot += ( fDot > g_fSSAOAcceptAngle ) ? ( 0.0f ) : ( 1.0f - ( abs( fDot ) * 0.25f ) );
		}

		fSummedDot /= 8.0f;
		fSummedDot += 0.5f;
		fSummedDot = ( fSummedDot <= 0.5f ) ? ( fSummedDot / 10.0f ) : ( fSummedDot );
			
		return fSummedDot;
	}
#endif