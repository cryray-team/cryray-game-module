#pragma once
#include "xrCryRayAPI.h"
#include "../xrCore/PPMdType.h"

enum
{
    COLLSION_FULL = 2,
    COLLISION_ACTOR = 1,
    COLLISION_NULL = 0,
};

extern XRCRYRAY_API u32 g_dead_body_collision;
extern XRCRYRAY_API xr_token dead_body_collision_tokens[];

enum
{
    CROSSHAIR_COP = 0,
    CROSSHAIR_BUILD_SOC = 1,
    CROSSHAIR_IWP = 2,
    CROSSHAIR_FALLEN_STAR = 3,
};

extern XRCRYRAY_API u32 type_crosshair;
extern XRCRYRAY_API xr_token type_crosshair_token[];

enum
{
    STANDART_CROSSHAIR = 0,
    COLLIDE_CROSSHAIR = 1,
    INERTION_CROSSHAIR = 2,
};

extern XRCRYRAY_API u32 ps_crosshair_mode;
extern XRCRYRAY_API xr_token crosshair_mode_token[];

enum
{
    WINDOWED = 1,
    WINDOWED_BORDERLESS = 2,
    FULLSCREEN = 3,
};

extern XRCRYRAY_API u32 ps_vid_windowtype;
extern XRCRYRAY_API xr_token vid_windowtype_token[];