#pragma once

#include <regex>

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>

#pragma warning(disable:4995)
#include <intrin.h>
#pragma warning(default:4995)

#define ENGINE_API

#define XR_MODULE "[xrRenderParticles]"
#include "xrCore/xrCore.h"
#define XR_MODULE "[xrRenderParticles]"
#include "xrRenderAPI/SkeletonXVertRender.h"
#include "xrEngine/bone.h"
#define RENDER 1
#include "xrEngine/Render.h"
#include "xrEngine/Device.h"
#include "xrRenderAPI/light.h"

#include "xrRenderParticles.h"
#include "ttapi.h"
