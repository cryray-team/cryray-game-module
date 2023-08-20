//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include 	"Headers\common.h"

struct 		vv
{
	float4	P		: POSITION;
	float2	tc		: TEXCOORD0;
	float4	c		: COLOR0;
};

struct 		v2p_particle
{
	float4 		color	: COLOR0;
	v2p_flat	base;
};

v2p_particle main( vv I )
{
	float4 	w_pos 	= I.P;

	// Eye-space pos/normal
	v2p_flat 		O;
	O.hpos 		= mul		(m_WVP,		w_pos	);
	O.N 		= normalize (eye_position-w_pos	);
	float3	Pe	= mul		(m_WV, 		I.P		);
	O.tcdh 		= float4	(I.tc.xyyy			);
	O.position	= float4	(Pe, 		0.2f		);

#ifdef 	USE_TDETAIL
	O.tcdbump	= O.tcdh * dt_params;			// dt tc
#endif

	v2p_particle	pp;
	pp.color = I.c;
	pp.base = O;

	return		pp;
}
FXVS;
