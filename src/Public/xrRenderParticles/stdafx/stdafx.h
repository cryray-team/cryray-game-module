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
#include "xrCore.h"
#define XR_MODULE "[xrRenderParticles]"
#include "SkeletonXVertRender.h"
#include "bone.h"
#define RENDER 1
#include "Render.h"
#include "Device.h"
#include "light.h"

#include "xrRenderParticles.h"
#include "ttapi.h"
