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
 	float2 	tc0: TEXCOORD0;	//main tc for screen
  	float4	c0: COLOR0;		//unused shit mb vertex shader idk
};


//intensity and frequency of flickering
#define SCREEN_FLICKERING float2(0.0255f,42.f)

//clamping for scale of glitchness
#define SCREEN_GLITCH_LIMIT float2(0.1f, 0.40f)

//thats a "offset" of channels etc
#define SCREEN_RGB_TCX float3(0.1f,0.2f,0.3f)
#define SCREEN_RGB_TCY float3(0.4f,0.3f,0.2f)

//multipliers for intensity etc
#define SCREEN_RGB_RAND float3(0.33f,0.66f,0.99f)

//just multiplier for rand intensity
#define SCREEN_RAND_TIME float(1.f)

float3 glitch(float3 screen, float2 tc)
{
    if ((tc.x - rand(timers.x*SCREEN_RAND_TIME) * noise(tc) ) < clamp(SCREEN_GLITCH_LIMIT.x,sin(4.*noise(tc)),SCREEN_GLITCH_LIMIT.y)) 
    {
        float randori_first = rand(sin(timers.x*SCREEN_RGB_RAND.x));
        screen.x *= s_pdascreen.Sample(smp_base, float2(tc.x-SCREEN_RGB_TCX.x, tc.y+SCREEN_RGB_TCY.x)).z*randori_first;
        
        float randori_second = rand(sin(timers.x*SCREEN_RGB_RAND.y));
        screen.y *= s_pdascreen.Sample(smp_base, float2(tc.x-SCREEN_RGB_TCX.y, tc.y+SCREEN_RGB_TCY.y)).y*randori_second;
        
        float randori_third = rand(sin(timers.x*SCREEN_RGB_RAND.z));
        screen.z *= s_pdascreen.Sample(smp_base, float2(tc.x-SCREEN_RGB_TCX.z, tc.y+SCREEN_RGB_TCY.z)).x*randori_third;
        
        screen += SCREEN_FLICKERING.x * sin(timers.x*SCREEN_FLICKERING.y);    
    }

	return screen;
} 

float4 main( v2p I ): SV_Target
{
	float4 col = s_pdascreen.Sample(smp_base, I.tc0);
	col.xyz = lerp(col.xyz, glitch(col.xyz, I.tc0), pda_params.y);
	return float4(col.r, col.g, col.b, pda_params.x)*pda_params.z;
}