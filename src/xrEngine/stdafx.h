#pragma once
#define XR_MODULE "[xrEngine]"

#include "../build_engine_config.h"

#include "../xrCore/xrCore.h"
#include "../xrAPI/xrAPI.h"
#include "../xrCryRayAPI/CRUtils.h"
#include "../xrCryRayAPI/EngineModes.h"
#include "../xrReShadeAPI/xrReShadeAPI.h"
#include "../xrCryRayAPI/templates.h"
#include "../xrParser/xrParser.h"
#include "../xrCryRayAPI/CRImGuiCR.h"

#ifdef _DEBUG
# define D3D_DEBUG_INFO
#endif

#pragma warning(push)
#pragma warning(disable:4995)
#include <d3d9.h>
#pragma warning(pop)

// you must define ENGINE_BUILD then building the engine itself
// and not define it if you are about to build DLL
#ifndef NO_ENGINE_API
#ifdef ENGINE_BUILD
#define DLL_API __declspec(dllimport)
#define ENGINE_API __declspec(dllexport)
#else
#ifdef SPECTRE
#define GAME_API __declspec(dllimport)
#else
#define GAME_API __declspec(dllexport)
#endif
#undef DLL_API
#define DLL_API __declspec(dllexport)
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#define DLL_API
#endif // NO_ENGINE_API

#include "../xrCRConsoleParams/xrCRConsoleParamsRender.h"
#include "../xrCRConsoleParams/xrCRConsoleParamsEngine.h"
#include "../xrCryRayAPI/xrCryRayTokensEngine.h"
#include "../xrCRConsoleParams/xrCRConsoleParamsGame.h"
#include "../xrCryRayAPI/xrTokens.h"

// Our headers
#include "Engine.h"
#include "defines.h"
#ifndef NO_XRLOG
#include "../xrcore/log.h"
#endif
#include "device.h"
#include "../xrcore/fs.h"

#include "../xrcdb/xrXRC.h"

#include "../xrSound/sound.h"

#include "xrImGui.h"
#include "StructCryRay.h"
#include "GameStructSettings.h"

extern ENGINE_API CInifile* pGameIni;

#ifndef SPECTRE
#pragma comment(lib, "lua51.lib" )

#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

#include "lua.hpp"

#pragma warning(push)
#pragma warning(disable:4995)
#include <luabind/luabind.hpp>
#pragma warning(pop)

#include <luabind/object.hpp>
#include <luabind/functor.hpp>
#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/iterator_policy.hpp>

#ifndef DEBUG
# define LUABIND_NO_ERROR_CHECKING
#endif

#if !defined(DEBUG) || defined(FORCE_NO_EXCEPTIONS)
# define LUABIND_NO_EXCEPTIONS
# define BOOST_NO_EXCEPTIONS
#endif

#define LUABIND_DONT_COPY_STRINGS
#endif

#define READ_IF_EXISTS(ltx,method,section,name,default_value)\
 (((ltx)->line_exist(section, name)) ? ((ltx)->method(section, name)) : (default_value))