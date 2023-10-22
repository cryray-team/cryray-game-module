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
	float3 SkyLuminanceScale(float3 sky)
	{
		return sky * 2.f / 3.f;
	}
	
	float3 pp_vibrance(float3 image, float val)
	{
		float luminance = dot(image, LUMINANCE_VECTOR );
		return lerp( luminance.xxx, image, val );
	}
	
	float3 pp_nightvision_combine_2(float3 img, float2 tc)
	{
		if(nv_color.w <= 1.f)
			return img;

		float noise  = frac(sin(dot(tc, float2(12.f, 78.f) + (timers.x*3.17f) )) * 43758.f); 
		  
		// fast color remover
		img.rgb = dot(img, LUMINANCE_VECTOR).xxx*saturate(nv_color.xyz);
		img.rgb *= nv_color.w;

		// vignette
		img += nv_postprocessing.z * sin(tc.y*timers.x);
		img += nv_postprocessing.y * sin(timers.x*50.f); 
		img += noise*nv_postprocessing.x;

		img *= nv_postprocessing.w-(distance(tc.xy,float2( 0.5f, 0.5f)))*1.5f;   
		return img.xyz;
	}
	
	float3 visor_reflection(float3 image, float2 tc)
	{
		float4 final = float4(0.f, 0.f, 0.f, 0.f);
		float2 center = float2(0.5f, 0.5f);

		float x = length(float2(tc.x, visor_ratio(tc.y)) - center);

		if (x < GM_VIS_RADIUS)
			return image;
		else
		{
			float p = saturate((x / GM_VIS_RADIUS - 1.f) * 2.f);

			for (int i = 0; i < GM_VIS_NUM; i++)
			{
				float N = 1.f - p / 0.6f + 0.4f * (i / (GM_VIS_NUM - 1.f)) * p;
				float2 m = (center - tc) * -N + center;
				final += s_image.SampleLevel(smp_rtlinear, m, 0);
			}
			final /= GM_VIS_NUM;
			
			float3 reflected = (image.xyz + GM_VIS_INTENSITY.xxx * final.xxx) / (1.f + GM_VIS_INTENSITY.xxx);	
			return reflected;		
		}
	}
	
	float3 img_corrections(float3 img)
	{
		//exposure
		img.xyz *= pp_img_corrections.x;
		
		//color grading (thanks KD and Crytek and Cjayho)
		float fLum = dot(img.xyz, LUMINANCE_VECTOR);
		float3 cColor = lerp(0.f, pp_img_cg.xyz, saturate( fLum * 2.f ) );
		cColor = lerp( cColor, 1.f, saturate( fLum - 0.5f) * 2.f );
		
		if (pp_img_cg.x > 0.f || pp_img_cg.y > 0.f || pp_img_cg.z > 0.f) 
		{
			img.xyz = saturate(lerp( img.xyz, cColor.xyz , saturate( fLum * 0.15f ) ));	
		}
		
		//saturation
		img.xyz = lerp(img.xyz, dot(img.xyz, LUMINANCE_VECTOR), (1.f - pp_img_corrections.z)); 
		
		//gamma correction
		img.xyz = pow(img,(1.f/pp_img_corrections.y));	
		
		//that's all :)
		return img.xyz;
	}
#endif