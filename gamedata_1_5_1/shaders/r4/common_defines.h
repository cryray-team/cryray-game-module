#ifndef	common_defines_h_included
#define	common_defines_h_included

//////////////////////////////////////////////////////////////////////////////////////////
// Defines                                		//
#define def_gloss       float(2.f /255.f)
#define def_dbumph      float(0.333f)
#define def_virtualh    float(0.05f)              // 5cm
#define def_distort     float(0.05f)             // we get -0.5 .. 0.5 range, this is -512 .. 512 for 1024, so scale it
#define def_hdr         float(9.h)         		// hight luminance range float(3.h)
#define def_hdr_clip	float(0.75h)        		//

#define	LUMINANCE_VECTOR	float3(0.3f, 0.38f, 0.22f)

#define SKY_WITH_DEPTH				// sky renders with depth to avoid some problems with reflections
#define SKY_EPS float(0.001f)
#define SKY_DEPTH float(10000.f)
#define FARPLANE float(180.f)

//////////////////////////////////////////////////////////////
// Screen space sunshafts
#define SS_INTENSITY float(0.35)		
#define SS_BLEND_FACTOR float(0.8)		
#define SS_LENGTH float(1.0)		
	
#if defined(SM_5) || defined(SM_4_1)
    #define TEXTURE2DMS(a, b) Texture2DMS<a>
#else
    #define TEXTURE2DMS(a, b) Texture2DMS<a, b>
#endif
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef SMAP_size
#define SMAP_size        1024
#endif
#define PARALLAX_H 0.02
#define parallax float2(PARALLAX_H, -PARALLAX_H/2)
//////////////////////////////////////////////////////////////////////////////////////////

#endif	//	common_defines_h_included
