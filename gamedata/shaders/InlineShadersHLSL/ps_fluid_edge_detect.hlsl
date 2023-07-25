//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\fluid_common_render.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
// A full-screen edge detection pass to locate artifacts
//	these artifacts are located on a downsized version of the rayDataTexture
// We use a smaller texture both to accurately find all the depth artifacts 
//  when raycasting to this smaller size and to save on the cost of this pass
// Use col.a to find depth edges of objects occluding the smoke
// Use col.g to find the edges where the camera near plane cuts the smoke volume
//
float4 main(VS_OUTPUT_EDGE vIn) : SV_Target
{

    // We need eight samples (the centre has zero weight in both kernels).
    float4 col;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV00); 
    float g00 = col.a;
    if(col.g < 0.f)
        g00 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV01); 
    float g01 = col.a;
    if(col.g < 0.f)
        g01 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV02); 
    float g02 = col.a;
    if(col.g < 0.f)
        g02 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV10); 
    float g10 = col.a;
    if(col.g < 0.f)
        g10 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV12); 
    float g12 = col.a;
    if(col.g < 0.f)
        g12 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV20); 
    float g20 = col.a;
    if(col.g < 0.f)
        g20 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV21); 
    float g21 = col.a;
    if(col.g < 0.f)
        g21 *= -1.f;
    col = rayDataTexSmall.Sample(samPointClamp, vIn.textureUV22); 
    float g22 = col.a;
    if(col.g < 0.f)
        g22 *= -1.f;
    	
    // Sobel in horizontal dir.
    float sx = 0.f;
    sx -= g00;
    sx -= g01 * 2.f;
    sx -= g02;
    sx += g20;
    sx += g21 * 2.f;
    sx += g22;
    // Sobel in vertical dir - weights are just rotated 90 degrees.
    float sy = 0.f;
    sy -= g00;
    sy += g02;
    sy -= g10 * 2.f;
    sy += g12 * 2.f;
    sy -= g20;
    sy += g22;

    float e = EdgeDetectScalar(sx, sy, edgeThreshold);
    return float4(e,e,e,1.f);

}