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
#include "Headers\sload.h"

f_deffer main ( p_bumped I )
{
  f_deffer	O;

  surface_bumped 	S 	= sload 	(I);

  // Sample normal, rotate it by matrix, encode position 
  float3 Ne				= mul       (float3x3(I.M1, I.M2, I.M3), S.normal);
		Ne				= normalize	(Ne);

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

	O			= pack_gbuffer(
								float4	(Ne, 										h		),
								float4 	(I.position.xyz + Ne*S.height*def_virtualh, ms		),
//								float4 	(I.position.xyz, ms),
								float4	(S.base.xyz, 								S.gloss	) );

  return O;
}
