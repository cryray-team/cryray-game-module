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

float4 main(p_screen I) : SV_Target
{
	const float GAUSS_SIGMA = 2.5f;
	const int support = int(GAUSS_SIGMA * 1.5f);
	
	//Get direction
	float2 dir = (0.f,0.f);
	if(blur_params.x == 1.f)
		dir = float2(1.f/blur_params.z, 0.f);
	else if(blur_params.y == 1.f)
		dir = float2(0.f, 1.f/blur_params.w);
		
	//Accumulation
	float3 image = (0.f, 0.f, 0.f);
	float norm = 0.f;
	
	//Blur loop
	[unroll]
	for (int i = -support; i <= support; i++) 
	{
		float coeff = exp(-0.5f * float(i) * float(i) / (GAUSS_SIGMA * GAUSS_SIGMA));
		image += s_image.SampleLevel(smp_nofilter, I.tc0 + float(i) * dir, 0).xyz * coeff;
		norm += coeff;
	}
	image *= 1.f/norm; 
    return float4(image, 1.f);
} 
