#pragma once
#include "xrCryRayAPI.h"
#include "../xrCore/PPMdType.h"

enum
{
    TYPE_DEF = 0,
    TYPE_COP,
    TYPE_ANO,
    TYPE_ST174,
    TYPE_CS,
    TYPE_SOC,
    TYPE_VV,
    TYPE_LA,
    TYPE_MIS,
    TYPE_IWP,
};

extern XRCRYRAY_API u32 type_hud_token;
extern XRCRYRAY_API xr_token qhud_type_token[];

extern XRCRYRAY_API LPCSTR ArrayTypeHud[10]; //-' 0..9
extern XRCRYRAY_API LPCSTR ArrayTypeMiniMap[10]; //-' 0..9
extern XRCRYRAY_API LPCSTR ArrayTypeMotionIcons[10]; //-' 0..9

extern XRCRYRAY_API u32 token_weight;
extern XRCRYRAY_API xr_token token_weight_size[];

extern XRCRYRAY_API u32 music_type_menu;
extern XRCRYRAY_API xr_token music_type_menu_token[];