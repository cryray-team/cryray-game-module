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

v2p_flat 	main (v_detail v)
{
	v2p_flat 		O;
	// index
	int 	i 	= v.misc.w;
	float4  m0 	= array[i+0];
	float4  m1 	= array[i+1];
	float4  m2 	= array[i+2];
	float4  c0 	= array[i+3];

	// Transform pos to world coords
	float4 	pos;
 	pos.x 		= dot	(m0, v.pos);
 	pos.y 		= dot	(m1, v.pos);
 	pos.z 		= dot	(m2, v.pos);
	pos.w 		= 1;

	// Normal in world coords
	float3 	norm;	
		norm.x 	= pos.x - m0.w	;
		norm.y 	= pos.y - m1.w	+ 0.75f;	// avoid zero
		norm.z	= pos.z - m2.w	;

	// Final out
	float4	Pp 	= mul		(m_WVP,	pos				);
	O.hpos 		= Pp;
	float3 orig		= mul		(m_WV,  normalize(norm)	);

	O.N = lerp(orig, mul((float3x3)m_WV,  v.pos), 0.25f);
	float3	Pe	= mul		(m_WV,  pos				);
	O.tcdh 		= float4	((v.misc * consts).xyyy	);

	O.tcdh.z = 1.f;

	O.position	= float4	(Pe, 		c0.w		);

	return O;
}
FXVS;
