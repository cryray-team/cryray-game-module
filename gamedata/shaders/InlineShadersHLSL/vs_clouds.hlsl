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
#include "Headers\cloudconfig.h"

struct vi
{
	float4 p : POSITION;
	float4 dir : COLOR0;   // dir0,dir1(w<->z)
	float4 color : COLOR1; // rgb. intensity
};

struct vf
{
	float4 color : COLOR0; // rgb. intensity, for SM3 - tonemap-prescaled, HI-res
	float3 tc0 : TEXCOORD0;
	float2 tc1 : TEXCOORD1;
	float4 hpos : SV_Position;
};

vf main(vi v)
{
	vf o;

	o.hpos = mul(m_WVP, v.p); // xform, input in world coords

	// generate tcs
	float2 d0 = v.dir.xy * 2.f - 1.f;
	float2 d1 = v.dir.wz * 2.f - 1.f;
	float2 _0 = v.p.xz * CLOUD_TILE0 + d0 * timers.z * CLOUD_SPEED0;
	float2 _1 = v.p.xz * CLOUD_TILE1 + d1 * timers.z * CLOUD_SPEED1;

	float scale = s_tonemap.Load(int3(0, 0, 0)).x;
	o.tc0 = float3(_0, scale); // copy tc
	o.tc1 = _1;				   // copy tc

	o.color = v.color; // copy color, low precision, cannot prescale even by 2
	o.color.w *= pow(abs(v.p.y), 25.f);

	return o;
}
