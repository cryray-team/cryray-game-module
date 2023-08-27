#pragma once

#define MTL_EXPORT_API
#define ENGINE_API
#define DLL_API

#define XR_MODULE "[xrPhysics]"
#include "xrCore.h"
#define XR_MODULE "[xrPhysics]"

#include "xrcdb.h"
#include "sound.h"

#include "xrPhysics.h"

#include "xrAPI.h"
#include "xrTokens.h"
#include "CRUtils.h"
#include "xrCryRayTokensEngine.h"

#ifdef DEBUG
#include "d3d9types.h"
#endif

class CGameMtlLibrary;
IC CGameMtlLibrary& GMLibrary()
{
    VERIFY(xrAPI.PGMLib);
    return *xrAPI.PGMLib;
}
