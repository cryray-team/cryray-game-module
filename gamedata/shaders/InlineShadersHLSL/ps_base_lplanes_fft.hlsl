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
 	float2 tc0		: TEXCOORD0;	// base
  	float4 c0		: COLOR0;		// sun
	float4 pos2d	: SV_Position;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel

float4 main( v2p I ) : SV_Target
{
	//Texcoordinates of screen
	float2 screen_tc = I.pos2d.xy / screen_res.xy;

	//Get base texture size
	uint width, height;
	s_base.GetDimensions(width, height);
	float2 tex_res = float2(width, height);
	
	//Get mark texcoord
    float2 mark_tc = screen_tc - float2(0.5,0.5);
    mark_tc *= (screen_res.xy / tex_res.xy) * (screen_res.x / screen_res.y);
 	mark_tc.y += 0.5;
 	mark_tc.x += 0.5;    

	//Sample mark texture
	float4 t_base = s_base.Sample(smp_rtlinear, mark_tc);
	
	//Output
	return float4(t_base.rgb, (t_base.a * I.c0.a));
}