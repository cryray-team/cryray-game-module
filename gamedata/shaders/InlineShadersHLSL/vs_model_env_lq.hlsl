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
#include "Headers\skin.h"
#include "ScreenSpaceAddon\screenspace_fog.h"

struct 	vf
{
	float2 tc0	: TEXCOORD0;		// base
	float3 tc1	: TEXCOORD1;		// environment
	float3 c0	: COLOR0;		// color
	float  fog	: FOG;
	float4 hpos	: SV_Position;
};

vf 	_main (v_model v)
{
	vf 		o;

	float4 	pos 	= v.P;
	float3  pos_w 	= mul(m_W, pos);
	float3 	norm_w 	= normalize(mul(m_W,float4(v.N, 0.0f)).xyz);

	o.hpos 		= mul(m_WVP, pos);		// xform, input in world coords
	o.tc0		= v.tc.xy;				// copy tc
	o.tc1		= calc_reflection(pos_w, norm_w);
	o.c0 		= calc_model_lq_lighting(float3(0.f,1.f,0.f)); // SSS 14.5 - Improve the illumination a little using a fake normal

	o.fog 		= saturate(calc_fogging(float4(pos_w,1.f)) );	// fog, input in world coords
	o.fog		= SSFX_FOGGING(1.f - o.fog, pos.y); // Add SSFX Fog

	return o;
}

/////////////////////////////////////////////////////////////////////////
#ifdef 	SKIN_NONE
vf	main(v_model v) 		{ return _main(v); 		}
#endif

#ifdef 	SKIN_0
vf	main(v_model_skinned_0 v) 	{ return _main(skinning_0(v)); }
#endif

#ifdef	SKIN_1
vf	main(v_model_skinned_1 v) 	{ return _main(skinning_1(v)); }
#endif

#ifdef	SKIN_2
vf	main(v_model_skinned_2 v) 	{ return _main(skinning_2(v)); }
#endif

#ifdef	SKIN_3
vf	main(v_model_skinned_3 v) 	{ return _main(skinning_3(v)); }
#endif

#ifdef	SKIN_4
vf	main(v_model_skinned_4 v) 	{ return _main(skinning_4(v)); }
#endif