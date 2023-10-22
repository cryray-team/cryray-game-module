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

struct 	a2v
{
	float4 P:	 	POSITION;	// Object-space position
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_shadow_direct main ( a2v I )
{
	v2p_shadow_direct	O;

	O.hpos 	= mul		(m_WVP,	I.P	);

 	return	O;
}
FXVS;
