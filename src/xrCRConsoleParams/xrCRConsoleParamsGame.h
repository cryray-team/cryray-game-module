#pragma once

#include "../xrCore/xrCore.h"
#include "../xrCore/PPMdType.h"

#ifdef XRCRCONSOLE_PARAMS_EXPORTS
#define XRCRCONSOLE_PARAMS_API __declspec(dllexport)
#else
#define XRCRCONSOLE_PARAMS_API __declspec(dllimport)
#endif

enum TypeCrosshair
{
    AF_CROSSHAIR_COLLIDE = (1 << 0),
    AF_CROSSHAIR_INERT = (1 << 1),
    AF_CROSSHAIR_STANDART = (1 << 2),
};

enum DefaultFlags
{
    HUD_CROSSHAIR = (1 << 0),
    HUD_CROSSHAIR_DIST = (1 << 1),
    HUD_WEAPON = (1 << 2),
    HUD_INFO = (1 << 3),
    HUD_DRAW = (1 << 4),
    HUD_CROSSHAIR_RT = (1 << 5),
    HUD_WEAPON_RT = (1 << 6),
    HUD_CROSSHAIR_DYNAMIC = (1 << 7),
    HUD_CROSSHAIR_RT2 = (1 << 9),
    HUD_DRAW_RT = (1 << 10),
    HUD_WEAPON_RT2 = (1 << 11),
    HUD_DRAW_RT2 = (1 << 12),
};

extern XRCRCONSOLE_PARAMS_API int fake_int;
extern XRCRCONSOLE_PARAMS_API int update_loot_pick;
extern XRCRCONSOLE_PARAMS_API int use_ammo_on_belt;
extern XRCRCONSOLE_PARAMS_API int wpn_auto_reload;
extern XRCRCONSOLE_PARAMS_API int r__nightvision;
extern XRCRCONSOLE_PARAMS_API int g_battery_use;

extern XRCRCONSOLE_PARAMS_API float minimap_zoom_factor;

extern XRCRCONSOLE_PARAMS_API float ai_aim_max_angle_medium;
extern XRCRCONSOLE_PARAMS_API float ai_aim_max_angle_hard;
extern XRCRCONSOLE_PARAMS_API float ai_aim_min_angle_medium;
extern XRCRCONSOLE_PARAMS_API float ai_aim_min_angle_hard;
extern XRCRCONSOLE_PARAMS_API float ai_aim_min_speed_medium;
extern XRCRCONSOLE_PARAMS_API float ai_aim_min_speed_hard;
extern XRCRCONSOLE_PARAMS_API float ai_aim_predict_time_medium;
extern XRCRCONSOLE_PARAMS_API float ai_aim_predict_time_hard;