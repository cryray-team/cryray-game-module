//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef HBAO_F_H
#define HBAO_F_H
	#define M_PI 3.1415926f
	#define g_Resolution screen_res.xy
	#define g_InvResolution screen_res.zw

#if SSAO_QUALITY == 3
	static const float kernel = 1500.f;
	static const float g_Contrast = 4.8f;
#elif SSAO_QUALITY == 2
	static const float kernel = 1300.f;
	static const float g_Contrast = 3.8f;
#elif SSAO_QUALITY == 1
	static const float kernel = 1200.f;
	static const float g_Contrast = 2.8f;
#endif

	static const float g_NumDir = 32.f;
	static const float g_NumSteps = 16.f;
	static const float g_MaxFootprintUV = 0.01f;
	static const float g_sqr_R = 0.160007462f;
	static const float g_inv_R = 2.49994159f;

	float falloff(float r)
	{
		return 1.f - r*r;
	}

	float length2(float3 v) 
	{ 
		return dot(v, v); 
	} 

	float tangent(float3 T)
	{
		return -T.z / length(T.xy);
	}

	float tangent(float3 P, float3 S)
	{
		return (P.z - S.z) / length(S.xy - P.xy);
	}
	
	float2 snap_uv_offset(float2 uv)
	{
		return round(uv * g_Resolution) * g_InvResolution;
	}
	
	float3 min_diff(float3 P, float3 Pr, float3 Pl)
	{
		float3 V1 = Pr - P;
		float3 V2 = P - Pl;
		return (length2(V1) < length2(V2)) ? V1 : V2;
	}

	float3 uv_to_eye(float2 uv, float eye_z)
	{
		uv = (uv * float2(2.f, 2.f) - float2(1.f, 1.f));
		return float3(uv * pos_decompression_params.xy * eye_z, eye_z);
	}

	float3 fetch_eye_pos(float2 uv)
	{
		float z = s_half_depth.SampleLevel(smp_nofilter, uv, 0);
		return uv_to_eye(uv, z);
	}

	float3 tangent_eye_pos(float2 uv, float4 tangentPlane)
	{
		// view vector going through the surface point at uv
		float3 V = fetch_eye_pos(uv);
		float NdotV = dot(tangentPlane.xyz, V);
		// intersect with tangent plane except for silhouette edges
		if (NdotV < 0.f) 
			V *= (tangentPlane.w / NdotV);
		
		return V;
	}
#endif