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

// Vertex
v2p_volume main ( float4 P: POSITION )
{
	v2p_volume	O;
	O.hpos 		= mul( m_WVP, P );
	O.tc 		= mul( m_texgen, P );

 	return	O;
}
FXVS;
