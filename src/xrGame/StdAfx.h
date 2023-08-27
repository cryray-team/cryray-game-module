#pragma once
#include "FastDynamicCast.h"
#include <Typelist.h>

/**/
#define GP_EMAIL_LEN 128
#define GP_NICK_LEN 32
#define GP_UNIQUENICK_LEN 32
#define GP_PASSWORD_LEN 32
/**/

#pragma warning(push)
#pragma warning(disable : 4995)
#include "../Public/xrEngine/stdafx/stdafx.h"
#include "../Public/xrEngine/defines.h"
#include "../xrEngine/NetServer/dplay8.h"
#pragma warning(pop)
#define XR_MODULE "[xrGame]"
#pragma warning(4 : 4018)
#pragma warning(4 : 4244)
#pragma warning(disable : 4505)

#pragma warning(disable : 4251)
#pragma warning(disable : 4127)
#pragma warning(disable : 4100)
#pragma warning(disable : 4201)

#include "../xrCRConsoleParams/xrCRConsoleParamsRender.h"
#include "../xrCRConsoleParams/xrCRConsoleParamsEngine.h"
#include "../xrCRConsoleParams/xrCRConsoleParamsGame.h"

#include "LUA/lua_G_CryRay.h"
#include "../xrCryRayAPI/xrCryRayTokensGame.h"

#include "../Intel_TBB/parallel_for.h"
#include "../Intel_TBB/blocked_range.h"

#include "../xrGameAPI/Actor/ActorFlags.h"

// AVO: print, don't fail
#include "../build_engine_config.h"
#ifdef NON_FATAL_VERIFY
#define THROW VERIFY
#define THROW2 VERIFY2
#define THROW3 VERIFY3
#elif XRAY_EXCEPTIONS
#define THROW(expr)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expr))                                                                                                   \
        {                                                                                                              \
            string4096 assertion_info;                                                                                 \
            ::Debug.gather_info(_TRE(#expr), 0, 0, 0, DEBUG_INFO, assertion_info);                                     \
            throw assertion_info;                                                                                      \
        }                                                                                                              \
    } while (0)
#define THROW2(expr, msg0)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expr))                                                                                                   \
        {                                                                                                              \
            string4096 assertion_info;                                                                                 \
            ::Debug.gather_info(_TRE(#expr), msg0, 0, 0, DEBUG_INFO, assertion_info);                                  \
            throw assertion_info;                                                                                      \
        }                                                                                                              \
    } while (0)
#define THROW3(expr, msg0, msg1)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expr))                                                                                                   \
        {                                                                                                              \
            string4096 assertion_info;                                                                                 \
            ::Debug.gather_info(_TRE(#expr), msg0, msg1, 0, DEBUG_INFO, assertion_info);                               \
            throw assertion_info;                                                                                      \
        }                                                                                                              \
    } while (0)
#else
#define THROW VERIFY
#define THROW2 VERIFY2
#define THROW3 VERIFY3
#endif

#include "../xrEngine/gamefont.h"
#include "../xrEngine/xr_object.h"
#include "../xrEngine/igame_level.h"
#include "xrphysics.h"

#include "../xrGameAPI/Namespaces_unity/namescape_monster_details.h"
#include "../xrGameAPI/Namespaces_unity/global_namespaces.h"