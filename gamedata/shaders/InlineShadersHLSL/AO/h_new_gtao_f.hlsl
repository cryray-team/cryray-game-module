//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef GTAO_F_H
#define GTAO_F_H	
	#if SSAO_QUALITY == 3
		#define GTAO_SAMPLE 4
	#elif SSAO_QUALITY == 2
		#define GTAO_SAMPLE 3
	#elif SSAO_QUALITY == 1
		#define GTAO_SAMPLE 2
	#endif
	
	#define GTAO_SLICES 4
	#define GTAO_RADIUS 4
	#define GTAO_TC_MUL 1.f
	
	float GTAO_Offsets(float2 uv)
	{
		int2 position = (int2)(uv * pos_decompression_params2.xy / GTAO_TC_MUL);
		return 0.25f * (float)((position.y - position.x) & 3);
	}

	float GTAO_Noise(float2 position)
	{
		return frac(52.9829189f * frac(dot(position, float2( 0.06711056f, 0.00583715f))));
	}
	
	float3 gbuf_unpack_position(float z, float2 pos2d)
	{
		return float3(z * (pos2d * pos_decompression_params.zw - pos_decompression_params.xy), z);
	}

	float3 unpack_position(float2 tc)
	{
	#ifndef MSAA_ANTIALIASING_ENABLE
		float depth = s_position.Sample(smp_nofilter, tc).z;
	#else
		float depth = s_position.Load(int3(tc * pos_decompression_params2.xy , 0), 0).z;
	#endif

		return gbuf_unpack_position((depth > 0.01f ? depth : 1000.f), tc * pos_decompression_params2.xy);
	}
#endif