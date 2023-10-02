//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef HDAO_H_F_H
#define HDAO_H_F_H
	static float g_fHDAORejectRadius    = 0.43f;	// Camera Z values must fall within the reject and accept radius to be 
	static float g_fHDAOAcceptRadius    = 0.0001f;  // considered as a valley
	static float g_fHDAONormalScale     = 0.10f;	// Scaling factor to control the effect the normals have 
	static float g_fAcceptAngle         = 0.98f;	// Used by the ValleyAngle function to determine shallow valleys
	static float g_fHDAOIntensity       = 1.5f;	    // Simple scaling factor to control the intensity of the occlusion
	
	static float g_fWeightTotal = 18.4198f;
	
	// Used by the valley angle function
	static const int2 g_i2NormalLoadPattern[NUM_NORMAL_LOADS] = 
	{
		{0, -9}, {6, -6},
		{10, 0}, {8, 9},
	};
	
	static const int2 g_i2HDAOSamplePattern[NUM_VALLEYS] =
    {
		{0, -11}, {2, -10}, {0, -9}, {5, -9}, {2, -8}, {7, -8}, 
		{0, -7}, {5, -7}, {2, -6}, {7, -6}, {8, -6}, {0, -5}, 
		{5, -5}, {10, -5}, {2, -4}, {7, -4}, {0, -3}, {5, -3}, 
		{10, -3}, {2, -2}, {7, -2}, {0, -1}, {5, -1}, {10, -1}, 
		{2, 0}, {7, 0}, {5, 1}, {10, 1}, {2, 2}, {7, 2}, {5, 3}, 
		{10, 3}, {2, 4}, {7, 4}, {5, 5}, {10, 5}, {2, 6}, {7, 6}, 
		{5, 7}, {6, 7}, {10, 7}, {2, 8}, {7, 8}, {5, 9}, {2, 10}, 
		{7, 10}, {5, 11}, {2, 12},
    };
	
	static const float g_fHDAOSampleWeights[NUM_VALLEYS] = 
    {
	    0.1538f, 0.2155f, 0.3077f, 0.2080f, 0.3657f, 0.1823f, 0.4615f, 0.3383f, 
		0.5135f, 0.2908f, 0.2308f, 0.6154f, 0.4561f, 0.1400f, 0.6560f, 0.3798f, 
		0.7692f, 0.5515f, 0.1969f, 0.7824f, 0.4400f, 0.9231f, 0.6078f, 0.2269f, 
		0.8462f, 0.4615f, 0.6078f, 0.2269f, 0.7824f, 0.4400f, 0.5515f, 0.1969f, 
		0.6560f, 0.3798f, 0.4561f, 0.1400f, 0.5135f, 0.2908f, 0.3383f, 0.2908f, 
		0.0610f, 0.3657f, 0.1823f, 0.2080f, 0.2155f, 0.0610f, 0.0705f, 0.0642f,
    };
	
	float ValleyAngle( uint2 u2ScreenCoord )
	{
		float3 f3N1;
		float3 f3N2;
		float fDot;
		float fSummedDot = 0.0f;
		int2 i2MirrorPattern;
		int2 i2OffsetScreenCoord;
		int2 i2MirrorOffsetScreenCoord;
		
		float3 N = gbuf_unpack_normal( g_txNormal.Load( int3( u2ScreenCoord, 0), 0 ).xy );

		for(int iNormal=0; iNormal<NUM_NORMAL_LOADS; iNormal++)
		{
			i2MirrorPattern = g_i2NormalLoadPattern[iNormal] * int2(-1, -1);
			i2OffsetScreenCoord = u2ScreenCoord + g_i2NormalLoadPattern[iNormal];
			i2MirrorOffsetScreenCoord = u2ScreenCoord + i2MirrorPattern;
			
			// Clamp our test to screen coordinates
			i2OffsetScreenCoord = (i2OffsetScreenCoord > ( g_f2RTSize - float2(1.0f, 1.0f))) ? ( g_f2RTSize - float2(1.0f, 1.0f)) : (i2OffsetScreenCoord);
			i2MirrorOffsetScreenCoord = (i2MirrorOffsetScreenCoord > (g_f2RTSize - float2(1.0f, 1.0f))) ? ( g_f2RTSize - float2(1.0f, 1.0f)) : (i2MirrorOffsetScreenCoord);
			i2OffsetScreenCoord = (i2OffsetScreenCoord < 0) ? (0) : ( i2OffsetScreenCoord);
			i2MirrorOffsetScreenCoord = (i2MirrorOffsetScreenCoord < 0) ? (0) : (i2MirrorOffsetScreenCoord);
							
			f3N1.xy  = g_txNormal.Load(int3( i2OffsetScreenCoord, 0), 0).xy;
			f3N1.xyz = gbuf_unpack_normal( f3N1.xy );
			f3N2.xy  = g_txNormal.Load(int3( i2MirrorOffsetScreenCoord, 0), 0).xy;				
			f3N2.xyz = gbuf_unpack_normal( f3N2.xy );

			
			fDot = dot(f3N1, N);
			
			fSummedDot += (fDot > g_fAcceptAngle) ? (0.0f) : (1.0f - (abs( fDot) * 0.25f));

			fDot = dot(f3N2, N);
			
			fSummedDot += (fDot > g_fAcceptAngle ) ? (0.0f) : (1.0f - ( abs( fDot ) * 0.25f));
		}

		fSummedDot /= 8.0f;
		fSummedDot += 0.5f;
		fSummedDot = (fSummedDot <= 0.5f) ? (fSummedDot / 10.0f) : (fSummedDot);
			
		return fSummedDot;
	}
#endif