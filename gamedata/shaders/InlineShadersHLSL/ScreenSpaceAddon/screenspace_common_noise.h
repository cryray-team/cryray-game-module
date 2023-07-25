//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

/**
 * @ Version: SCREEN SPACE SHADERS - UPDATE 12.5
 * @ Description: Main file - Noise
 * @ Modified time: 2022-11-23 14:22
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

float4 SSFX_mod289(float4 x) 
{ 
	return x - floor(x / 289.f) * 289.f; 
}

float4 SSFX_permute(float4 x) 
{ 
	return SSFX_mod289((x * 34.f + 1.f) * x); 
}

float SSFX_noise(float2 tc)
{
	float2	noise = frac(tc.xy * 0.5f);
	return noise.x + noise.y * 0.5f;
}

float2 SSFX_noise2(float2 p)
{
	p = p % 289.f;
	float x = (34 * p.x + 1) * p.x % 289.f + p.y;
	x = (34.f * x + 1.f) * x % 289.f;
	x = frac(x / 41.f) * 2.f - 1.f;
	return normalize(float2(x - floor(x + 0.5f), abs(x) - 0.5f));
}

float SSFX_gradient_noise_IGN(float2 uv)
{
	// IGN
	uv = frac(uv);
	float f = 0.06711056f * uv.x + 0.00583715f * uv.y;
	return frac(52.9829189f * frac(f));
}

float SSFX_gradientNoise(float2 p)
{
	float2 ip = floor(p);
	float2 fp = frac(p);
	float d00 = dot(SSFX_noise2(ip), fp);
	float d01 = dot(SSFX_noise2(ip + float2(0.f, 1.f)), fp - float2(0.f, 1.f));
	float d10 = dot(SSFX_noise2(ip + float2(1.f, 0.f)), fp - float2(1.f, 0.f));
	float d11 = dot(SSFX_noise2(ip + float2(1.f, 1.f)), fp - float2(1.f, 1.f));
	fp = fp * fp * fp * (fp * (fp * 6.f - 15.f) + 10.f);
	return lerp(lerp(d00, d01, fp.y), lerp(d10, d11, fp.y), fp.x);
}