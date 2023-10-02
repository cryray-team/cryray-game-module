//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\h_common.hlsl"
uniform float4 test;
/*
Texture2D	tex_lens_dirt;

uniform float4		e_allcol;				// 				x - r2_rcol 					| y - r2_gcol 						| z - r2_bcol 						| w - no
uniform	float4		e_complex_uniform;		// 				x - no					 		| y - r2_lens_dirt_type 			| z - no
uniform float4 		e_lens_dirt;			// 				x - r2_lens_dirt_val 			| y - no 							| z - no 	

float4 get_full_image(float2 texcoord)
{
	return s_image.Sample(smp_rtlinear, texcoord);
}

float3 get_na_image(float2 texcoord)
{
	return get_full_image(texcoord).xyz;
}

float3 get_na_gblurred_image(float2 texcoord, float dx, float dy)
	{
		float3 	img  = get_na_image(texcoord);
				img += get_na_image(texcoord +  float2(dx, 0));
				img += get_na_image(texcoord +  float2(dy, 0));
				img += get_na_image(texcoord +  float2(-dx, 0));
				img += get_na_image(texcoord +  float2(-dx, -dy));
				img += get_na_image(texcoord +  float2(dx, -dy));

		return 	img/6;
	}

float rnd(float2 uv)
{
    float f = frac(sin(dot(uv, float2(12.1234, 72.8392) )*45123.2));

 return f;   
}
*/

float rnd(float2 p)
{
    float f = frac(sin(dot(p, float2(12.1234, 72.8392) )*45123.2));
    return f;   
}

float rnd(float w)
{
    float f = frac(sin(w)*1000.);
    return f;   
}

float regShape(float2 p, int N)
{
    float f;
    float a = atan2(p.x,p.y)+.2;
    float b = 6.28319/float(N);
    f = smoothstep(.5,.51, cos(floor(.5+a/b)*b-a)*length(p.xy));
 
    return f;
}

float3 circle(float2 p, float size, float decay, float3 color,float3 color2, float dist, float3 mouse)
{ 
    //l is used for making rings.I get the length and pass it through a sinwave
    //but I also use a pow function. pow function + sin function , from 0 and up, = a pulse, at least
    //if you return the max of that and 0.0.
    
    float l = length(p + mouse.x*(dist*4.))+size/2.;
    
    //l2 is used in the rings as well...somehow...
    float l2 = length(p + mouse.x*(dist*4.))+size/3.;
    
    ///these are circles, big, rings, and  tiny respectively
    float c = max(0.01-pow(length(p + mouse.x*dist), size*1.4), 0.0)*50.;
    float c1 = max(0.001-pow(l-0.3, 1./40.)+sin(l*30.), 0.0)*3.;
    float c2 =  max(0.04/pow(length(p-mouse.x*dist/2. + 0.09)*1., 1.), 0.0)/20.;
    float s = max(0.01-pow(regShape(p*5. + mouse.x*dist*5. + 0.9, 6) , 1.), 0.0)*5.;
    
   	color = 0.5+0.5*sin(color);
    color = cos(float3(0.44, .24, .2)*8. + dist*4.)*0.5+.5;
 	float3 f = c*color ;
    f += c1*color;
    
    f += c2*color;  
    f +=  s*color;

    return f-0.01;
}

float4 main(v2p_aa_AA I, float4 pos2d : SV_Position/*float2 tc : TEXCOORD0, float4 pos2d : SV_Position*/) : SV_Target
{
    float2 uv = pos2d.xy / screen_res.xy-0.5;
    uv.x*= screen_res.x / screen_res.y;

    float2 mm = (test.xy *1000)/screen_res.xy - 0.5;
    mm.x *= screen_res.x / screen_res.y;

    float3 circColor = float3(0.9, 0.2, 0.1);
    float3 circColor2 = float3(0.3, 0.1, 0.9);

    float3 color = lerp(float3(0.3, 0.2, 0.02)/0.9, float3(0.2, 0.5, 0.8), uv.y)*3.-0.52*sin(timers.x);
    
    for(float i=0.;i<10.;i++)
    {
        color += circle(uv, pow(rnd(i*2000.)*1.8, 2.)+1.41, 0.0, circColor+i , circColor2+i, rnd(i*20.)*3.+0.2-.5, mm.x);
    }

    float a = atan2(uv.y-mm.y, uv.x-mm.x);
    float l = max(1.0-length(uv-mm)-0.84, 0.0);

    float bright = 0.1;

    color += max(0.1/pow(length(uv-mm)*5., 5.), 0.0)*abs(sin(a*5.+cos(a*9.)))/20.;
    color += max(0.1/pow(length(uv-mm)*10., 1./20.), .0)+abs(sin(a*3.+cos(a*9.)))/8.*(abs(sin(a*9.)))/1.;
    color += (max(bright/pow(length(uv-mm)*4., 1./2.), 0.0)*4.)*float3(0.2, 0.21, 0.3)*4.;

    color*= exp(1.0-length(uv-mm))/5.;

    pos2d = float4(color, 1);

    /*
    float2 mm = pos2d.xy / screen_res.xy - 0.5;
    mm.x *= screen_res.x / screen_res.y;

    float2 center = I.Tex0.xy;
    float3 img = s_image.Sample(smp_nofilter, I.Tex0.xy);

    float3 circColor = float3(0.9f, 0.2f, 0.1f);
    float3 circColor2 = float3(0.3f, 0.1f, 0.9f);

    //float mm = 1.f;

    //float2 mm = test.xy / screen_res.xy - 0.5;

    //float3 color = L_sun_color;//float3(0, 0, 0);
    float2 uv = center - 0.5f;

    //uv.x *= (screen_res.x * screen_res.w);
    float3 color = lerp(float3(0.3, 0.2, 0.02)/0.9, float3(0.2, 0.5, 0.8), uv.y)*3.-0.52*sin(timers.x);
    [unroll]
    for(float f = 0.f; f < 10.f; f++)
    {
        color += circle(uv, pow(rnd(f * 2000.f) * 1.8f, 2.f) + 1.41f, 0.f, circColor + f, circColor2 + f, rnd(f * 20.f) * 3.f + 0.2f - 0.5f, mm.x);
    }

    float a = atan2(uv.y - test.y, uv.x - test.x);
    float l = max(1.f - length(uv-mm)-0.84f, 0.f);

    float bright = 0.1f;

    //add the sun with the frill things
    color += max(0.1/pow(length(uv-mm)*5.f, 5.f), 0.f)*abs(sin(a*5.+cos(a*9.f)))/20.f;
    color += max(0.1/pow(length(uv-mm)*10.f, 1.f/20.f), 0.f)+abs(sin(a*3.f+cos(a*9.f)))/8.f*(abs(sin(a*9.f)))/1.f;

    //add another sun in the middle (to make it brighter)  with the20color I want, and bright as the numerator.
    color += (max(bright/pow(length(uv-mm)*4.f, 1.f/2.f), 0.f)*4.f)*float3(0.2f, 0.21f, 0.3f)*4.f;

    color*= exp(1.f-length(uv-mm))/5.f;

    img += color;

    if (e_complex_uniform.y != 0)
		{
			float dx11_lens_dirt_base = pow(get_na_gblurred_image((float(1).xx - float2(center)), 15 * screen_res.z, 15 * screen_res.w), 2);

			float3 dx11_lens_dirt;

			if (e_complex_uniform.y > 0)
			{
				dx11_lens_dirt = (dx11_lens_dirt_base * tex_lens_dirt.Sample(smp_rtlinear, center));
			}

			img.xyz += (dx11_lens_dirt * e_lens_dirt.x * 10);
		}
    */
    return pos2d;
}