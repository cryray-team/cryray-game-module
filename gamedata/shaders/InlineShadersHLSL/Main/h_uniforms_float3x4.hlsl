//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef UNIFORMS3X4_H
#define UNIFORMS3X4_H
	uniform float3x4 m_v2w;
	uniform float3x4 m_sunmask;	// ortho-projection
	uniform float3x4 m_xform;
	uniform float3x4 m_xform_v;
	uniform float3x4 m_invW;
#endif