#pragma once

namespace
{
    template <typename T>
    void InitConfig(T& config, const char* name, bool fatal = true, bool readOnly = true, bool loadAtStart = true,
        bool saveAtEnd = true, u32 sectCount = 0, const CInifile::allow_include_func_t& allowIncludeFunc = nullptr)
    {
        string_path fname;
        FS.update_path(fname, "$game_config$", name);
        config = xr_new<CInifile>(fname, readOnly, loadAtStart, saveAtEnd, sectCount, allowIncludeFunc);

        CHECK_OR_EXIT(config->section_count() || !fatal, make_string("Cannot find file %s.", fname));
    }

    void InitCryRayConfig() 
    { 
        InitConfig(pCryRaySettings, "cryray_engine_config.ltx", false, true, true, false);
    }

    void InitCryRayConfigSettings()
    {
        LPCSTR GameMode = pCryRaySettings->r_string("compatibility", "engine_mode");

        CryRayAPI.EngineModeToRender = GameMode;

        auto l_OtherSettings = []
        {
            LPCSTR DisableBootScreens = pCryRaySettings->r_string("render_cryray", "disable_boot_screens");

            if (xr_strcmp("false", DisableBootScreens) == 0)
            {
                pSGameSettings->bDisableBootScreens = false;
            }
            else
            {
                pSGameSettings->bDisableBootScreens = true;
            }

            Fvector3 NightvisionGammaAmb = pCryRaySettings->r_fvector3("render_cryray", "nightvision_gamma_ambient");
            pSGameSettings->vNightvisionGammaAmbient = NightvisionGammaAmb.set(NightvisionGammaAmb);

            Fvector4 NightvisionGammaHemi = pCryRaySettings->r_fvector4("render_cryray", "nightvision_gamma_hemi");
            pSGameSettings->vNightvisionGammaHemi = NightvisionGammaHemi.set(NightvisionGammaHemi);

            LPCSTR LoadLevelAmbientsSounds = pCryRaySettings->r_string("gameplay", "load_level_ambients_sounds");

            if (xr_strcmp("false", LoadLevelAmbientsSounds) == 0)
            {
                pSGameSettings->bLoadLevelAmbientsSounds = false;
            }
            else
            {
                pSGameSettings->bLoadLevelAmbientsSounds = true;
            }

            LPCSTR MoreArtsSlots = pCryRaySettings->r_string("gameplay", "artefacts_slot_16");

            if (xr_strcmp("false", MoreArtsSlots) == 0)
            {
                pSGameSettings->bMoreArtefactsSlots = false;
            }
            else
            {
                pSGameSettings->bMoreArtefactsSlots = true;
            }

            LPCSTR EnableNewSleep = pCryRaySettings->r_string("gameplay", "enable_sleeping");

            if (xr_strcmp("false", EnableNewSleep) == 0)
            {
                pSGameSettings->bEnableSleep = false;
            }
            else
            {
                pSGameSettings->bEnableSleep = true;
            }

            LPCSTR EnableNewThirst = pCryRaySettings->r_string("gameplay", "enable_thists");

            if (xr_strcmp("false", EnableNewThirst) == 0)
            {
                pSGameSettings->bEnableThirst = false;
            }
            else
            {
                pSGameSettings->bEnableThirst = true;
            }
        };

        if (xr_strcmp("ano_1_5_1", GameMode) == 0)
        {
            ANO_mode = true;
        }
        else if (xr_strcmp("redux_1_1", GameMode) == 0)
        {
            ANO_mode = true;
            REDUX_mode = true;
        }
        else if(xr_strcmp("iwp", GameMode) == 0)
        {
            IWP_mode = true;
            ANO_mode = false;
            REDUX_mode = false;
        }

        l_OtherSettings();
    }
} // namespace