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

struct 	_in        	
{
	float4	P	: POSITIONT;	// xy=pos, zw=tc0
	float2	tcJ	: TEXCOORD0;	// jitter coords
};

struct 	v2p
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
	float2	tcJ	: TEXCOORD1;	// jitter coords
	float4	hpos: SV_Position;
};
//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p main ( _in I )
{
	v2p 		O;
	O.hpos 		= float4	(I.P.x, -I.P.y, 0.f, 1.f);
	float  	scale 	= s_tonemap.Load(int3(0.f,0.f,0.f)).x;
	O.tc0		= float4	(I.P.zw, scale, scale);
	O.tcJ		= I.tcJ;
 	return	O;
}

FXVS;
