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

struct vf
{
	float4 C	: COLOR0;
	float4 P	: POSITION;
};

struct v2p
{
	float4 C	: COLOR0;
	float4 P	: SV_Position;
};

uniform float4 		tfactor;
v2p main (vf i)
{
	v2p 		o;

	o.P 		= mul			(m_WVP, i.P);			// xform, input in world coords
	o.C 		= tfactor*i.C;

	return o;
}
