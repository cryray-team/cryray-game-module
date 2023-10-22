//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	common_functions_h_included
#define	common_functions_h_included

//	contrast function
float Contrast(float Input, float ContrastPower)
{
     //piecewise contrast function
     bool IsAboveHalf = Input > 0.5f ;
     float ToRaise = saturate(2.f*(IsAboveHalf ? 1.f-Input : Input));
     float Output = 0.5f*pow(ToRaise, ContrastPower); 
     Output = IsAboveHalf ? 1.f-Output : Output;
     return Output;
}

float3 vibrance( float3 img, float val )
{
    float luminance = dot( float3( img.rgb ), LUMINANCE_VECTOR );
    return float3( lerp( luminance, float3( img.rgb ), val ));
}

void        tonemap              (out float4 low, out float4 high, float3 rgb, float scale)
{
        rgb	= (bloom_type == 4) ? rgb*scale : rgb*sqrt(scale);

	if (bloom_type == 4)
	{
    	low		= float4(((rgb*(1+rgb/fWhiteIntensitySQR)) / (rgb+1)), 0);
		high	= float4(rgb/def_hdr, 0);
	}
	//-' Чистое небо
	else if(bloom_type == 3)
	{
		low		= ((rgb*(1+rgb/fWhiteIntensitySQR)) / (rgb+1)).xyzz;
		high	= rgb.xyzz/def_hdr;
	}
	//-' Билд 2218
	else if(bloom_type == 2)
	{
		low 	= float4(rgb, 0);
		high	= float4(rgb-def_lum_hrange, dot(min(rgb,def_lum_hrange), LUMINANCE_VECTOR ) );
	}
	//-' Лост Альфа
	else if(bloom_type == 1)
	{
		low		= rgb.xyzz;
		high	= rgb.xyzz-saturate(rgb.xyzz);
	}
}

float3 compute_colored_ao(float ao, float3 albedo)
{ //https://www.activision.com/cdn/research/s2016_pbs_activision_occlusion.pptx
    float3 a = 2.0404f * albedo - 0.3324f;
    float3 b = -4.7951f * albedo + 0.6417f;
    float3 c = 2.7552f * albedo + 0.6903f;

    return max(ao, ((ao * a + b) * ao + c) * ao);
}

float4 combine_bloom(float3 low, float4 high)    
{
        return        float4(low.xyz + high.xyz*high.w, 1.f);
}	

float calc_fogging( float4 w_pos )      
{
	return dot(w_pos,fog_plane);         
}

float2 unpack_tc_base( float2 tc, float du, float dv )
{
		return (tc.xy + float2	(du,dv))*(32.f/32768.f); //!Increase from 32bit to 64bit floating point
}

float3 calc_sun_r1( float3 norm_w )    
{
	return L_sun_color*saturate(dot((norm_w),-L_sun_dir_w));                 
}

float3 calc_model_hemi_r1( float3 norm_w )    
{
 return max(0.f,norm_w.y)*L_hemi_color.xyz;
}

float3 calc_model_lq_lighting( float3 norm_w )    
{
	return L_material.x*calc_model_hemi_r1(norm_w.xyz) + L_ambient.xyz + L_material.y*calc_sun_r1(norm_w.xyz);
}

float3 	unpack_normal( float3 v )	{ return 2.f*v-1.f; }
float3 	unpack_bx2( float3 v )	{ return 2.f*v-1.f; }
float3 	unpack_bx4( float3 v )	{ return 4.f*v-2.f; } //!reduce the amount of stretching from 4*v-2 and increase precision
float2 	unpack_tc_lmap( float2 tc )	{ return tc*(1.f/32768.f);	} // [-1  .. +1 ] 
float4	unpack_color( float4 c ) { return c.bgra; }
float4	unpack_D3DCOLOR( float4 c ) { return c.bgra; }
float3	unpack_D3DCOLOR( float3 c ) { return c.bgr; }

float3   p_hemi( float2 tc )
{
	float4	t_lmh = s_hemi.Sample( smp_rtlinear, tc);
	return	t_lmh.a;
}

float   get_hemi( float4 lmh)
{
	return lmh.a;
}

float   get_sun( float4 lmh)
{
	return lmh.g;
}

float3	v_hemi(float3 n)
{
	return L_hemi_color.xyz*(0.5f + 0.5f*n.y);                   
}

float3	v_sun(float3 n)                        	
{
	return L_sun_color*dot(n,-L_sun_dir_w);                
}

float3	calc_reflection( float3 pos_w, float3 norm_w )
{
    return reflect(normalize(pos_w-eye_position), norm_w);
}
//CUSTOM
float3 blend_soft(float3 a, float3 b)
{
	return 1.f - (1.f - a) * (1.f - b);
}

float4 screen_to_proj(float2 screen, float z)
{
	float4 proj;
	proj.w = 1.f;
	proj.z = z;
	proj.x = screen.x*2.f - proj.w;
	proj.y = -screen.y*2.f + proj.w;
	return proj;
}


float4 convert_to_screen_space(float4 proj)
{
	float4 screen;
	screen.x = (proj.x + proj.w)*0.5f;
	screen.y = (proj.w - proj.y)*0.5f;
	screen.z = proj.z;
	screen.w = proj.w;
	return screen;
}

float4 proj_to_screen(float4 proj)
{
	float4 screen = proj;
	screen.x = (proj.x + proj.w);
	screen.y = (proj.w - proj.y);
	screen.xy *= 0.5f;
	return screen;
}

float normalize_depth(float depth)
{
	return (saturate(depth/100.f));
}

#ifndef SKY_WITH_DEPTH
float is_sky(float depth)
{
	return step(depth, SKY_EPS);
}
float is_not_sky(float depth)
{
	return step(SKY_EPS, depth);
}
#else
float is_sky(float depth)
{
	return step(abs(depth - SKY_DEPTH), SKY_EPS);
}
float is_not_sky(float depth)
{
	return step(SKY_EPS, abs(depth - SKY_DEPTH));
}
#endif

float hash(float2 intro)
{
return frac(1.0e4 * sin(17.f*intro.x + 0.1f*intro.y) * (0.1f + abs(sin(13.f*intro.y + intro.x))));
}

float hash3D(float3 intro)
{
return hash(float2(hash(intro.xy),intro.z));
}

float hash12(float2 p)
{
	float3 p3  = frac(float3(p.xyx) * 0.1031f);
    p3 += dot(p3, p3.yzx + 19.19f);
    return frac((p3.x + p3.y) * p3.z);
}

float2 hash22(float2 p)
{
	float3 p3 = frac(float3(p.xyx) * float3(0.1031f, 0.1030f, 0.0973f));
    p3 += dot(p3, p3.yzx+19.19f);
    return frac((p3.xx+p3.yz)*p3.zy);
}

float rand(float n)
{
    return frac(cos(n)*343.42f);
}

float noise(float2 tc)
{
    return frac(sin(dot(tc, float2(12.f, 78.f) + (timers.x) )) * 43758.f)*0.25f; 
}

#endif	//	common_functions_h_included
