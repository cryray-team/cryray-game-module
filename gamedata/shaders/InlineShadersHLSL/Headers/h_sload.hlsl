//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef SLOAD_H
#define SLOAD_H

#include "Headers\h_common.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////
// Bumped surface loader                //
//////////////////////////////////////////////////////////////////////////////////////////
struct	surface_bumped
{
	float4	base;
	float3	normal;
	float	gloss;
	float	height;

};

float4 tbase( float2 tc )
{
   return	s_base.Sample( smp_base, tc);
}

#if defined(USE_STEEPPARALLAX)

static const float fParallaxStartFade = 8.f;
static const float fParallaxStopFade = 12.f;

void UpdateTC( inout p_bumped I)
{
	if (I.position.z < fParallaxStopFade)
	{
		const float maxSamples = 25.f;
		const float minSamples = 5.f;
		const float fParallaxOffset = -0.013f;

		float3	 eye = mul (float3x3(I.M1.x, I.M2.x, I.M3.x,
									 I.M1.y, I.M2.y, I.M3.y,
									 I.M1.z, I.M2.z, I.M3.z), -I.position.xyz);

		eye = normalize(eye);
		
		//	Calculate number of steps
		float nNumSteps = lerp( maxSamples, minSamples, eye.z );

		float	fStepSize			= 1.f / nNumSteps;
		float2	vDelta				= eye.xy * fParallaxOffset*1.2f;
		float2	vTexOffsetPerStep	= fStepSize * vDelta;

		//	Prepare start data for cycle
		float2	vTexCurrentOffset	= I.tcdh;
		float	fCurrHeight			= 0.f;
		float	fCurrentBound		= 1.f;

		for( int i=0; i<nNumSteps; ++i )
		{
			if (fCurrHeight < fCurrentBound)
			{	
				vTexCurrentOffset += vTexOffsetPerStep;		
				fCurrHeight = s_bumpX.SampleLevel( smp_base, vTexCurrentOffset.xy, 0 ).a; 
				fCurrentBound -= fStepSize;
			}
		}

		//	Reconstruct previouse step's data
		vTexCurrentOffset -= vTexOffsetPerStep;
		float fPrevHeight = s_bumpX.Sample( smp_base, float3(vTexCurrentOffset.xy,0) ).a;

		//	Smooth tc position between current and previouse step
		float	fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
		float	fDelta1 = (fCurrentBound - fCurrHeight);
		float	fParallaxAmount = (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1 ) / ( fDelta2 - fDelta1 );
		float	fParallaxFade 	= smoothstep(fParallaxStopFade, fParallaxStartFade, I.position.z);
		float2	vParallaxOffset = vDelta * ((1- fParallaxAmount )*fParallaxFade);
		float2	vTexCoord = I.tcdh + vParallaxOffset;
	
		//	Output the result
		I.tcdh.xy = vTexCoord;

#if defined(USE_TDETAIL) && defined(USE_STEEPPARALLAX)
		I.tcdbump = vTexCoord * dt_params;
#endif
	}

}

#elif	defined(USE_PARALLAX) && defined(USE_STEEPPARALLAX)

void UpdateTC( inout p_bumped I)
{
	float3	 eye = mul (float3x3(I.M1.x, I.M2.x, I.M3.x,
								 I.M1.y, I.M2.y, I.M3.y,
								 I.M1.z, I.M2.z, I.M3.z), -I.position.xyz);
								 
	float	height	= s_bumpX.Sample( smp_base, I.tcdh).w;	//
			//height  /= 2;
			//height  *= 0.8;
			height	= height*(parallax.x) + (parallax.y);	//
	float2	new_tc  = I.tcdh + height * normalize(eye);	//

	//	Output the result
	I.tcdh.xy	= new_tc;
}

#else	//	USE_PARALLAX

void UpdateTC( inout p_bumped I)
{
	;
}

#endif	//	USE_PARALLAX

surface_bumped sload_i( p_bumped I)
{
	surface_bumped	S;
   
	UpdateTC(I);	//	All kinds of parallax are applied here.

	float4 	Nu	= s_bump.Sample( smp_base, I.tcdh );		// IN:	normal.gloss
	float4 	NuE	= s_bumpX.Sample( smp_base, I.tcdh);	// IN:	normal_error.height

	S.base		= tbase(I.tcdh);				//	IN:  rgb.a
	S.normal	= Nu.wzy + (NuE.xyz - 1.f);	//	(Nu.wzyx - .5h) + (E-.5)
	S.gloss		= Nu.x*Nu.x;					//	S.gloss = Nu.x*Nu.x;
	S.height	= NuE.w;
	//S.height	= 0;

#ifdef        USE_TDETAIL
#ifdef        USE_TDETAIL_BUMP
	float4 NDetail		= s_detailBump.Sample( smp_base, I.tcdbump);
	float4 NDetailX		= s_detailBumpX.Sample( smp_base, I.tcdbump);
	S.gloss				= S.gloss * NDetail.x * 2.f;
	//S.normal			+= NDetail.wzy-.5;
	S.normal			+= NDetail.wzy + NDetailX.xyz - 1.f; //	(Nu.wzyx - .5h) + (E-.5)

	float4 detail		= s_detail.Sample( smp_base, I.tcdbump);
	S.base.rgb			= S.base.rgb * detail.rgb * 2.f;

//	S.base.rgb			= float3(1,0,0);
#else        //	USE_TDETAIL_BUMP
	float4 detail		= s_detail.Sample( smp_base, I.tcdbump);
	S.base.rgb			= S.base.rgb * detail.rgb * 2.f;
	S.gloss				= S.gloss * detail.w * 2.f;

#endif        //	USE_TDETAIL_BUMP
#endif

	return S;
}

surface_bumped sload_i( p_bumped I, float2 pixeloffset )
{
	surface_bumped	S;
   
   // apply offset
#ifdef	MSAA_ALPHATEST_LOW
   I.tcdh.xy += pixeloffset.x * ddx(I.tcdh.xy) + pixeloffset.y * ddy(I.tcdh.xy);
#endif

	UpdateTC(I);	//	All kinds of parallax are applied here.

	float4 	Nu	= s_bump.Sample( smp_base, I.tcdh );		// IN:	normal.gloss
	float4 	NuE	= s_bumpX.Sample( smp_base, I.tcdh);	// IN:	normal_error.height

	S.base		= tbase(I.tcdh);				//	IN:  rgb.a
	S.normal	= Nu.wzyx + (NuE.xyz - 1.f);	//	(Nu.wzyx - .5h) + (E-.5)
	S.gloss		= Nu.x*Nu.x;					//	S.gloss = Nu.x*Nu.x;
	S.height	= NuE.w;
	//S.height	= 0;

#ifdef        USE_TDETAIL
#ifdef        USE_TDETAIL_BUMP
#ifdef MSAA_ALPHATEST_LOW
#if defined(USE_STEEPPARALLAX)
   I.tcdbump.xy += pixeloffset.x * ddx(I.tcdbump.xy) + pixeloffset.y * ddy(I.tcdbump.xy);
#endif
#endif

	float4 NDetail		= s_detailBump.Sample( smp_base, I.tcdbump);
	float4 NDetailX		= s_detailBumpX.Sample( smp_base, I.tcdbump);
	S.gloss				= S.gloss * NDetail.x * 2.f;
	//S.normal			+= NDetail.wzy-.5;
	S.normal			+= NDetail.wzy + NDetailX.xyz - 1.f; //	(Nu.wzyx - .5h) + (E-.5)

	float4 detail		= s_detail.Sample( smp_base, I.tcdbump);
	S.base.rgb			= S.base.rgb * detail.rgb * 2.f;

//	S.base.rgb			= float3(1,0,0);
#else        //	USE_TDETAIL_BUMP
#ifdef MSAA_ALPHATEST_LOW
   I.tcdbump.xy += pixeloffset.x * ddx(I.tcdbump.xy) + pixeloffset.y * ddy(I.tcdbump.xy);
#endif
	float4 detail		= s_detail.Sample( smp_base, I.tcdbump);
	S.base.rgb			= S.base.rgb * detail.rgb * 2.f;
	S.gloss				= S.gloss * detail.w * 2.f;
#endif        //	USE_TDETAIL_BUMP
#endif

	return S;
}

surface_bumped sload ( p_bumped I)
{
    surface_bumped      S   = sload_i	(I);
	
	if (xshading_model != 6.f)
		S.normal.z			*=	0.5f;		//. make bump twice as contrast (fake, remove me if possible)
	
    return              S;
}

surface_bumped sload ( p_bumped I, float2 pixeloffset )
{
    surface_bumped      S   = sload_i	(I, pixeloffset );
	
	if (xshading_model != 6.f)
		S.normal.z			*=	0.5f;		//. make bump twice as contrast (fake, remove me if possible)
	
    return              S;
}

#endif
