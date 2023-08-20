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

uniform float4 		c_brightness;
uniform float4 		c_colormap;

//////////////////////////////////////////////////////////////////////////////////////////
// Pixel
float4 main( p_postpr I ) : SV_Target
{
//	float3	t_0 	= tex2D		(s_base0,I.tc0);
//	float3	t_1 	= tex2D		(s_base1,I.tc1);
	float3	t_0 	= s_base0.Sample( smp_rtlinear, I.Tex0);
	float3	t_1 	= s_base1.Sample( smp_rtlinear, I.Tex1);	
	float3 	image_o	= (t_0+t_1)*0.5f;					// add_d2

	float	grad_i 	= dot		(image_o,(0.3333f).xxx);

	float3 	image0 	= s_grad0.Sample( smp_rtlinear, float2(grad_i,0.5f));
	float3 	image1 	= s_grad1.Sample( smp_rtlinear, float2(grad_i,0.5f));
	float3	image	= lerp		(image0, image1, c_colormap.y);
			image	= lerp		(image_o, image, c_colormap.x);

	float	gray 	= dot		(image,I.Gray);			// dp3
			image 	= lerp 		(gray,image,I.Gray.w);		// mul/mad

//	float4	t_noise	= tex2D		(s_noise,I.tc2);	
	float4	t_noise	= s_noise.Sample( smp_linear, I.Tex2);	
	float3 	noised 	= image*t_noise*2.f;                     		// mul_2x
			image	= lerp( noised, image, I.Color.w); 	// lrp ?
			image	= (image * I.Color + c_brightness)*2.f;		// mad
//		image	= (image + c_brightness) * I.Color;		// mad ?

	// out
	return  float4( image, 1.f);					// +mov
}
