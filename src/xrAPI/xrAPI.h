#pragma once

#include "../xrCore/PPMdType.h"

#ifdef XRAPI_EXPORTS
#define XRAPI_API __declspec(dllexport)
#else
#define XRAPI_API __declspec(dllimport)
#endif

struct xr_token;
class IRender_interface;
class IRenderFactory;
class CDUInterface;
class IUIRender;
class CGameMtlLibrary;
class IDebugRender;
class ISoundManager;
class CSoundRender_Core;

namespace XRAPI_ENV
{
	class XRAPI_API XRAPI_ENVIRONMENT
	{
	public:
		IRender_interface*		Render;
		IRenderFactory*			RenderFactory;
		CDUInterface*			DU;
		xr_token*				vid_mode_token;
		IUIRender*				UIRender;
		CGameMtlLibrary*		PGMLib;
		IDebugRender*			DRender;
		ISoundManager*			Sound;
		CSoundRender_Core*		SoundRender;
        xr_token*				vid_quality_token;
	};
}
extern XRAPI_API XRAPI_ENV::XRAPI_ENVIRONMENT xrAPI;