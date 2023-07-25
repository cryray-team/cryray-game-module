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

float4 blendSoftLight(float4 a, float4 b)
{
	float4 c = 2.f * a * b + a * a * (1.f - 2.f * b);
	float4 d = sqrt(a) * (2.f * b - 1.f) + 2 * a * (1.f - b);
	
	return (b < 0.5f) ? c : d;
}

float4 main(p_screen I) : SV_Target
{
	float4 	cScreen 	= s_image.Load(int3(I.tc0.xy * screen_res.xy, 0), 0);
	float4 	cSunShafts 	= s_sunshafts.Load(int3(I.tc0.xy * screen_res.xy, 0), 0);
	
	float 	fShaftsMask = saturate(1.00001f - cSunShafts.w) * ssss_params.y * 2;
	
	// normalize suncolor
	float4 	sunColor 	= float4(normalize(L_sun_color.xyz), 1);
	
	float4 	outColor 	= cScreen + cSunShafts.xyzz * ssss_params.x * sunColor * (1.f - cScreen);
			outColor 	= blendSoftLight(outColor, sunColor * fShaftsMask * 0.5f + 0.5f);
	
	return outColor;
}