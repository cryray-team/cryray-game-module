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

//////////////////////////////////////////////////////////////////////////////////////////
float4	MiddleGray;
//////////////////////////////////////////////////////////////////////////////////////////
//	perform 4x4 bilinear, 8x8p, the step (C)
//	c):	8x8p	=> 1x1p		with exp

	// native bilinear
float sample( float2 tc )
{
	float4	data = s_image.Sample( smp_rtlinear, tc );
	return 	dot( data, 1.f/4.f );	// sum components
}

float4 main ( p_filter I ) : SV_Target
{
	// sample
	float4 	accum0;
		accum0.x =	sample(I.Tex0.xy);
		accum0.y = 	sample(I.Tex1.xy);
		accum0.z = 	sample(I.Tex2.xy);
		accum0.w =	sample(I.Tex3.xy);
	float4 	accum1;
		accum1.x =	sample(I.Tex4.xy);
		accum1.y = 	sample(I.Tex5.xy);
		accum1.z = 	sample(I.Tex6.xy);
		accum1.w =	sample(I.Tex7.xy);
	float4 	accum2;
		accum2.x =	sample(I.Tex0.wz);
		accum2.y = 	sample(I.Tex1.wz);
		accum2.z = 	sample(I.Tex2.wz);
		accum2.w =	sample(I.Tex3.wz);
	float4 	accum3;
		accum3.x =	sample(I.Tex4.wz);
		accum3.y = 	sample(I.Tex5.wz);
		accum3.z = 	sample(I.Tex6.wz);
		accum3.w =	sample(I.Tex7.wz);

	// perform accumulation
	float4	final;
		final.x	= dot(accum0,1.f/4.f);
		final.y	= dot(accum1,1.f/4.f);
		final.z	= dot(accum2,1.f/4.f);
		final.w	= dot(accum3,1.f/4.f);
	float 	result	= dot(final, 1.f/4.f);

	// OK
	float	scale 		= MiddleGray.x / (result*MiddleGray.y + MiddleGray.z);	// final

	float  	scale_prev	= s_tonemap.Sample( smp_nofilter, I.Tex0.xy ).x;
	float 	rvalue = lerp(scale_prev,scale,MiddleGray.w);

			clamp(rvalue, 1.f/128.f, 20.f);

	return rvalue;

}
