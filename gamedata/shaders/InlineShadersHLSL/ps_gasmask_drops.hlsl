//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#include "Headers\common.h"

// Original shader: https://www.shadertoy.com/view/ltffzl
//Done by BigWIngs (thanks man!). Modified, and ported by LVutner (2019)

float2 mod289(float2 x) { return x - floor(x * (1.f / 289.f)) * 289.f; }
float3 mod289(float3 x) { return x - floor(x * (1.f / 289.f)) * 289.f; }
float4 mod289(float4 x) { return x - floor(x * (1.f / 289.f)) * 289.f; }
float3 permute(float3 x) { return mod289(((x*34.f)+1.f)*x); }
float4 permute(float4 x) { return mod289(((x*34.f)+1.f)*x); }
/// <summary>
/// 2D Noise by Ian McEwan, Ashima Arts.
/// <summary>
float snoise_2D (float2 v)
{
    const float4 C = float4(0.211324865405187f,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439f,  // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626f, // -1.0 + 2.0 * C.x
                        0.024390243902439f); // 1.0 / 41.0

    // First corner
    float2 i  = floor(v + dot(v, C.yy) );
    float2 x0 = v -   i + dot(i, C.xx);

    // Other corners
    float2 i1;
    i1 = (x0.x > x0.y) ? float2(1.f, 0.f) : float2(0.f, 1.f);
    float4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    float3 p = permute( permute( i.y + float3(0.f, i1.y, 1.f ))
        + i.x + float3(0.f, i1.x, 1.f ));

    float3 m = max(0.5f - float3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.f);
    m = m*m ;
    m = m*m ;

    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

    float3 x = 2.f * frac(p * C.www) - 1.f;
    float3 h = abs(x) - 0.5f;
    float3 ox = floor(x + 0.5f);
    float3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159f - 0.85373472095314f * ( a0*a0 + h*h );

    // Compute final noise value at P
    float3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.f * dot(m, g);
}

float2 l_jh2(float2 f, float4 s, float l)
{
	float2 x = s.xy, V = s.zw;
	float y = snoise_2D(f * float2(GM_DROPS_TURBSIZE, GM_DROPS_TURBSIZE))*0.5f;
	float4 r = float4(y, y, y, 1.f);
	r.xy = float2(r.x + r.z/4.f, r.y + r.x/2.f);
	r -= 1.5f;
	r *= l;
	return saturate(f + (x + V) *r.xy);
}


float3 N13(float p) {
   // from DAVE HOSKINS
   float3 p3 = frac(float3(p,p,p) * float3(0.1031f,0.11369f,0.13787f));
   p3 += dot(p3, p3.yzx + 19.19f);
   return frac(float3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}

float N(float t) {
    return frac(sin(t*12345.564f)*7658.76f);
}

float Saw(float b, float t) {
    return smoothstep(0.f, b, t)*smoothstep(1.f, b, t);
}

float2 DropLayer2(float2 uv, float t) 
{
    uv.y = 1.f-uv.y;  
    float2 UV = uv;
  
    uv.y += t*0.75f;
    float2 a = float2(9.f, 2.f);
    float2 grid = a*2.f;
    float2 id = floor(uv*grid);
    
    float colShift = N(id.x); 
    uv.y += colShift;
    
    id = floor(uv*grid);
    float3 n = N13(id.x*35.2f+id.y*2376.1f);
    float2 st = frac(uv*grid)-float2(0.5f, 0.f);
    
    float x = n.x-0.5f;
    
    float y = UV.y;
    float wiggle = sin(y+sin(y));
    x += wiggle*(0.5f-abs(x))*(n.z-0.5f);
    x *= 0.7f;
    float ti = frac(t+n.z);
    y = (Saw(0.85f, ti)-0.5f)*0.9f+0.5f;
    float2 p = float2(x, y);
    
    float d = length((st-p)*a.yx);
    
    float mainDrop = smoothstep(0.5f, 0.f, d);
    
    float r = sqrt(smoothstep(1.f, y, st.y));
    float cd = abs(st.x-x);
    float trail = smoothstep(0.23f*r, 0.15f*r*r, cd);
    float trailFront = smoothstep(-0.02f, 0.02f, st.y-y);
    trail *= trailFront*r*r;
    
    y = UV.y;
    float trail2 = smoothstep(0.2f*r, 0.f, cd);
    float droplets = max(0.f, (sin(y*(1.f-y)*120.f)-st.y))*trail2*trailFront*n.z;
    y = frac(y*1.f)+(st.y-0.5f); //1 is fucking trail 
    float dd = length(st-float2(x, y));
    droplets = smoothstep(0.3f, 0.f, dd);
    float m = mainDrop+droplets*r*trailFront;

    return float2(m, trail);
}

float StaticDrops(float2 uv, float t) 
{
	uv *= 40.f;
    
    float2 id = floor(uv);
    uv = frac(uv)-0.5f;
    float3 n = N13(id.x*107.45f+id.y*3543.654f);
    float2 p = (n.xy-0.5f)*0.7f;
    float d = length(uv-p);
    
    float fade = Saw(0.025f, frac(t+n.z));
    float c = smoothstep(0.5f, 0.f, d)*frac(n.z*10.f)*fade;
    return c;
}

float2 Drops(float2 uv, float t, float l0, float l1, float l2) 
{
    float s = StaticDrops(lerp(uv, l_jh2(uv, GM_DROPS_TURBSHIFT, GM_DROPS_TURBTIME), GM_DROPS_TURBCOF), t)*l0; 
    float2 m1 = DropLayer2(lerp(uv, l_jh2(uv, GM_DROPS_TURBSHIFT, GM_DROPS_TURBTIME), GM_DROPS_TURBCOF), t)*l1;
    float2 m2 = DropLayer2(lerp(uv, l_jh2(uv, GM_DROPS_TURBSHIFT, GM_DROPS_TURBTIME), GM_DROPS_TURBCOF)*1.75, t)*l2;
    
    float c = s+m1.x+m2.x;
    c = smoothstep(0.3f, 1.f, c);
    
    return float2(c, max(m1.y*l0, m2.y*l1));
}

float4 main(p_screen I) : SV_Target
{
    float rainAmount = drops_control.x;
	float2 uv = I.tc0;

    float T = drops_control.z*(timers.x + rainAmount * 2.f);    

    float t = T*0.2f;

    float staticDrops = smoothstep(-0.5f, 4.f, rainAmount)*8.f;
    float layer1 = smoothstep(0.25f, 0.75f, rainAmount);
    float layer2 = smoothstep(0.f, 0.5f, rainAmount);

    float2 c = Drops(uv, t, staticDrops, layer1, layer2);

    float2 e = float2(0.00005f, 0.f); //fycking uv
    float cx = Drops(uv+e, t, staticDrops, layer1, layer2).x;
    float cy = Drops(uv+e.yx, t, staticDrops, layer1, layer2).x;
    float2 n = float2(cx-c.x, cy-c.x);


    float3 col = s_image.Sample(smp_nofilter, uv + n);

	return float4(col, 1.f);
}