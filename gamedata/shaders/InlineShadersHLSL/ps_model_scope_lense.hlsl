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

struct 	v2p
{
 	float2 	tc0: 		TEXCOORD0;	// base
 	float3 	tc1: 		TEXCOORD1;	// environment
  	float4	c0:			COLOR0;		// sun.(fog*fog)
};

#include "Headers\pnv.h"

float resize(float input, float factor, float offset)
{
	return (input - 0.5f + offset) / factor + 0.5f - offset;
}

inline bool isSecondVPActive()
{
	return (m_blender_mode.z == 1.f);
}

float4 main	(v2p I, float4 pos2d : SV_Position): SV_Target
{
	float4 t_base = s_base.Sample(smp_base, I.tc0);
	I.tc0.x = resize(I.tc0.x, screen_res.x / screen_res.y, 0);		
	
	float2 svp_tc = pos2d.xy / screen_res.xy;
	
	float4 t_svp = s_vp2.Sample(smp_base, svp_tc);
	
	if (!isSecondVPActive()) 
	{
		t_svp.rgb /= 100;
	} 
	else if (m_blender_mode.x == 1.f) 
	{
		t_svp.rgb = calc_night_vision_effect(I.tc0, t_svp, float3(0.66, 2.0, 0.5));
	}
	else if (m_blender_mode.x == 2.f) 
	{
		t_svp.rgb = calc_night_vision_effect(I.tc0, t_svp, float3(0.79, 0.191, 0.247));
	}

    svp_tc -= 0.5;
    svp_tc.x *= screen_res.x / screen_res.y;
    svp_tc += 0.5; 

	t_svp.rgb *= 1.-smoothstep(0.25,0.3, distance(svp_tc,float2(0.5f,0.5f)));

	float3 final = lerp(t_svp, t_base, t_base.a);
	
	return float4(final.r, final.g, final.b, m_hud_params.w);
}