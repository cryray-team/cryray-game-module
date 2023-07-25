//////////////////////////////////////////////////////////////////////////
//-' CryRay Engine x64 Shaders
//-' Based on shaders from the original Anomaly 1.5.1
//-' As well as an integrated shader pack Screen Space 15.4
//-' Repository with shaders: https://github.com/cryray-team
//-' Also, if you are a shader developer
//-' You can join our server in discord: https://discord.gg/R78uVUZCTR
//-' OldSerpskiStalker7777, CryRay Team
//////////////////////////////////////////////////////////////////////////

#ifndef	common_policies_h_included
#define	common_policies_h_included

//	Define default sample index for MSAA
#ifndef	ISAMPLE
#define ISAMPLE 0
#endif	//	ISAMPLE

//	redefine sample index
#ifdef 	INLINE_MSAA_OPTIMIZATION
#undef	ISAMPLE
#define ISAMPLE	iSample
#endif	//	INLINE_MSAA_OPTIMIZATION

/////////////////////////////////////////////////////////////////////////////
// GLD_P - gbuffer_load_data
	#define	GLD_P( _tc, _pos2d, _iSample ) _tc, _pos2d, _iSample


/////////////////////////////////////////////////////////////////////////////
// CS_P
#ifdef MSAA_ANTIALIASING_ENABLE
#		define	CS_P( _P, _N, _tc0, _tcJ, _pos2d, _iSample ) _P, _N, _tc0, _tcJ, _pos2d, _iSample
#else
#		define	CS_P( _P, _N, _tc0, _tcJ, _pos2d, _iSample ) _P, _N, _tc0, _tcJ, _pos2d
#endif

#endif	//	common_policies_h_included
