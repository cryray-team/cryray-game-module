//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	TONEMAPPING_H
#define	TONEMAPPING_H

uniform float tnmp_a;
uniform float tnmp_b;
uniform float tnmp_c;
uniform float tnmp_d;
uniform float tnmp_e;
uniform float tnmp_f;	
uniform float tnmp_w;
uniform float tnmp_exposure;
uniform float tnmp_gamma;
uniform float tnmp_onoff;

float3 Uncharted2ToneMapping(float3 color)
{

    color *= tnmp_exposure;
    color = ((color * (tnmp_a * color + tnmp_c * tnmp_b) + tnmp_d * tnmp_e) / (color * (tnmp_a * color + tnmp_b) + tnmp_d * tnmp_f)) - tnmp_e / tnmp_f;
    float white = ((tnmp_w * (tnmp_a * tnmp_w + tnmp_c * tnmp_b) + tnmp_d * tnmp_e) / (tnmp_w * (tnmp_a * tnmp_w + tnmp_b) + tnmp_d * tnmp_f)) - tnmp_e / tnmp_f;
    color /= white;
    color = pow(color, (1.f / tnmp_gamma));
    return color;
}
#endif