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
#include "Headers\wmark.h"

struct vf
{
	float2 tc0	: TEXCOORD0;
	float3 c0	: COLOR0;		// c0=all lighting
	float  fog	: FOG;
	float4 hpos	: SV_Position;
};

vf main (v_static v)
{
	vf 		o;

	float3 	N 	= 	unpack_normal	(v.Nh);
	float4 	P 	= 	wmark_shift	(v.P,N);
	o.hpos 		= 	mul		(m_VP, P);			// xform, input in world coords
	o.tc0		= 	unpack_tc_base	(v.tc,v.T.w,v.B.w);		// copy tc

	o.c0		= 0.f;	//L_final;
	o.fog 		= saturate(calc_fogging 		(v.P));				// fog, input in world coords

	return o;
}
