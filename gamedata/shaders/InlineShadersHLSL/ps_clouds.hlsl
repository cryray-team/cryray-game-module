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

struct 	v2p
{
	float4 color : COLOR0;  // rgb. intensity, for SM3 - tonemap prescaled
	float3 tc0 : TEXCOORD0; // .z - tonemap scale
	float2 tc1 : TEXCOORD1;
};

Texture2D	s_clouds0	: register(t0);
Texture2D	s_clouds1	: register(t1);
	
//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 	main	( v2p I )	: SV_Target
{
	float4 s0 = s_clouds0.Sample(smp_base, I.tc0.xy);
	float4 s1 = s_clouds1.Sample(smp_base, I.tc1.xy);

	float4 final = I.color * (s0 + s1);
		
	final.rgb = SkyLuminanceScale(I.tc0.z * final.rgb);
	
	return final;
}
