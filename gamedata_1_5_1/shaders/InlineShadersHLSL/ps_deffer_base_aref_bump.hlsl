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
float4 	main	( p_bumped I ) : SV_Target
{
	surface_bumped 	S 		= sload 	(I);
	S.base.w = (S.base.w-def_aref.y*0.5f)/(1.f-def_aref.y*0.5f);
	return S.base;
}
#else	//	INLINE_ATOC
f_deffer 	main	( p_bumped I )
{
	f_deffer		O;

	surface_bumped 	S 		= sload 	(I);

	clip((S.base.w-def_aref.y*0.5f)/(1.f-def_aref.y*0.5f));

	// Sample normal, rotate it by matrix, encode position 
  	float3	Ne  = mul		(float3x3(I.M1, I.M2, I.M3), S.normal);
			Ne	= normalize	(Ne);

	// hemi,sun,material
	float 	ms	= xmaterial		;
#ifdef USE_LM_HEMI
//	float4	lm 	= tex2D			(s_hemi, I.lmh);
	float4	lm 	= s_hemi.Sample( smp_rtlinear, I.lmh);
	//float 	h  	= dot			(lm.rgb,1.h/3.h);
	float 	h  	= get_hemi(lm);
#else
	float 	h	= I.position.w	;
#endif

	O = pack_gbuffer(
						float4	(Ne,										h),
  						float4 	(I.position.xyz + Ne*S.height*def_virtualh, ms),
  						float4	(S.base.x,S.base.y,S.base.z,				S.gloss) );

	return 	O	;
}

#endif //	INLINE_ATOC
