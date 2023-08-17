//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef GAUSSBLUR_H_INCLUDED
#define GAUSSBLUR_H_INCLUDED

float4 Gauss(Texture2D t2d, float2 texCoord, int factor, bool optimize)
{
	float4 outColor = float4(1.f, 1.f, 1.f, 1.f);

	if(!optimize)
	{
		float dx = factor*0.5f/screen_res.x;
		float dy = factor*0.5f/screen_res.y;

		outColor.rgb =
		(		
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy, 0).rgb +
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy + float2(dx, 1.f), 0).rgb +
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy + float2(dy, 1.f), 0).rgb +
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy + float2(-dx, 1.f), 0).rgb +
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy + float2(-dx, -dy), 0).rgb +
			1.f * t2d.SampleLevel(smp_rtlinear, texCoord.xy + float2(dx, -dy), 0).rgb
		)/6.f;	
	}
	else
	{
		const float delta = factor * (0.5f/screen_res.x);

		outColor.rgb =  t2d.SampleLevel( smp_rtlinear, texCoord.xy+delta, 0).rgb +
			t2d.SampleLevel( smp_rtlinear, texCoord.xy-delta, 0).rgb * (1.f/2.f);
	}

	return outColor;
}

#endif//GAUSSBLUR_H_INCLUDED