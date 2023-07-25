#pragma once

#define XR_MODULE "[xrSound]"
#include "xrCore/xrCore.h"
#include "../xrCryRayAPI/xrTokens.h"
#include <regex>
#include "../xrCryRayAPI/CRUtils.h"
#define XR_MODULE "[xrSound]"

// mmsystem.h
#define MMNOSOUND
#define MMNOMIDI
#define MMNOAUX
#define MMNOMIXER
#define MMNOJOY
#include <mmsystem.h>

// mmreg.h
#define NOMMIDS
#define NONEWRIFF
#define NOJPEGDIB
#define NONEWIC
#define NOBITMAP
#include <mmreg.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "xrCDB/xrCDB.h"
#include "sound.h"

#define ENGINE_API

#include "xrcore/xr_resource.h"

#include "parallel_for.h"
#include "parallel_for_each.h"
#include "blocked_range.h"
