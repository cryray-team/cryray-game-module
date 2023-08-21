//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "DX10Fog\h_fluid_common.hlsl"

float GravityBuoyancy;

//////////////////////////////////////////////////////////////////////////////////////////
//	Pixel
float4 main( p_fluidsim input ) : SV_Target
{
    float3 npos = GetAdvectedPosTexCoords(input);

	float4 Velocity = Texture_velocity0.SampleLevel( samLinear, npos, 0) * modulate;

#ifdef	USE_GRAVITY

	float Dencity = Texture_color.SampleLevel( samLinear, npos, 0);
	Velocity.y += Dencity * GravityBuoyancy;

#endif	//	USE_GRAVITY

	return Velocity;
}