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
#include "Headers\lmodel.h"
#include "Headers\shadow.h"

float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4 pos2d : SV_Position ) : SV_Target
{

	gbuffer_data gbd = gbuffer_load_data( tc, pos2d );

	float4 _P = float4( gbd.P, 1.f );
	float4 PS = mul( m_shadow,  _P );

	float s	= shadow(PS);

	float2 tc1	= mul( m_sunmask, _P );		//

	tc1 /= 2.f;
	tc1 = frac(tc1);

	float4 water = s_water2D.SampleLevel( smp_linear, tc1, 0 );

	water.xyz = (water.xzy-0.5f)*2.f;
	water.xyz = mul( m_V, water.xyz );
	water *= s;

	return float4(water.xyz,s);
}