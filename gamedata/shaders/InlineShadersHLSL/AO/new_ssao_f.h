//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef SSAO_F_H
#define SSAO_F_H
	static const float SSAO_RADIUS  = 1.2f; //-' Радиус найденного окклюдера
	
#if SSAO_QUALITY == 3
	static const float SSAO_CONTRAST = 2.4f; //-' Контраст общего эффекта АО
#elif SSAO_QUALITY == 2
	static const float SSAO_CONTRAST = 1.8f; //-' Контраст общего эффекта АО
#elif SSAO_QUALITY == 1
	static const float SSAO_CONTRAST = 1.5f; //-' Контраст общего эффекта АО
#endif

	float3 GetViewPos(float2 uv, float2 invFocalLen)
	{
		float3 p;
		p.z		= get_depth_fast(uv);
		p.xy	= (uv * float2(2.f, -2.f) + float2(-1.f, 1.f)) * invFocalLen * p.zz;
		return p;
	}

	float doPBAO(float2 uv, float3 pos, float3 n, float invRad, float bias, float2 invFocalLen, float selfOcc)
	{
		float3 p	= GetViewPos(uv, invFocalLen);
		float3 dist	= p - pos;
		float3 v	= normalize(dist);
		float atten	= length(dist) * invRad;
		return max(-selfOcc, dot( n, v) - bias) * (1.f / (atten * atten + 1.f));
	}

	float3 mirror(float3 vDir, float3 vPlane) 
	{
		return vDir - 2.f * vPlane * dot(vPlane,vDir);
	}
#endif