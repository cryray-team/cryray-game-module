//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
p_screen main(v2p_screen I)
{
	p_screen O;
	// Transform to screen space (in d3d9 it was done automatically)
	O.hpos.x 	=  (I.HPos.x * screen_res.z * 2.f - 1.f);
	O.hpos.y 	= -(I.HPos.y * screen_res.w * 2.f - 1.f);
	O.hpos.zw 	=	I.HPos.zw;
	
    O.tc0 		= 	I.tc0;
  
    return O; 
}