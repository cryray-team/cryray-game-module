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

float4 main(float2 uv : TEXCOORD0) : SV_Target
{
	float3	YUV	= s_base.Sample( smp_base, uv );

	float	Y	= YUV.z;
	float	U	= YUV.y;
	float	V	= YUV.x;

	float	c	= 1.16406f	;
	float3	_Y	= float3		(c,				c,			c)			*Y	;
	float3	_U	= float3		(0, 			-0.390625f, +2.01562f)	*U	;
	float3	_V	= float3		(+1.59765f,		-0.8125f,	0)			*V	;
	float3	_S	= float3		(-0.86961f,		+0.53076f,	-1.0786f)		;
	

	return float4(_Y+_U+_V+_S,1);
}