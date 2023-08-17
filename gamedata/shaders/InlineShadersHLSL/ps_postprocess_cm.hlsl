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
// Pixel
float4 main( p_postpr I ) : SV_Target
{
	float3	t_0 	= s_base0.Sample( smp_rtlinear, I.Tex0.xy).xyz;
	float3	t_1 	= s_base1.Sample( smp_rtlinear, I.Tex1.xy).xyz;	
	float3 	image_o	= (t_0+t_1)*0.5f;					// add_d2

	float	grad_i 	= dot		(image_o.xyz,(0.3333f).xxx);

	float3 	image0 	= s_grad0.Sample( smp_rtlinear, float2(grad_i,0.5f)).xyz;
	float3 	image1 	= s_grad1.Sample( smp_rtlinear, float2(grad_i,0.5f)).xyz;
	float3	image	= lerp		(image0.xyz, image1, c_colormap.y).xyz;
			image	= lerp		(image_o.xyz, image, c_colormap.x);

	float	gray 	= dot		(image,I.Gray.xyz);			// dp3
			image 	= lerp 		(gray,image,I.Gray.w);		// mul/mad

	float3	t_noise	= s_noise.Sample( smp_linear, I.Tex2.xy).xyz;	
	float3 	noised 	= image.xyz*t_noise.xyz*2.f;                     		// mul_2x
			image	= lerp( noised.xyz, image, I.Color.w); 	// lrp ?
			image	= (image * I.Color.rgb + c_brightness.rgb)*2.f;		// mad

	// out
	return  float4( image, 1.f);					// +mov
}
