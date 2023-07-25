#pragma once

#include "../xrCore/xrCore.h"
#include "../xrCore/PPMdType.h"

#ifdef XRCRCONSOLE_PARAMS_EXPORTS
#define XRCRCONSOLE_PARAMS_API __declspec(dllexport)
#else
#define XRCRCONSOLE_PARAMS_API __declspec(dllimport)
#endif

//-' Для консольных команд:
enum
{
    AI_IGNORE_ACTOR = (1 << 0),
    RS_DISCORD_RPC_ACTIVE_DEFAULT = (1 << 1),
    RS_DISCORD_RPC_ACTIVE_ANOMALY = (1 << 2),
    RS_KEYSTART_PRESS = (1 << 3),
    RS_COD_PICKUP = (1 << 4),
    RS_FEEL_GRENADE = (1 << 5),
    RS_AIM_SWAY = (1 << 6),
    RS_ALWAYS_ACTIVE = (1 << 7),
};

//-' Группировки ГГ:
enum
{
    E_STALKER = (1 << 0),
    E_BANDIT = (1 << 1),
    E_CSKY = (1 << 2),
    E_DOLG = (1 << 3),
    E_FREEDOM = (1 << 4),
    E_KILLER = (1 << 5),
    E_ARMY = (1 << 6),
    E_ECOLOG = (1 << 7),
    E_MONOLITH = (1 << 8),
};

//-' Глубина резкости
enum
{
    AF_RELOAD_DOF = (1 << 9),
    AF_ZOOM_DOF = (1 << 10),
    AF_HUD_ANIMATOR_DOF = (1 << 11),
    AF_WEATHER_DOF = (1 << 12),
};

enum
{
    //-' Флаг больше реализма для BttR:
    // FLAG_MORE_REALISM                       = (1 << 3),
    //-' Убор рук, при открытом инвентаре
    AF_HAND_HIDE_INVENTORY = (1 << 13),
    // AF_HAND_HIDE_WITH_RUCK                  = (1 << 14),
	/**/
    //-' Система повреждения из 7days
    FLAG_7DAYS_HITS = (1 << 15),
    //-' Опции DoF: при перезарядке, зум, худ аниматор, погода
};

extern XRCRCONSOLE_PARAMS_API float g_fov;
extern XRCRCONSOLE_PARAMS_API float g_scope_fov;
extern XRCRCONSOLE_PARAMS_API float psHUD_FOV_def;
extern XRCRCONSOLE_PARAMS_API float psHUD_FOV;

//-' Параметра дождя:
extern XRCRCONSOLE_PARAMS_API float max_distance_rain_addition;
extern XRCRCONSOLE_PARAMS_API float distance_source_rain_addition;

extern XRCRCONSOLE_PARAMS_API Flags32 CryRayFlags32;

extern XRCRCONSOLE_PARAMS_API Fvector2 c_font_scale;
extern XRCRCONSOLE_PARAMS_API Fvector2 g_current_font_scale;