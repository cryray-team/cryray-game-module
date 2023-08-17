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

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main ( p_build I ) : SV_Target
{
    // hi-rgb.base-lum
	float3	s0	= s_image.Sample( smp_rtlinear, I.Tex0.xy).rgb;
	float3	s1	= s_image.Sample( smp_rtlinear, I.Tex1.xy).rgb;
	float3	s2	= s_image.Sample( smp_rtlinear, I.Tex2.xy).rgb;
	float3	s3	= s_image.Sample( smp_rtlinear, I.Tex3.xy).rgb;

	float3	avg	= ( (s0+s1) + (s2+s3) )/2.f;
	float	hi	= dot( avg, 1.f )-b_params.x	;	// assume def_hdr equal to 3.0

	return	float4( avg, hi );
}
