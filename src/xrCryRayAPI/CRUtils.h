#pragma once
#include "../xrCryRayAPI/xrCryRayAPI.h"

namespace CryRayUtils
{
    extern XRCRYRAY_API int GetNumOfDisplays();
    extern XRCRYRAY_API int GetRandomNumber(int min, int max);

    extern XRCRYRAY_API float fGetRandomNumber(float min, float max);

    extern XRCRYRAY_API DWORD GetWindowsVersionNumber();

    extern XRCRYRAY_API void CursorFakeClick();

    extern XRCRYRAY_API bool KeySlfl();
    extern XRCRYRAY_API bool IsEnableReShadeOverrideGraphics();

    constexpr auto STAGE_1 = "st_loading_saved_game_cryray_engine";
    constexpr auto STAGE_2 = "st_creating_new_game_cryray_engine";
    constexpr auto STAGE_3 = "st_loading_alife_simulator_cryray_engine";
    constexpr auto STAGE_5 = "st_client_synchronising_cryray_engine";
    constexpr auto STAGE_6 = "st_loading_ai_objects_cryray_engine";
    constexpr auto STAGE_7 = "st_client_spawning_cryray_engine";
    constexpr auto STAGE_8 = "st_loading_textures_cryray_engine";
    constexpr auto STAGE_9 = "st_client_synchronising_cryray_engine";
    constexpr auto STAGE_10 = "st_server_starting_cryray_engine";
    constexpr auto STAGE_11 = "st_loading_shaders_cryray_engine";
    constexpr auto STAGE_12 = "st_loading_geometry_cryray_engine";
    constexpr auto STAGE_13 = "st_loading_spatial_db_cryray_engine";
    constexpr auto STAGE_14 = "st_loading_details_cryray_engine";
    constexpr auto STAGE_16 = "st_loading_lights_cryray_engine";
    constexpr auto STAGE_18 = "st_loading_cform_cryray_engine";
}

class XRCRYRAY_API CRYRAY_API_ENV_GLOBAL
{
public:
#ifndef JSON_BUILD
    static int LevelID;

    std::string EngineModeToRender{}; //-' Отправить режим движка в рендер
    std::string CopyrightsCryRayEngine{}; //-' Встроенные коппирайты через расшифровку
    std::string GameVersionBuildCryRayEngine{}; //-' Версия движка в коде

    bool MTLogStart{};

    bool D3pda{}; //-' Включено ли 3д ПДА

    bool LogCoreWorkersMultiplier{}; //-' Вывод сообщений в лог по ttapi
    bool BlockCoreWorkersMultiplier{}; //-' Блокировщик для ttapi

    DWORD GetCoreWorkersMultiplier{}; //-' Для передачи параметра из Device.h в xrParticles

    bool AMD{}; //-' Видеокарта AMD
    bool Intel{}; //-' Видеокарта Intel
    bool NVIDIA{}; //-' Видеокарта NVIDIA

    bool IsImGuiActive{}; //-' Открыт ли ImGui, чтобы запретить движение камеры в демо рекорде

    static RECT winRect;

    static bool sounds_EFX;
    static bool sounds_caching;
    static bool ignore_verify;
    static bool g_actor_allow_ladder;
    static bool BDemoRecordActive;
    static bool InlineDirectXOldGamma; //-' DirectX гамма, отключена
    static bool bTotchOnSlot;
    static unsigned int iErrorsHSLS; //-' Кол-во ошибок в шейдерах на этапе компиляции
#endif
};

extern XRCRYRAY_API CRYRAY_API_ENV_GLOBAL CryRayAPI;