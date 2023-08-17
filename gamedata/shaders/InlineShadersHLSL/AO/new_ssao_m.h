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
float calc_ssao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d)
#else
float calc_ssao(float3 P, float3 N, float2 tc, float2 tcJ, float4 pos2d, uint iSample)
#endif
{
    // define kernel
    float n = 0.f;
    const float step = 0.875f;
    const float fScale = 0.025f;

    const float DEG2RAD = 0.017453292519943295769236907685f;
    const float twopi = 6.283185307179586476925286766559f;
    const float invPi = 0.318309886183790671537767526745f;
    const float inv2 = 0.5f;
    const float inv5_3 = 0.188679245283f;
    const float inv8 = 0.125f;
    const float inv16 = 0.0625f;
    const float selfOcc = 0.f;    // range: 0.0f to 1.0f
    const float hFov = 30.f * DEG2RAD;
    const float2 invRes = 1.f / pos_decompression_params2.xy;
    const float aspect = pos_decompression_params2.y / pos_decompression_params2.x;
    const float2 invFocalLen = float2(tan(hFov), tan(hFov) * aspect);
    const float2 focalLen = 1.f / invFocalLen;

    const float3 arrKernel[8] =
    {
        normalize(float3(1.f, 1.f, 1.f)) * fScale * (n += step),
        normalize(float3(-1.f, -1.f, -1.f)) * fScale * (n += step),
        normalize(float3(-1.f, -1.f, 1.f)) * fScale * (n += step),
        normalize(float3(-1.f, 1.f, -1.f)) * fScale * (n += step),
        normalize(float3(-1.f, 1.f, 1.f)) * fScale * (n += step),
        normalize(float3(1.f, -1.f, -1.f)) * fScale * (n += step),
        normalize(float3(1.f, -1.f, 1.f)) * fScale * (n += step),
        normalize(float3(1.f, 1.f, -1.f)) * fScale * (n += step),
    };
    // create random rot matrix
    float3 rotSample = jitter0.Load(int3((int)pos2d.x & 63, (int)pos2d.y & 63, 0)).xyz;
    rotSample = normalize(rotSample) * 0.5f;

    // get pixel position and normal
    // get pixel position and normal
#ifdef OVERRIDE_MSAA_ISAMPLE
    gbuffer_data gbd = gbuffer_load_data(GLD_P(tc.xy, pos2d.xy, 0));
#else
    gbuffer_data gbd = gbuffer_load_data(GLD_P(tc.xy, pos2d.xy, ISAMPLE));
#endif
    float3 pos = GetViewPos(tc.xy, invFocalLen);

    [flatten] if (pos.z <= EPSDEPTH)
        pos.z = EPSDEPTH;

    float3 normal = normalize(gbd.N.xyz);

    // calculate angle bias
    float bias = sin(1.f * DEG2RAD);

    // calculate contrast
    float contrast = inv16 / (1.f - saturate(bias)) * SSAO_CONTRAST;

    // calculate radius
    float radius = SSAO_RADIUS * saturate(pos.z * inv5_3) * (1.f + pos.z * inv8);
    float invRad = 1.f / radius;
    float2 radius2D = radius * focalLen / pos.z;
    float ao = 0.f;

    // calculate ao
    for (int i = 0; i < 8; i++)
    {
        float2 deltaUV = mirror(arrKernel[i].xxx, rotSample.xyz).xy * radius2D.xy;
        ao += doPBAO(tc.xy + deltaUV, pos, normal, invRad, bias, invFocalLen, selfOcc);
        ao += doPBAO(tc.xy + deltaUV * inv2, pos, normal, invRad, bias, invFocalLen, selfOcc);
    }

    return (1.f - (ao * contrast + selfOcc));
}

