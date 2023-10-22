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

#ifndef	ISAMPLE
#define ISAMPLE 0
#endif

#define	GLD_P( _tc, _pos2d, _iSample ) _tc, _pos2d, _iSample

#define	CS_P( _P, _N, _tc0, _tcJ, _pos2d, _iSample ) _P, _N, _tc0, _tcJ, _pos2d

#endif	//	common_policies_h_included
