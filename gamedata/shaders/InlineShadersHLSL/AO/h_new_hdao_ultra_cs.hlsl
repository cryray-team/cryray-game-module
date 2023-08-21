//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

// CS Output buffers
RWTexture2D<float> g_ResultTexture : register( u0 );

//=============================================================================================================================
// Group shared memory (LDS)
//=============================================================================================================================
groupshared struct
{
	float fCameraZ[GROUP_TEXEL_DIM][GROUP_TEXEL_DIM];
} g_LDS;

//=============================================================================================================================
// Helper function to load data from the LDS, given texel coord
// NOTE: X and Y are swapped around to ensure horizonatal reading across threads, this avoids
// LDS memory bank conflicts
//=============================================================================================================================
float LoadFromLDS( uint2 u2Texel )
{
	return g_LDS.fCameraZ[u2Texel.y][u2Texel.x];
}

//=============================================================================================================================
// Helper function to store data to the LDS, given texel coord
// NOTE: X and Y are swapped around to ensure horizonatal wrting across threads, this avoids
// LDS memory bank conflicts
//=============================================================================================================================
void StoreToLDS( float fValue, uint2 u2Texel )
{
	g_LDS.fCameraZ[u2Texel.y][u2Texel.x] = fValue;
}

float ComputeHDAO(uint2 u2CenterTexel, uint2 u2ScreenPos)
{
    // Locals
    float fCenterZ;
    float2 f2SamplePos;
    float2 f2MirrorSamplePos;
    float fOcclusion = 0.0f;
    float2 f2SampledZ;
    float2 f2Diff;
    float2 f2Compare;
    float fDot;

    // Get the general valley angle, to scale the result by
    fDot = ValleyAngle(u2ScreenPos);

    // Sample center texel
    fCenterZ = LoadFromLDS(u2CenterTexel);

    // Loop through each valley
    [unroll]
    for (uint uValley = 0; uValley < NUM_VALLEYS; uValley++)
    {
        // Sample
        f2SampledZ.x = LoadFromLDS(u2CenterTexel + g_i2HDAOSamplePattern[uValley]);
        f2SampledZ.y = LoadFromLDS(u2CenterTexel - g_i2HDAOSamplePattern[uValley]);

        // Valley detect
        f2Diff = fCenterZ.xx - f2SampledZ;
        f2Compare = (f2Diff < g_fHDAORejectRadius.xx) ? (1.0f) : (0.0f);
        f2Compare *= (f2Diff > g_fHDAOAcceptRadius.xx) ? (1.0f) : (0.0f);

        // Weight occlusion
        fOcclusion += (f2Compare.x * f2Compare.y * g_fHDAOSampleWeights[uValley]);
    }

    // Finally calculate the HDAO occlusion value
    fOcclusion /= g_fWeightTotal;
    fOcclusion *= g_fHDAOIntensity * fDot;
    fOcclusion *= fCenterZ < 0.5f ? 0.0f : lerp(0.0f, 1.0f, saturate(fCenterZ - 0.5f));
    fOcclusion = 1.0f - saturate(fOcclusion);

    return fOcclusion;
}

//=============================================================================================================================
// HDAO CS: Performs valley detection in Camera Z space, and offsets by the Z 
// component of the camera space normal
//=============================================================================================================================
[numthreads(GROUP_THREAD_DIM, GROUP_THREAD_DIM, 1)]
void main(uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{ 
    // Locals
    float2 f2ScreenCoord;
    float2 f2Coord;
    float2 f2InvTextureSize = 1.0f / g_f2RTSize;
    float4 f4Depth;
    float4 f4Normal;
    float4 f4LDSValue;
    uint uColumn, uRow;

    if (GI < GATHER_THREADS)
    {
        // Get the screen position for this threads TEX ops
        uColumn = (GI % GATHER_THREADS_PER_ROW)  * GATHER_PER_THREAD * 2;
        uRow = (GI / GATHER_THREADS_PER_ROW) * 2;
        f2ScreenCoord = float2((float2(Gid.x, Gid.y) * float2(ALU_DIM, ALU_DIM)) - float2(GROUP_TEXEL_OVERLAP, GROUP_TEXEL_OVERLAP)) + float2(uColumn, uRow);
        
        #ifndef MSAA_ANTIALIASING_ENABLE
            
            // Offset for the use of gather4
            f2ScreenCoord += float2(1.0f, 1.0f); 

        #endif
    
        // Gather from input textures and lay down in the LDS
        [unroll] 
        for (uint uGather = 0; uGather < GATHER_PER_THREAD; uGather++)
        {
            #ifdef MSAA_ANTIALIASING_ENABLE

                f2Coord = float2(f2ScreenCoord.x + float(uGather * 2), f2ScreenCoord.y);

                f4Depth.x = g_txDepth.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(0, 1, 0)).z;
                f4Depth.y = g_txDepth.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(1, 1, 0)).z;
                f4Depth.z = g_txDepth.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(1, 0, 0)).z;
                f4Depth.w = g_txDepth.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(0, 0, 0)).z;

                f4Normal.x = g_txNormal.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(0, 1, 0)).x;
                f4Normal.y = g_txNormal.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(1, 1, 0)).x;
                f4Normal.z = g_txNormal.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(1, 0, 0)).x;
                f4Normal.w = g_txNormal.Load(int3(f2Coord, 0), MSAA_SAMPLE_INDEX, int3(0, 0, 0)).x;
            #else
                f2Coord = float2(f2ScreenCoord.x + float(uGather * 2), f2ScreenCoord.y) * f2InvTextureSize;
                f4Depth = g_txDepth.GatherBlue(g_SamplePoint, f2Coord);
                f4Normal = g_txNormal.GatherRed(g_SamplePoint, f2Coord);
            #endif

            f4LDSValue = f4Depth + (f4Normal * g_fHDAONormalScale.xxxx);

            StoreToLDS(f4LDSValue.x, uint2(uColumn + (uGather * 2) + 0, uRow + 1));
            StoreToLDS(f4LDSValue.y, uint2(uColumn + (uGather * 2) + 1, uRow + 1));
            StoreToLDS(f4LDSValue.z, uint2(uColumn + (uGather * 2) + 1, uRow + 0));
            StoreToLDS(f4LDSValue.w, uint2(uColumn + (uGather * 2) + 0, uRow + 0));
        }
    }

    // Enforce a group barrier with sync
    GroupMemoryBarrierWithGroupSync();

    // Calculate the screen pos
    uint2 u2ScreenPos = uint2(Gid.x * ALU_DIM + GTid.x, Gid.y * ALU_DIM + GTid.y);

    // Make sure we don't write outside the target buffer
    if ((u2ScreenPos.x < uint(g_f2RTSize.x)) && (u2ScreenPos.y < uint(g_f2RTSize.y)))
    {
        // Write the data directly to an AO texture:
        g_ResultTexture[u2ScreenPos.xy] = ComputeHDAO(uint2(GTid.x + GROUP_TEXEL_OVERLAP, GTid.y + GROUP_TEXEL_OVERLAP), u2ScreenPos);
    }
}
