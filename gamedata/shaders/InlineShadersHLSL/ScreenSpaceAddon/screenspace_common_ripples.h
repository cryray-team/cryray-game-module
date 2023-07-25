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
 * @ Version: SCREEN SPACE SHADERS - UPDATE 12
 * @ Description: Ripples code
 * @ Modified time: 2022-10-29 05:05
 * @ Author: https://www.moddb.com/members/ascii1457
 * @ Mod: https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders
 */

static const float4 SSFX_ripples_timemul = float4(1.f, 0.85f, 0.93f, 1.13f); 
static const float4 SSFX_ripples_timeadd = float4(0.f, 0.2f, 0.45f, 0.7f);

// https://seblagarde.wordpress.com/2013/01/03/water-drop-2b-dynamic-rain-and-its-effects/
float3 SSFX_computeripple(float4 Ripple, float CurrentTime, float Weight)
{
	Ripple.yz = Ripple.yz * 2.f - 1.f; // Decompress perturbation

	float DropFrac = frac(Ripple.w + CurrentTime); // Apply time shift
	float TimeFrac = DropFrac - 1.f + Ripple.x;
	float DropFactor = saturate(0.2f + Weight * 0.8f - DropFrac);
	float FinalFactor = DropFactor * Ripple.x * 
						sin( clamp(TimeFrac * 9.f, 0.f, 3.f) * 3.14159265359f);

	return float3(Ripple.yz * FinalFactor * 0.65f, 1.f);
}

float3 SSFX_ripples( Texture2D ripple_tex, float2 tc ) 
{
	float4 Times = (timers.x * SSFX_ripples_timemul + SSFX_ripples_timeadd) * 1.5f;
	Times = frac(Times);

	float4 Weights = float4( 0.f, 1.f, 0.65f, 0.25f);

	// Compute ripples
	float3 Ripple1 = SSFX_computeripple(ripple_tex.Sample( smp_base, tc + float2( 0.25f,0.f)), Times.x, Weights.x);
	float3 Ripple2 = SSFX_computeripple(ripple_tex.Sample( smp_base, tc + float2(-0.55f,0.3f)), Times.y, Weights.y);
	float3 Ripple3 = SSFX_computeripple(ripple_tex.Sample( smp_base, tc + float2(0.6f, 0.85f)), Times.z, Weights.z);
	float3 Ripple4 = SSFX_computeripple(ripple_tex.Sample( smp_base, tc + float2(0.5f,-0.75f)), Times.w, Weights.w);
	
	Weights = saturate(Weights * 4.f);
	float4 Z = lerp(1, float4(Ripple1.z, Ripple2.z, Ripple3.z, Ripple4.z), Weights);
	float3 Normal = float3( Weights.x * Ripple1.xy +
							Weights.y * Ripple2.xy + 
							Weights.z * Ripple3.xy +
							Weights.w * Ripple4.xy, 
							Z.x * Z.y * Z.z * Z.w);
	
	return normalize(Normal) * 0.5f + 0.5f;
}