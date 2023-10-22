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
v2p_build main ( v_build I )
{
	v2p_build O;
	{
		I.P.xy += 0.5f;	//	Bugs with rasterizer??? Possible float-pixel shift.

		O.HPos.x = I.P.x * screen_res.z * 2.f - 1.f;
		O.HPos.y = (I.P.y * screen_res.w * 2.f - 1.f)*-1.f;
		O.HPos.zw = I.P.zw;
	}

	O.Tex0 = I.Tex0;
	O.Tex1 = I.Tex1;
	O.Tex2 = I.Tex2;
	O.Tex3 = I.Tex3;

 	return O;
}