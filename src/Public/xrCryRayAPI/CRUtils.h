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
    std::string EngineModeToRender{}; //-' ��������� ����� ������ � ������
    std::string CopyrightsCryRayEngine{}; //-' ���������� ���������� ����� �����������
    std::string GameVersionBuildCryRayEngine{}; //-' ������ ������ � ����

    bool MTLogStart{};

    bool D3pda{}; //-' �������� �� 3� ���

    bool AMD{}; //-' ���������� AMD
    bool Intel{}; //-' ���������� Intel
    bool NVIDIA{}; //-' ���������� NVIDIA

    bool IsImGuiActive{}; //-' ������ �� ImGui, ����� ��������� �������� ������ � ���� �������

    static RECT winRect;

    static int LevelID;

    static bool ignore_verify;
    static bool g_actor_allow_ladder;
    static bool BDemoRecordActive;
    static bool bTotchOnSlot;
    static bool bHelmetAllowed;
    static bool bBackpackAllowed;
    static bool sounds_EFX;
    static bool sounds_caching;

    static float fStaminaMinus;
    static float fLuminocityHemi;
    static unsigned int iErrorsHSLS;
#endif
};

extern XRCRYRAY_API CRYRAY_API_ENV_GLOBAL CryRayAPI;