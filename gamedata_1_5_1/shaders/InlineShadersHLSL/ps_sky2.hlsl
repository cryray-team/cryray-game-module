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

struct         v2p
{
	float4	factor	: COLOR0;        // for SM3 - factor.rgb - tonemap-prescaled
	float3	tc0		: TEXCOORD0;
	float3	tc1		: TEXCOORD1;
};
struct        _out
{
	float4	low		: SV_Target0;
	float4	high	: SV_Target1;
};


TextureCube	s_sky0	:register(t0);
TextureCube	s_sky1	:register(t1);

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
_out main( v2p I )
{

	float3	s0		= s_sky0.SampleLevel( smp_base, I.tc0, 0 );
	float3	s1		= s_sky1.SampleLevel( smp_base, I.tc1, 0 );
	float3	sky		= I.factor.zyx*lerp( s0, s1, I.factor.w );

	// final tone-mapping
	_out			o;

	o.low        	=		( (sky*(1.f+sky/2.89f)) / (sky+1.f) ).xyzz;
	o.high        	=		o.low /def_hdr	;
	return        	o;
}