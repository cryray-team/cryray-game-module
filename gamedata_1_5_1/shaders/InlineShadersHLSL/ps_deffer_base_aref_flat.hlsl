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
#include "Headers\h_sload.hlsl"

#ifdef	INLINE_ATOC

float4 	main	( p_flat I ) : SV_Target
{
	float4 D 	= s_base.Sample (smp_linear, I.tcdh); //tbase (I.tcdh);
	D.w = (D.w-def_aref*0.5f)/(1.f-def_aref*0.5f);
	return D;
}

#else	//	INLINE_ATOC

f_deffer 	main	( p_flat I )
{
  f_deffer	O;

  // 1. Base texture + kill pixels with low alpha
  

  float4 D = s_base.Sample (smp_linear, I.tcdh);

clip((D.w-def_aref.x*0.5f)/(1.f-def_aref.x*0.5f));

#ifdef	USE_TDETAIL
//	D.rgb	= 2*D.rgb*tex2D	(s_detail, I.tcdbump).rgb;
	D.rgb	= 2.f*D.rgb * s_detail.Sample( smp_base, I.tcdbump ).rgb;
#endif

	// hemi,sun,material
	float 	ms	= xmaterial		;
#ifdef USE_LM_HEMI
//	float4	lm 	= tex2D			(s_hemi, I.lmh);
	float4	lm 	= s_hemi.Sample( smp_rtlinear, I.lmh);
//	float 	h  	= dot			(lm.rgb,1.h/3.h);
	float 	h  	= get_hemi(lm);
#else
	float 	h	= I.position.w	;
#endif

	D *= I.tcdh.z;

  // 2. Standart output
  float4 Ne     = float4	(normalize((float3)I.N.xyz), 					h			);
  
  O				= pack_gbuffer(
  								Ne,
								float4 	(I.position.xyz + Ne.xyz*def_virtualh/2.f	, 	ms			),
								float4	(D.rgb,											def_gloss	) );
  return O;
}

#endif	//	INLINE_ATOC
