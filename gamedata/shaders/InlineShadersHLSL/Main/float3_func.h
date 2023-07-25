//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	FLOAT3_FUNC_H
#define	FLOAT3_FUNC_H
	float3 visor_reflect(float3 color, float2 tc)
	{
		float3 final = 0.f;
		float2 center = float2(0.5f, 0.5f);

		float ratio = pos_decompression_params2.y / pos_decompression_params2.x;

		float x = length(float2(tc.x, (tc.y - 0.5f) * ratio + 0.5f) - center);

		static const float y = 12.f;
		static const float t = 0.37f;

		if (x >= t)
		{
			float p = saturate((x / t - 1.f) * 2.f);

			[unroll]
			for (int i = 0; i < y; i++)
			{
				float N = 1.f - p / 0.8f + 0.15f * (i / (y - 1.f)) * p;
				float2 m = (center - tc) * -N + center;
				float3 visor = s_image.Sample(smp_rtlinear, 1 - m).rgb;
				final += visor;
			}

			final /= y;
			final *= 1.f - saturate((x - t - 0.05f) * 5.2f);

			return (color + final * 0.45f) / (1.f + 0.45f);
		}
		else
			return color;
	};
	
	float3 pp_vibrance(float3 image, float val)
	{
		float luminance = dot(image, LUMINANCE_VECTOR );
		return lerp( luminance.xxx, image, val );
	}
	
	float3 pp_nightvision(float3 img, float2 tc)
	{
		if(nv_color.w <= 1.f)
			return img;

		float noise  = frac(sin(dot(tc, float2(12.f, 78.f) + (timers.x*3.17f) )) * 43758.f); 
		  
		//fast color remover
		img.rgb = dot(img, LUMINANCE_VECTOR).xxx*saturate(nv_color.xyz);
		img.rgb *= nv_color.w;
		//////////////////////////////////////////////////////////////////////////////////////////
		// vignette
		img += nv_postprocessing.z * sin(tc.y*timers.x);
		img += nv_postprocessing.y * sin(timers.x*50.f); 
		img += noise*nv_postprocessing.x;

		img *= nv_postprocessing.w-(distance(tc.xy,float2( 0.5f, 0.5f)))*1.5f;   
		return img.xyz; 
	}
	
	float3 SkyLuminanceScale(float3 sky)
	{
		return sky * 2.f / 3.f;
	}
#endif