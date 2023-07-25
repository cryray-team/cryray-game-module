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

// Directionally Localized antiAliasing
// Original ver: facepuncherfromrussia (2011)
// Update   ver: ForserX (2018) 
// New: New Luminance code

#define PIXEL_SIZE screen_res.zw
#define LD(o, dx, dy, uv) o = s_image.Sample(smp_rtlinear, uv + float2(dx, dy) * PIXEL_SIZE.xy);

static const bool bPreserveHf = 0;

// sRGB primaries and D65 white point
inline float Luminance(float3 rgb)
{
#ifdef HDR 
	float3 CurrentDot = float3(0.2627, 0.6780, 0.0593);
#else // HD
	float3 CurrentDot = float3(0.2126, 0.7152, 0.0722);
#endif
    return dot(rgb, CurrentDot);
}


float4 main(v2p_aa_AA I) : SV_Target
{
    const float lambda = 3.0f;
    const float epsilon = 0.1f;

    float4 center, left_01, right_01, top_01, bottom_01;
 
    LD(center,      0,   0, I.Tex0.xy)
    LD(left_01,  -1.5,   0, I.Tex0.xy)
    LD(right_01,  1.5,   0, I.Tex0.xy)
    LD(top_01,      0,-1.5, I.Tex0.xy)
    LD(bottom_01,   0, 1.5, I.Tex0.xy)
 
	float4 w_h = 2.0f * (left_01 + right_01);
	float4 w_v = 2.0f * (top_01 + bottom_01);

	float4 left, right, top, bottom;
	 
	LD(left,  -1,  0, I.Tex0.xy)
	LD(right,  1,  0, I.Tex0.xy)
	LD(top,    0, -1, I.Tex0.xy)
	LD(bottom, 0,  1, I.Tex0.xy)
	 
	float4 edge_h = abs(left + right - 2.f * center) / 2.f;
	float4 edge_v = abs(top + bottom - 2.f * center) / 2.f;
	 
	float4 blurred_h = (w_h + 2.f * center) / 6.f;
	float4 blurred_v = (w_v + 2.f * center) / 6.f;
	 
	float edge_h_lum = Luminance(edge_h.xyz);
	float edge_v_lum = Luminance(edge_v.xyz);
	
	float blurred_h_lum = Luminance(blurred_h.xyz);
	float blurred_v_lum = Luminance(blurred_v.xyz);
	 
	float edge_mask_h = saturate((lambda * edge_h_lum - epsilon) / blurred_v_lum);
	float edge_mask_v = saturate((lambda * edge_v_lum - epsilon) / blurred_h_lum);
	 
	float4 clr = center;
	clr = lerp(clr, blurred_h, edge_mask_v);
	clr = lerp(clr, blurred_v, edge_mask_h * 0.5f); // TFU2 uses 1.0f instead of 0.5f
	 
	float4 h0, h1, h2, h3, h4, h5, h6, h7;
	float4 v0, v1, v2, v3, v4, v5, v6, v7;

	LD(h0, 1.5, 0, I.Tex0.xy) LD(h1, 3.5, 0, I.Tex0.xy) LD(h2, 5.5, 0, I.Tex0.xy) LD(h3, 7.5, 0, I.Tex0.xy) LD(h4, -1.5,0, I.Tex0.xy) LD(h5, -3.5,0, I.Tex0.xy) LD(h6, -5.5,0, I.Tex0.xy) LD(h7, -7.5,0, I.Tex0.xy)
	LD(v0, 0, 1.5, I.Tex0.xy) LD(v1, 0, 3.5, I.Tex0.xy) LD(v2, 0, 5.5, I.Tex0.xy) LD(v3, 0, 7.5, I.Tex0.xy) LD(v4, 0,-1.5, I.Tex0.xy) LD(v5, 0,-3.5, I.Tex0.xy) LD(v6, 0,-5.5, I.Tex0.xy) LD(v7, 0,-7.5, I.Tex0.xy)
	 
	float long_edge_mask_h = (h0.a + h1.a + h2.a + h3.a + h4.a + h5.a + h6.a + h7.a) / 8.f;
	float long_edge_mask_v = (v0.a + v1.a + v2.a + v3.a + v4.a + v5.a + v6.a + v7.a) / 8.f;
	 
	long_edge_mask_h = saturate(long_edge_mask_h * 2.f - 1.f);
	long_edge_mask_v = saturate(long_edge_mask_v * 2.f - 1.f);

	if (abs(long_edge_mask_h - long_edge_mask_v) > 0.2f)	// resistant to noise (TFU2 SPUs)
	{
		float4 left, right, top, bottom;
	 
		LD(left,  -1,  0, I.Tex0.xy)
		LD(right,  1,  0, I.Tex0.xy)
		LD(top,    0, -1, I.Tex0.xy)
		LD(bottom, 0,  1, I.Tex0.xy)
	 
		float4 long_blurred_h = (h0 + h1 + h2 + h3 + h4 + h5 + h6 + h7) / 8.f;
		float4 long_blurred_v = (v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7) / 8.f;
	 
		float lb_h_lum   = Luminance(long_blurred_h.xyz);
		float lb_v_lum   = Luminance(long_blurred_v.xyz);
	 
		float center_lum = Luminance(center.xyz);
		float left_lum   = Luminance(left.xyz);
		float right_lum  = Luminance(right.xyz);
		float top_lum    = Luminance(top.xyz);
		float bottom_lum = Luminance(bottom.xyz);
	 
		float4 clr_v = center;
		float4 clr_h = center;
	 
		float hx = saturate((lb_h_lum - top_lum   ) / (center_lum - top_lum   ));
		float vx = saturate((lb_v_lum - left_lum  ) / (center_lum - left_lum  ));
		
		float hy = saturate(1 + (lb_h_lum - center_lum) / (center_lum - bottom_lum));
		float vy = saturate(1 + (lb_v_lum - center_lum) / (center_lum - right_lum ));
	 
		// Vectorized searching
		float4 vhxy = float4(vx, vy, hx, hy);
		vhxy = vhxy == float4(0.f, 0.f, 0.f, 0.f) ? float4(1.f, 1.f, 1.f, 1.f) : vhxy;
	 
		clr_v = lerp(left  , clr_v, vhxy.x);
		clr_v = lerp(right , clr_v, vhxy.y);
		clr_h = lerp(top   , clr_h, vhxy.z);
		clr_h = lerp(bottom, clr_h, vhxy.w);

		clr = lerp(clr, clr_v, long_edge_mask_v);
		clr = lerp(clr, clr_h, long_edge_mask_h);
	}
	

	return clr;
}