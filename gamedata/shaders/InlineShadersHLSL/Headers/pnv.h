//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

//defines
#define NV_BRIGHTNESS 5.0

//effects
#define NV_FLICKERING
#define NV_NOISE

//effect settings
#define FLICKERING_INTENSITY 0.0035
#define FLICKERING_FREQ 60.0
#define NOISE_INTENSITY 0.15

float4 calc_night_vision_effect(float2 tc0, float4 color, float3 NV_COLOR)
{
    float lum = dot(color.rgb, float3( 0.3f, 0.38f, 0.22f)*NV_BRIGHTNESS );  //instead of float3 use LUMINANCE_floatTOR in stalker
    color.rgb = NV_COLOR*lum;
    
    //cheap noise function
    float noise = frac(sin(dot(tc0, float2(12.0, 78.0) + (timers.x*1.17f) )) * 43758.0); 

    //////////////////////////////////////////////////////////////////////////////////////////
    // noise
    #ifdef NV_NOISE
    color  += noise * NOISE_INTENSITY; 
    #endif
    //////////////////////////////////////////////////////////////////////////////////////////
    // screen flickering
    #ifdef NV_FLICKERING
    color += FLICKERING_INTENSITY * sin(timers.x*FLICKERING_FREQ);   
    #endif
	
   	return color; 
}