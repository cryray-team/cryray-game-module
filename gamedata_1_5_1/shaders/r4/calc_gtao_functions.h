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
		float depth = s_position.Sample(smp_nofilter, tc).z;

		return gbuf_unpack_position((depth > 0.01f ? depth : 1000.f), tc * pos_decompression_params2.xy);
	}
#endif