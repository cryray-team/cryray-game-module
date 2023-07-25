#pragma once

#define MTL_EXPORT_API
#define ENGINE_API
#define DLL_API

#define XR_MODULE "[xrPhysics]"
#include "xrCore/xrCore.h"
#define XR_MODULE "[xrPhysics]"

#include "xrcdb/xrcdb.h"
#include "xrsound/sound.h"

#include "xrPhysics.h"

#include "xrAPI/xrAPI.h"
#include "xrCryRayAPI/xrTokens.h"
#include "xrCryRayAPI/CRUtils.h"
#include "xrCryRayAPI/xrCryRayTokensEngine.h"

#ifdef DEBUG
#include "d3d9types.h"
#endif

class CGameMtlLibrary;
IC CGameMtlLibrary& GMLibrary()
{
    VERIFY(xrAPI.PGMLib);
    return *xrAPI.PGMLib;
}
