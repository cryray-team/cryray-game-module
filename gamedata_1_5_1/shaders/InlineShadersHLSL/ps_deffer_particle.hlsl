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
#include "Headers\h_sload.hlsl"

struct p_particle
{
    float4 color : COLOR0;
    p_flat base;
};

f_deffer main(p_particle II)
{
    f_deffer O;
    p_flat I;
    I = II.base;

    float4 D = s_base.Sample(smp_base, I.tcdh);
    D *= II.color;

    clip(D.w - def_aref.w);

    float4 Ne = float4(normalize((float3)I.N.xyz), I.position.w);

    O = pack_gbuffer( 
        Ne,
        float4(I.position.xyz + Ne.xyz * def_virtualh/2.f, xmaterial),
        float4(D.xyz, def_gloss)
    );

    return O;
}
