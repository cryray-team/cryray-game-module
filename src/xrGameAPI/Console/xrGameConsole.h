#pragma once
#include "UIZoneMap.h"
#include "HUDManager.h"
#include "../LUA/TimerChainID.h"
#include "../xrThreads/ThreadsID.h"
using namespace ThreadManagerID;

#include <ctime>
#include <filesystem>
namespace fs = std::filesystem;

#include "Inventory.h"
#include "Torch.h"

extern string_path g_last_saved_game;

namespace ConsoleUtils
{
    bool valid_saved_game_name(LPCSTR file_name);
    void get_files_list(xr_vector<shared_str>& files, LPCSTR dir, LPCSTR file_ext);
}

namespace ConsoleCommands 
{
    extern void CmdList();
}

namespace xrGameConsole
{
    using namespace ConsoleUtils;
    
    class CCC_UIMapUpdate : public CCC_Float
    {
    public:
        CCC_UIMapUpdate(LPCSTR N) : CCC_Float(N, &minimap_zoom_factor, 0.1f, 4.0f){};

        virtual void Execute(const char* args)
        {
            minimap_zoom_factor = atof(args);

            CUIZoneMap* pZoneMap = CurrentGameUI() && CurrentGameUI()->UIMainIngameWnd ?
                CurrentGameUI()->UIMainIngameWnd->GetZoneMap() :
                nullptr;

            if (pZoneMap && pZoneMap->visible)
                pZoneMap->SetupCurrentMap();
        }
    };

    class CCC_Nightvision : public CCC_Integer
    {
    public:
        CCC_Nightvision(LPCSTR N) : CCC_Integer(N, &r__nightvision, 0, 3){};

        virtual void Execute(const char* args) override
        {
            static char* nv_effect[] = {"nightvision_1", "nightvision_2", "nightvision_3"};

            if (g_pGameLevel)
            {
                if (CryRayAPI.bTotchOnSlot && CTorch::m_nv_effect != "")
                {
                    int index = -1;
                    for (int i = 0; i < sizeof(nv_effect) / sizeof(nv_effect[0]); i++)
                    {
                        if (strcmp(nv_effect[i], CTorch::m_nv_effect.c_str()) == 0)
                        {
                            index = i;
                            break;
                        }
                    }

                    if ((index != -1 && atof(args) == index + 1) || atof(args) == 0)
                    {
                        r__nightvision = atof(args);
                    }
                    else
                    {
                        r__nightvision = 0;
                    }
                }
            }

        }

        virtual void Save(IWriter* F) override
        {
        
        }
    };

    class CCC_UiHud_Mode : public CCC_Token
    {
    public:
        CCC_UiHud_Mode(LPCSTR N, u32* V, xr_token* T) : CCC_Token(N, V, T){};

        virtual void Execute(LPCSTR args)
        {
            CCC_Token::Execute(args);

            if ((g_pGamePersistent && g_pGameLevel && Level().game))
            {
                if (*value >= 0 && *value <= 9)
                {
                    HUD().OnScreenResolutionChanged(); // перезагрузка окон
                    CurrentGameUI()->ReinitDialogs(); // реинит диалоговых окон вроде talk wnd
                }
            }
        }
    };

    class CCC_StartTimerID : public CCC_Integer
    {
    public:
        CCC_StartTimerID(LPCSTR N) : CCC_Integer(N, &fake_int, 0, 1){};

        virtual void Execute(const char* args) 
        { 
            auto l_start1 = [&]() 
            {
                int count = 0;
                while (count < 3)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    Msg("%s - %i", __FUNCTION__, count);
                    count++;
                }

                Msg("%s - end", __FUNCTION__);
            };

            auto l_start2 = [&]() 
            {
                int count = 0;
                while (count < 5)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    Msg("%s - %i", __FUNCTION__, count);
                    count++;
                }

                Msg("%s - end", __FUNCTION__);
            };

            auto l_start3 = [&]() {
                int count = 0;
                while (count < 8)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    Msg("%s - %i", __FUNCTION__, count);
                    count++;
                }

                Msg("%s - end", __FUNCTION__);
            };

            ThreadPool::enqueue(l_start1);
            ThreadPool::enqueue(l_start2);
            ThreadPool::enqueue(l_start3);
        }

        virtual void Save(IWriter* F) override {}
    };

    class CCC_StopTimerID : public CCC_Integer
    {
    public:
        CCC_StopTimerID(LPCSTR N) : CCC_Integer(N, &fake_int, 0, 2){};

        virtual void Execute(const char* args) 
        { 
            Msg("~ Unfinished. TODO");
        }

        virtual void Save(IWriter* F) override {}
    };

    class CCC_KillActor : public CCC_Integer
    {
    public:
        CCC_KillActor(LPCSTR N) : CCC_Integer(N, &fake_int, 0, 1){};

        virtual void Execute(const char* args)
        {
            auto l_start1 = [&]() 
            { 
                lua_G_CryRay::KillActorScript();
            };

            l_start1();
        }

        virtual void Save(IWriter* F) override {}
    };

    //-' OldSerpskiStalker
    class CCC_LoadLastSaveA : public IConsole_Command
    {
    public:
        CCC_LoadLastSaveA(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = true; }

        virtual void Execute(LPCSTR args) override
        {
            std::string directoryPath = "appdata\\savedgames";

            fs::directory_iterator end;
            fs::path latestFile;
            std::filesystem::file_time_type latestTimeType;

            for (fs::directory_iterator iter(directoryPath); iter != end; ++iter)
            {
                if (iter->is_regular_file() && iter->path().extension() == ".scoc")
                {
                    std::filesystem::file_time_type fileTimeType = fs::last_write_time(iter->path());

                    if (fileTimeType > latestTimeType)
                    {
                        latestTimeType = fileTimeType;
                        latestFile = iter->path();
                    }
                }
            }

            if (!latestFile.empty())
            {
                std::chrono::system_clock::time_point latestTimePoint
                {
                    std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        latestTimeType - fs::file_time_type::clock::now() + std::chrono::system_clock::now())
                };

                std::time_t latestTime = std::chrono::system_clock::to_time_t(latestTimePoint);
                const std::string latestFilePath = latestFile.string();
                std::string filename = latestFile.filename().string();
                std::string command = "load " + filename.substr(0, filename.length() - 5);
                Msg("~ Last file: %s", filename.c_str());
                Console->Execute(command.c_str());

                //CDialogHolder DialogHolder;
                //DialogHolder.CleanInternals();
            }
        }

        virtual void Save(IWriter* F) override
        {
            
        }
    };

    class CCC_SetWeather : public IConsole_Command
    {
    public:
        CCC_SetWeather(LPCSTR N) : IConsole_Command(N){};

        virtual void Execute(LPCSTR args)
        {
            if (!xr_strlen(args) || !g_pGamePersistent)
                return;

            g_pGamePersistent->Environment().SetWeather(args, true);
        }

        virtual void fill_tips(vecTips& tips, u32 mode)
        {
            if (!g_pGamePersistent)
                return;

            for (auto& [name, cycle] : g_pGamePersistent->Environment().WeatherCycles)
                tips.push_back(name);
        }
    };

    class CCC_Spawn_to_inventory : public IConsole_Command
    {
    public:
        CCC_Spawn_to_inventory(LPCSTR N) : IConsole_Command(N){};
        virtual void Execute(LPCSTR args)
        {
            if (!g_pGameLevel)
            {
                Log("Error: No game level!");
                return;
            }

            if (!pSettings->section_exist(args))
            {
                Msg("! Section [%s] isn`t exist...", args);
                return;
            }

            char Name[128];
            Name[0] = 0;
            sscanf(args, "%s", Name);

            Level().spawn_item(Name, Actor()->Position(), false, Actor()->ID());
        }
        virtual void Info(TInfo& I) { strcpy(I, "name,team,squad,group"); }
    };

    class CCC_Spawn : public IConsole_Command
    {
    public:
        CCC_Spawn(LPCSTR N) : IConsole_Command(N){};
        virtual void Execute(LPCSTR args)
        {
            if (!g_pGameLevel)
                return;

            // #ifndef	DEBUG
            if (GameID() != eGameIDSingle)
            {
                Msg("For this game type entity-spawning is disabled.");
                return;
            };
            // #endif

            if (!pSettings->section_exist(args))
            {
                Msg("! Section [%s] isn`t exist...", args);
                return;
            }

            char Name[128];
            Name[0] = 0;
            sscanf(args, "%s", Name);
            Fvector pos = Actor()->Position();
            pos.y += 3.0f;
            Level().g_cl_Spawn(Name, 0xff, M_SPAWN_OBJECT_LOCAL, pos);
        }
        virtual void Info(TInfo& I) { strcpy(I, "name,team,squad,group"); }
    };

    class CCC_ALifeSave : public IConsole_Command
    {
    public:
        CCC_ALifeSave(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = true; };

        virtual void Execute(LPCSTR args)
        {
            if (!g_actor || !Actor()->g_Alive())
            {
                Msg("cannot make saved game because actor is dead :(");
                return;
            }

            if (!pAutosaveManager->bAutosaveConsole)
                Console->Execute("stat_memory");

            string_path S, S1;
            S[0] = 0;
            strncpy_s(S, sizeof(S), args, _MAX_PATH - 1);

            CTimer timer;
            timer.Start();

            if (!xr_strlen(S))
            {
                static int last_index =
                    0; //-' Счетчик хранения индекса файла, для того чтобы не перезаписывать файлы ->
                //-' autosave_0, autosave_1, autosave_2 (Первый запуск)
                //-' autosave_3, autosave_4, autosave_5 (Перезапуск игры)
                //-' Продолжаем запись с последнего файла по статическому индексу
                LPCSTR game_saves_path = FS.get_path("$game_saves$")->m_Path;
                //- ...
                //-' OldSerpskiStalker
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                if (pAutosaveManager->bAutosaveConsole)
                {
                    auto GetLastSaveIndex = [](const std::string& game_saves_path) -> void {
                        // Поиск последнего файла с префиксом "autosave_" в указанной директории
                        WIN32_FIND_DATAA find_data;
                        HANDLE handle = FindFirstFileA((game_saves_path + "\\autosave_*.scop").c_str(), &find_data);

                        if (handle != INVALID_HANDLE_VALUE)
                        {
                            do
                            {
                                if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                                {
                                    //-' Извлечение индекса файла из его имени
                                    std::string file_name = find_data.cFileName;
                                    int prefix_length = strlen("autosave_");
                                    int index_length = file_name.length() - prefix_length - strlen(".scop");
                                    if (index_length > 0)
                                    {
                                        std::string index_str = file_name.substr(prefix_length, index_length);
                                        int index = std::stoi(index_str);
                                        last_index = std::max(last_index, index);
                                    }
                                }
                            } while (FindNextFileA(handle, &find_data));
                            FindClose(handle);
                        }
                    };

                    GetLastSaveIndex(game_saves_path);

                    char autosave_template[64];
                    sprintf(autosave_template, "autosave_%i", last_index + 1);
                    strconcat(sizeof(S), S, "", autosave_template);
                    pAutosaveManager->bAutosaveConsole = false;
                }
                else
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////
                    strconcat(sizeof(S), S, Core.UserName, " - ", "quicksave");

                NET_Packet net_packet;
                net_packet.w_begin(M_SAVE_GAME);
                net_packet.w_stringZ(S);
                net_packet.w_u8(0);
                Level().Send(net_packet, net_flags(TRUE));
            }
            else
            {
                if (!valid_saved_game_name(S))
                {
                    Msg("! Save failed: invalid file name - %s", S);
                    return;
                }

                NET_Packet net_packet;
                net_packet.w_begin(M_SAVE_GAME);
                net_packet.w_stringZ(S);
                net_packet.w_u8(1);
                Level().Send(net_packet, net_flags(TRUE));
            }

            Msg("Game save overhead  : %f milliseconds", timer.GetElapsed_sec() * 1000.f);

            SDrawStaticStruct* _s = CurrentGameUI()->AddCustomStatic("game_saved", true);
            LPSTR save_name;
            STRCONCAT(save_name, CStringTable().translate("st_game_saved").c_str(), ": ", S);
            _s->wnd()->TextItemControl()->SetText(save_name);

            xr_strcat(S, ".dds");
            FS.update_path(S1, "$game_saves$", S);

            timer.Start();

            MainMenu()->Screenshot(IRender_interface::SM_FOR_GAMESAVE, S1);

            Msg("Screenshot overhead : %f milliseconds", timer.GetElapsed_sec() * 1000.f);

        } // virtual void Execute

        virtual void fill_tips(vecTips& tips, u32 mode) { get_files_list(tips, "$game_saves$", SAVE_EXTENSION); }
    }; // CCC_ALifeSave

    class CCC_ALifeLoadFrom : public IConsole_Command
    {
    public:
        CCC_ALifeLoadFrom(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = true; };

        virtual void Execute(LPCSTR args)
        {
            string_path saved_game;
            strncpy_s(saved_game, sizeof(saved_game), args, _MAX_PATH - 1);

            if (!ai().get_alife())
            {
                Log("! ALife simulator has not been started yet");
                return;
            }

            if (!xr_strlen(saved_game))
            {
                Log("! Specify file name!");
                return;
            }

            if (!CSavedGameWrapper::saved_game_exist(saved_game))
            {
                Msg("! Cannot find saved game %s", saved_game);
                return;
            }

            if (!CSavedGameWrapper::valid_saved_game(saved_game))
            {
                Msg("! Cannot load saved game %s, version mismatch or saved game is corrupted", saved_game);
                return;
            }

            if (!valid_saved_game_name(saved_game))
            {
                Msg("! Cannot load saved game %s, invalid file name", saved_game);
                return;
            }

            if (MainMenu()->IsActive())
                MainMenu()->Activate(false);

            Console->Execute("stat_memory");

            if (Device.Paused())
                Device.Pause(FALSE, TRUE, TRUE, "CCC_ALifeLoadFrom");

            NET_Packet net_packet;
            net_packet.w_begin(M_LOAD_GAME);
            net_packet.w_stringZ(saved_game);
            Level().Send(net_packet, net_flags(TRUE));
        }

        virtual void fill_tips(vecTips& tips, u32 mode) { get_files_list(tips, "$game_saves$", SAVE_EXTENSION); }
    }; // CCC_ALifeLoadFrom

    class CCC_LoadLastSave : public IConsole_Command
    {
    public:
        CCC_LoadLastSave(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = true; }

        virtual void Execute(LPCSTR args)
        {
            string_path saved_game = "";
            if (args)
            {
                strncpy_s(saved_game, sizeof(saved_game), args, _MAX_PATH - 1);
            }

            if (saved_game && *saved_game)
            {
                xr_strcpy(g_last_saved_game, saved_game);
                return;
            }

            if (!*g_last_saved_game)
            {
                Msg("! cannot load last saved game since it hasn't been specified");
                return;
            }

            if (!CSavedGameWrapper::saved_game_exist(g_last_saved_game))
            {
                Msg("! Cannot find saved game %s", g_last_saved_game);
                return;
            }

            if (!CSavedGameWrapper::valid_saved_game(g_last_saved_game))
            {
                Msg("! Cannot load saved game %s, version mismatch or saved game is corrupted", g_last_saved_game);
                return;
            }

            if (!valid_saved_game_name(g_last_saved_game))
            {
                Msg("! Cannot load saved game %s, invalid file name", g_last_saved_game);
                return;
            }

            LPSTR command;
            if (ai().get_alife())
            {
                STRCONCAT(command, "load ", g_last_saved_game);
                Console->Execute(command);
                return;
            }

            STRCONCAT(command, "start server(", g_last_saved_game, "/single/alife/load)");
            Console->Execute(command);
        }

        virtual void Save(IWriter* F)
        {
            if (!*g_last_saved_game)
                return;

            F->w_printf("%s %s\r\n", cName, g_last_saved_game);
        }
    };

    class CCC_Script : public IConsole_Command
    {
    public:
        CCC_Script(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = false; };
        virtual void Execute(LPCSTR args)
        {
            if (!xr_strlen(args))
            {
                Log("* Specify script name!");
            }
            else
            {
                // rescan pathes
                FS_Path* P = FS.get_path("$game_scripts$");
                P->m_Flags.set(FS_Path::flNeedRescan, TRUE);
                FS.rescan_pathes();
                // run script
                if (ai().script_engine().script_process(ScriptEngine::eScriptProcessorLevel))
                    ai().script_engine()
                        .script_process(ScriptEngine::eScriptProcessorLevel)
                        ->add_script(args, false, true);
            }
        }

        virtual void Status(TStatus& S) { xr_strcpy(S, "<script_name> (Specify script name!)"); }
        virtual void Save(IWriter* F) {}

        virtual void fill_tips(vecTips& tips, u32 mode) { get_files_list(tips, "$game_scripts$", ".script"); }
    };

    class CCC_ScriptCommand : public IConsole_Command
    {
    public:
        CCC_ScriptCommand(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = false; };

        virtual void Execute(LPCSTR args)
        {
            if (!xr_strlen(args))
                Log("* Specify string to run!");
            else
            {
                if (ai().script_engine().script_process(ScriptEngine::eScriptProcessorLevel))
                {
                    ai().script_engine()
                        .script_process(ScriptEngine::eScriptProcessorLevel)
                        ->add_script(args, true, true);
                    return;
                }

                string4096 S;
                shared_str m_script_name = "console command";
                xr_sprintf(S, "%s\n", args);
                int l_iErrorCode = luaL_loadbuffer(ai().script_engine().lua(), S, xr_strlen(S), "@console_command");
                if (!l_iErrorCode)
                {
                    l_iErrorCode = lua_pcall(ai().script_engine().lua(), 0, 0, 0);
                    if (l_iErrorCode)
                    {
                        ai().script_engine().print_output(ai().script_engine().lua(), *m_script_name, l_iErrorCode);
                        ai().script_engine().on_error(ai().script_engine().lua());
                        return;
                    }
                }

                ai().script_engine().print_output(ai().script_engine().lua(), *m_script_name, l_iErrorCode);
            }
        } // void	Execute

        virtual void Status(TStatus& S)
        {
            xr_strcpy(S, "<script_name.function()> (Specify script and function name!)");
        }
        virtual void Save(IWriter* F) {}

        virtual void fill_tips(vecTips& tips, u32 mode)
        {
            if (mode == 1)
            {
                get_files_list(tips, "$game_scripts$", ".script");
                return;
            }

            IConsole_Command::fill_tips(tips, mode);
        }
    };

    struct CCC_JumpToLevel : public IConsole_Command
    {
        CCC_JumpToLevel(LPCSTR N) : IConsole_Command(N){};

        virtual void Execute(LPCSTR level)
        {
            if (!ai().get_alife())
            {
                Msg("! ALife simulator is needed to perform specified command!");
                return;
            }

            GameGraph::LEVEL_MAP::const_iterator I = ai().game_graph().header().levels().begin();
            GameGraph::LEVEL_MAP::const_iterator E = ai().game_graph().header().levels().end();
            for (; I != E; ++I)
                if (!xr_strcmp((*I).second.name(), level))
                {
                    ai().alife().jump_to_level(level);
                    return;
                }
            Msg("! There is no level \"%s\" in the game graph!", level);
        }

        virtual void Save(IWriter* F){};

        virtual void fill_tips(vecTips& tips, u32 mode)
        {
            if (!ai().get_alife())
            {
                Msg("! ALife simulator is needed to perform specified command!");
                return;
            }

            GameGraph::LEVEL_MAP::const_iterator itb = ai().game_graph().header().levels().begin();
            GameGraph::LEVEL_MAP::const_iterator ite = ai().game_graph().header().levels().end();
            for (; itb != ite; ++itb)
            {
                tips.push_back((*itb).second.name());
            }
        }
    };
 } // namespace xrGameConsole