//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	FLOAT_FUNC_H
#define	FLOAT_FUNC_H
	float get_depth_fast(float2 tc)
	{
	#ifndef MSAA_ANTIALIASING_ENABLE
		return s_position.Sample(smp_nofilter, tc).z;
	#else
		return s_position.Load(int3(tc * pos_decompression_params2.xy, 0), 0).z;
	#endif
	}
	float is_in_range(float3 args)
	{
		float mn = (args.x > args.y) ? 1.f : 0.f;
		float mx = (args.z > args.x) ? 1.f : 0.f;
		return mn*mx;
	}
#endif