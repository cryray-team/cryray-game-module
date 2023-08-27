#include "StdAfx.h"
#include "../xrEngine/Console/Console.h"
#include "../xrEngine/Console/Commands/ConsoleCommands.h"
#include "../xrEngine/customhud.h"
#include "../xrEngine/fdemorecord.h"
#include "../xrEngine/fdemoplay.h"
#include "xrMessages.h"
#include "xrserver.h"
#include "level.h"
#include "script_debugger.h"
#include "ai_debug.h"
#include "alife_simulator.h"
#include "game_cl_base.h"
#include "game_cl_single.h"
#include "game_sv_single.h"
#include "hit.h"
#include "PHDestroyable.h"
#include "actor.h"
#include "customzone.h"
#include "script_engine.h"
#include "script_engine_space.h"
#include "script_process.h"
#include "xrServer_Objects.h"
#include "ui/UIMainIngameWnd.h"
// #include "../xrphysics/PhysicsGamePars.h"
#include "../xrphysics/iphworld.h"
#include "string_table.h"
#include "ai_space.h"
#include "AI/monsters/BaseMonster/base_monster.h"
#include "date_time.h"
#include "mt_config.h"
//#include "ui/UIOptConCom.h"
#include "UIGameSP.h"
#include "ui/UIActorMenu.h"
#include "ui/UIStatic.h"
#include "zone_effector.h"
#include "GameTask.h"
#include "MainMenu.h"
#include "saved_game_wrapper.h"
#include "level_graph.h"
// #include "../xrEngine/resourcemanager.h"
// #include "../xrEngine/doug_lea_memory_allocator.h"
#include "cameralook.h"
#include "character_hit_animations_params.h"
#include "inventory_upgrade_manager.h"

#include "ai_debug_variables.h"
#include "../xrphysics/console_vars.h"

#include "..\xrCore\LocatorAPI.h"
#ifdef DEBUG
#include "PHDebug.h"
#include "ui/UIDebugFonts.h"
#include "game_graph.h"
#include "CharacterPhysicsSupport.h"
#endif // DEBUG

#include "../xrEngine\x_ray.h"
#include "../xrEngine/AutosaveManager.h"

extern string_path g_last_saved_game;

#include "../xrGameAPI/Console/xrGameConsole.h"

#ifndef ANOMALY
#include "../xrGameAPI_IWP/WeaponHudBase/player_hud.h"
#else

#endif

namespace ConsoleUtils
{
    bool valid_saved_game_name(LPCSTR file_name)
    {
        LPCSTR I = file_name;
        LPCSTR E = file_name + xr_strlen(file_name);
        for (; I != E; ++I)
        {
            if (!strchr("/\\:*?\"<>|^()[]%", *I))
                continue;

            return (false);
        };

        return (true);
    }

    void get_files_list(xr_vector<shared_str>& files, LPCSTR dir, LPCSTR file_ext)
    {
        VERIFY(dir && file_ext);
        files.xr_clear();

        FS_Path* P = FS.get_path(dir);
        P->m_Flags.set(FS_Path::flNeedRescan, TRUE);
        FS.m_Flags.set(CLocatorAPI::flNeedCheck, TRUE);
        FS.rescan_pathes();

        LPCSTR fext;
        STRCONCAT(fext, "*", file_ext);

        FS_FileSet files_set;
        FS.file_list(files_set, dir, FS_ListFiles, fext);
        u32 len_str_ext = xr_strlen(file_ext);

        FS_FileSetIt itb = files_set.begin();
        FS_FileSetIt ite = files_set.end();

        for (; itb != ite; ++itb)
        {
            LPCSTR fn_ext = (*itb).name.c_str();
            VERIFY(xr_strlen(fn_ext) > len_str_ext);
            string_path fn;
            strncpy_s(fn, sizeof(fn), fn_ext, xr_strlen(fn_ext) - len_str_ext);
            files.push_back(fn);
        }
        FS.m_Flags.set(CLocatorAPI::flNeedCheck, FALSE);
    }
}

namespace ConsoleCommands
{
	void CmdList() 
	{
        using namespace xrGameConsole;

        CMD1(CCC_MemStats, "stat_memory");

        CMD1(CCC_JumpToLevel, "jump_to_level");
        CMD1(CCC_ALifeSave, "save"); // save game
        CMD1(CCC_ALifeLoadFrom, "load"); // load game from ...
        CMD1(CCC_LoadLastSave, "load_last_save"); // load last saved game from ...
        CMD1(CCC_SetWeather, "set_weather");
        CMD1(CCC_StartTimerID, "task_test_start");
        CMD1(CCC_StopTimerID, "task_test_stop");
        CMD1(CCC_KillActor, "kill_actor");
        CMD1(CCC_LoadLastSaveA, "exe_lost_save");
        CMD1(CCC_UIMapUpdate, "rs_minimap_zoom_factor");
        CMD1(CCC_Spawn_to_inventory, "g_spawn_to_inventory");
        CMD1(CCC_Spawn, "g_spawn");
        CMD1(CCC_Nightvision, "r__nightvision");

        CMD1(CCC_Script, "run_script");
        CMD1(CCC_ScriptCommand, "run_string");

        CMD3(CCC_UiHud_Mode, "ui_hud_type", &type_hud_token, qhud_type_token);
        CMD3(CCC_Mask, "hud_disable_minimap", &psActorFlags, AF_DISABLE_MINIMAP);
        CMD3(CCC_Token, "xrGame_actor_portable_weight", &token_weight, token_weight_size);
        CMD3(CCC_Token, "xrGame_music_type_menu", &music_type_menu, music_type_menu_token);

        CMD3(CCC_Token, "type_crosshair", &type_crosshair, type_crosshair_token);
        CMD3(CCC_Token, "mode_crosshair", &ps_crosshair_mode, crosshair_mode_token);

        CMD4(CCC_Integer, "xrGame_autoreload_wpn", &wpn_auto_reload, 0, 1);
        CMD4(CCC_Integer, "update_loot_pick_soc", &update_loot_pick, 0, 1);
        CMD4(CCC_Integer, "use_ammo_on_belt", &use_ammo_on_belt, 0, 1);
        CMD4(CCC_Integer, "torch_battery_use", &g_battery_use, 0, 1);
        CMD4(CCC_Integer, "keypress_on_start", &g_keypress_on_start, 0, 1);

        CMD4(CCC_Float, "hud_fov", &psHUD_FOV_def, 0.1f, 1.0f);
        CMD4(CCC_Float, "fov", &g_fov, 45.0f, 90.0f);
        CMD4(CCC_Float, "scope_fov", &g_scope_fov, 0.45f, 0.85f);

        CMD4(CCC_Float, "head_bob_factor", &g_head_bob_factor, 0.f, 2.f); //-' Покачивание головы
	}
}