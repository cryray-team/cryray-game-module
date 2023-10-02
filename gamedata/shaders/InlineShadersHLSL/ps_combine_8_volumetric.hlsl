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

struct	_input      
{
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
};

struct	_out
{
        float4	low		: SV_Target0;
        float4	high	: SV_Target1;
};

//	TODO: DX10: Use load instead of sample
_out main( _input I )
{
	// final tone-mapping
	float          	tm_scale        = I.tc0.w;	// interpolated from VS

	_out	o;
	float4	color;

	color = s_vollight.Load(int3(I.tc0.xy*screen_res.xy, 0));

	tonemap(o.low, o.high, color.rgb, tm_scale );

	return o;
}