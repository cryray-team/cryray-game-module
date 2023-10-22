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

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
#ifdef	USE_AREF
v2p_shadow_direct_aref main ( v_shadow_direct_aref I )
#else	//	USE_AREF
v2p_shadow_direct main ( v_shadow_direct I )
#endif	//	USE_AREF
{
#ifdef	USE_AREF
	v2p_shadow_direct_aref 	O;
#else	//	USE_AREF
	v2p_shadow_direct 		O;
#endif	//	USE_AREF
	

	// Transform to world coords
	float3 	pos	= mul		(m_xform , I.P);

	// 
	float 	base 	= m_xform._24;			// take base height from matrix
	float 	dp		= calc_cyclic  (wave.w+dot(pos,(float3)wave));
	float 	H 		= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	float 	inten 	= H * dp;			// intensity
	float2 	result;
#ifdef	USE_TREEWAVE
			result	= 0.f;
#else	//	USE_TREEWAVE
#ifdef	USE_AREF
	float 	frac 	= I.tc.z*consts.x;		// fractional (or rigidity)
#else	//	USE_AREF
	float 	frac 	= 0.f;
#endif	//	USE_AREF
			result	= calc_xz_wave	(wind.xz*inten, frac);
#endif	//	USE_TREEWAVE

	float4 	f_pos 	= float4(pos.x+result.x, pos.y, pos.z+result.y, 1);

	O.hpos 	= mul		(m_VP,	f_pos	);
#ifdef	USE_AREF
	O.tc0 	= (I.tc * consts).xy;		//	+ result;
#endif	//	USE_AREF

 	return	O;
}
FXVS;
