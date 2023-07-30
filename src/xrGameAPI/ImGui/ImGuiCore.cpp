#include "StdAfx.h"
#include "ImGuiCore.h"

const char* empty = "";

float editor_longitude = 0.0f;
float editor_altitude = 0.0f;

CUIWindow* curWnd = nullptr;

#ifdef ANOMALY
UIMiniMap::CUIMiniMap* curWnd_p = nullptr;
xr_map<UIMiniMap::CUIMiniMap*, bool> collapsed_p;
#else
CUIMiniMap* curWnd_p = nullptr;
xr_map<CUIMiniMap*, bool> collapsed_p;
#endif

xr_map<CUIWindow*, bool> collapsed;

xr_set<shared_str> modifiedWeathers;

CImGuiCore::EditorStage stage{};

CImGuiCore* ImGuiCore = nullptr;

CImGuiCore::CImGuiCore()
{ 
    ImGuiHelpWnd = false;
    ImGuiWeatherEditor = false;
    ImGuiUIEditorHelper = false;
    ImGuiHelpWndDis = IsDebuggerPresent() ? true : false;

	stage = EditorStage::None;
}

CImGuiCore::~CImGuiCore() {}

void CImGuiCore::ShowMain()
{
    ImguiWnd wnd("Main");

    if (wnd.Collapsed)
        return;

    CryRayAPI.IsImGuiActive = true;

    ImGui::Text(u8"CryRay Engine x64");

    if (ImGui::Button("Imgui Help") && ImGuiHelpWndDis)
        ImGuiHelpWnd ^= 1;

    if (ImGui::Button("Weather Editor"))
        ImGuiWeatherEditor ^= 1;

    if (ImGui::Button("UI Editor"))
        ImGuiUIEditorHelper = !ImGuiUIEditorHelper;

    bool full = stage == EditorStage::Full;

    if (ImGui::Checkbox("Active", &full))
        stage = full ? EditorStage::Full : EditorStage::Light;

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void CImGuiCore::ShowEditor()
{
    if (!IsEditor())
        return;

    if (IsEditorActive())
        xrImGui::EnableInputOnFrame();

    ShowMain();

    if (ImGuiWeatherEditor)
        ShowWeatherEditor(ImGuiWeatherEditor);

    if (ImGuiUIEditorHelper)
        UIShowEditor(ImGuiUIEditorHelper);

    if (ImGuiHelpWnd && ImGuiHelpWndDis)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow(&ImGuiHelpWnd);
    }
}

bool CImGuiCore::IsEditor() 
{
	return stage != EditorStage::None; 
}

bool CImGuiCore::IsEditorActive()
{
    return stage == EditorStage::Full || (stage == EditorStage::Light);
}

bool CImGuiCore::EditorKeyPressCallback(int key)
{
    if (key == DIK_F10)
    {
        stage = static_cast<EditorStage>((static_cast<int>(stage) + 1) % static_cast<int>(EditorStage::Count));
        Msg("~ [%s]: %i", __FUNCTION__, stage);
    }
    else if (key == DIK_ESCAPE)
    {
        stage = EditorStage::None;
    }
    else if (stage == EditorStage::None)
    {
        CryRayAPI.IsImGuiActive = false;
    }

    if (IsEditor() && !IsEditorActive())
        return true;

    return false;
}

void CImGuiCore::SaveWeather(shared_str name, const xr_vector<CEnvDescriptor*>& env)
{
    CInifile f(nullptr, FALSE, FALSE, FALSE);

    for (auto el : env)
    {
        f.w_string(el->m_identifier.c_str(),    "ambient", el->env_ambient->name().c_str());
        f.w_fvector3(el->m_identifier.c_str(),  "ambient_color", el->ambient);
        f.w_fvector4(el->m_identifier.c_str(),  "hemisphere_color", el->hemi_color);
        f.w_fvector4(el->m_identifier.c_str(),  "flora_hemi_color", el->flora_hemi);
        f.w_float(el->m_identifier.c_str(),     "contrast_depth", el->m_fColorCorrection);
        f.w_float(el->m_identifier.c_str(),     "HDR_amount", el->fHDR);
        f.w_float(el->m_identifier.c_str(),     "bloom_treshold", el->fBloomTreshold);
        f.w_float(el->m_identifier.c_str(),     "bloom_saturation", el->fBloomSaturation);
        f.w_fvector4(el->m_identifier.c_str(),  "clouds_color", el->clouds_color);
        f.w_float(el->m_identifier.c_str(),     "clouds_velocity_0", el->clouds_velocity_0);
        f.w_float(el->m_identifier.c_str(),     "clouds_velocity_1", el->clouds_velocity_1);
        f.w_string(el->m_identifier.c_str(),    "clouds_texture", el->clouds_texture_name.c_str());
        f.w_fvector3(el->m_identifier.c_str(),  "sky_color", el->sky_color);
        f.w_float(el->m_identifier.c_str(),     "sky_rotation", rad2deg(el->sky_rotation));
        f.w_string(el->m_identifier.c_str(),    "sky_texture", el->sky_texture_name.c_str());
        f.w_string(el->m_identifier.c_str(),    "sun", el->lens_flare_id.c_str());
        f.w_fvector3(el->m_identifier.c_str(),  "sun_color", el->sun_color);
        f.w_float(el->m_identifier.c_str(),     "sun_altitude", rad2deg(el->sun_dir.getH()));
        f.w_float(el->m_identifier.c_str(),     "sun_longitude", rad2deg(el->sun_dir.getP()));
        f.w_float(el->m_identifier.c_str(),     "sun_shafts_intensity", el->m_fSunShaftsIntensity);
        f.w_float(el->m_identifier.c_str(),     "far_plane", el->far_plane);
        f.w_fvector3(el->m_identifier.c_str(),  "fog_color", el->fog_color);
        f.w_float(el->m_identifier.c_str(),     "fog_density", el->fog_density);
        f.w_float(el->m_identifier.c_str(),     "fog_distance", el->fog_distance);
        f.w_float(el->m_identifier.c_str(),     "dof_kernel", el->dof_kernel);
        f.w_float(el->m_identifier.c_str(),     "dof_far_vector3", el->dof_far_plus);
        f.w_float(el->m_identifier.c_str(),     "fog_height", el->m_fFogShadersHeight);
        f.w_float(el->m_identifier.c_str(),     "fog_max_dist", el->m_fFogShadersMaxDist);
        f.w_float(el->m_identifier.c_str(),     "fog_ground_density", el->m_fFogShadersDensity);
        f.w_fvector3(el->m_identifier.c_str(),  "rain_color", el->rain_color);
        f.w_float(el->m_identifier.c_str(),     "rain_density", el->rain_density);
        f.w_float(el->m_identifier.c_str(),     "rain_angle", el->rain_angle);
        f.w_float(el->m_identifier.c_str(),     "rain_length", el->rain_length);
        f.w_float(el->m_identifier.c_str(),     "rain_width", el->rain_width);
        f.w_float(el->m_identifier.c_str(),     "rain_speed_max", el->rain_speed_max);
        f.w_float(el->m_identifier.c_str(),     "rain_speed_min", el->rain_speed_min);
        f.w_float(el->m_identifier.c_str(),     "rain_volumeco", el->rain_volume_coefficient);
        f.w_float(el->m_identifier.c_str(),     "rain_aanglec", el->rain_additional_angle_coefficient);
        f.w_string(el->m_identifier.c_str(),    "thunderbolt_collection", el->tb_id.c_str());
        f.w_float(el->m_identifier.c_str(),     "thunderbolt_duration", el->bolt_duration);
        f.w_float(el->m_identifier.c_str(),     "thunderbolt_period", el->bolt_period);        
        f.w_float(el->m_identifier.c_str(),     "water_intensity", el->m_fWaterIntensity);
        f.w_float(el->m_identifier.c_str(),     "wet_surface_factor", el->m_fWetSurfaces);      
        f.w_float(el->m_identifier.c_str(),     "wind_velocity", el->wind_velocity);
        f.w_float(el->m_identifier.c_str(),     "wind_direction", rad2deg(el->wind_direction));
        f.w_float(el->m_identifier.c_str(),     "wind_sound_volume", el->wind_volume);
        f.w_float(el->m_identifier.c_str(),     "tree_amplitude_intensity", el->m_fTreeAmplitudeIntensity);
        f.w_float(el->m_identifier.c_str(),     "volumetric_intensity_factor", el->volumetric_intensity_factor);
        f.w_float(el->m_identifier.c_str(),     "volumetric_distance_factor", el->volumetric_distance_factor);
        f.w_float(el->m_identifier.c_str(),     "swing_normal_amp1", el->m_cSwingDesc[0].amp1);
        f.w_float(el->m_identifier.c_str(),     "swing_normal_amp2", el->m_cSwingDesc[0].amp2);
        f.w_float(el->m_identifier.c_str(),     "swing_normal_rot1", el->m_cSwingDesc[0].rot1);
        f.w_float(el->m_identifier.c_str(),     "swing_normal_rot2", el->m_cSwingDesc[0].rot2);
        f.w_float(el->m_identifier.c_str(),     "swing_normal_speed", el->m_cSwingDesc[0].speed);
        f.w_float(el->m_identifier.c_str(),     "swing_fast_amp1", el->m_cSwingDesc[1].amp1);
        f.w_float(el->m_identifier.c_str(),     "swing_fast_amp2", el->m_cSwingDesc[1].amp2);
        f.w_float(el->m_identifier.c_str(),     "swing_fast_rot1", el->m_cSwingDesc[1].rot1);
        f.w_float(el->m_identifier.c_str(),     "swing_fast_rot2", el->m_cSwingDesc[1].rot2);
        f.w_float(el->m_identifier.c_str(),     "swing_fast_speed", el->m_cSwingDesc[1].speed);
    }

    string_path fileName;
    FS.update_path(fileName, "$game_weathers$", name.c_str());
    strconcat(sizeof(fileName), fileName, fileName, ".ltx");
    f.save_as(fileName);
}

void CImGuiCore::NextTexture(char* tex, int texSize, int offset)
{
    string_path dir, fn;
    _splitpath(tex, nullptr, dir, fn, nullptr);
    strconcat(sizeof(fn), fn, fn, ".dds");
    xr_vector<LPSTR>* files = FS.file_list_open("$game_textures$", dir, FS_ListFiles);

    if (!files)
        return;

    size_t index = 0;
    for (size_t i = 0; i != files->size(); i++)
    {
        if (strcmp((*files)[i], fn) == 0)
        {
            index = i;
            break;
        }
    }

    size_t newIndex = index;

    while (true)
    {
        newIndex = (newIndex + offset + files->size()) % files->size();
        if (strstr((*files)[newIndex], "#small") == nullptr && strstr((*files)[newIndex], ".thm") == nullptr)
            break;
    }

    string_path newFn;
    _splitpath((*files)[newIndex], nullptr, nullptr, newFn, nullptr);
    strconcat(texSize, tex, dir, newFn);
    FS.file_list_close(files);
}

bool CImGuiCore::EditTexture(const char* label, shared_str& texName)
{
    char tex[100];
    strncpy(tex, texName.data(), 100);
    bool changed = false;
    static shared_str prevValue;
    ImGui::PushID(label);

    if (ImGui::InputText("", tex, 100))
    {
        texName = tex;
        changed = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("..."))
    {
        ImGui::OpenPopup("Choose texture");
        prevValue = texName;
    }

    ImGui::SameLine();

    ImGui::Text(label);
    ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);

    if (ImGui::BeginPopupModal("Choose texture", NULL, 0))
    {
        string_path dir, fn;
        _splitpath(tex, nullptr, dir, fn, nullptr);
        strconcat(sizeof(fn), fn, fn, ".dds");
        static xr_map<xr_string, xr_vector<xr_string>> dirs;
        auto filtered = dirs[dir];

        if (filtered.empty())
        {
            xr_vector<LPSTR>* files = FS.file_list_open("$game_textures$", dir, FS_ListFiles);
            if (files)
            {
                filtered.resize(files->size());
                auto e = std::copy_if(files->begin(), files->end(), filtered.begin(),
                    [](auto x) { return strstr(x, "#small") == nullptr && strstr(x, ".thm") == nullptr; });
                filtered.resize(e - filtered.begin());
                std::sort(filtered.begin(), filtered.end(),
                    [](auto a, auto b) { return CompareNaturall(a.c_str(), b.c_str()) < 0; });
                dirs[dir] = filtered;
            }
            FS.file_list_close(files);
        }

        int cur = -1;
        for (size_t i = 0; i != filtered.size(); i++)
        {
            if (filtered[i] == fn)
            {
                cur = i;
                break;
            }
        }

        if (ImGui_ListBox(
                "", &cur,
                [](void* data, int idx, const char** out_text) -> bool {
                    xr_vector<xr_string>* textures = (xr_vector<xr_string>*)data;
                    *out_text = (*textures)[idx].c_str();
                    return true;
                },
                &filtered, filtered.size(), ImVec2(-1.0f, -20.0f)))
        {
            string_path newFn;
            _splitpath(filtered[cur].c_str(), nullptr, nullptr, newFn, nullptr);
            strconcat(100, tex, dir, newFn);
            texName = tex;
            changed = true;
        }

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            string_path newFn;
            _splitpath(prevValue.data(), nullptr, nullptr, newFn, nullptr);
            strconcat(100, tex, dir, newFn);
            texName = tex;
            changed = true;
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();

    return changed;
}

void CImGuiCore::ShowWeatherEditor(bool& show)
{
    if (!ImGui::Begin(modifiedWeathers.empty() ? "Weather###Weather" : "Weather*###Weather", &show))
    {
        ImGui::End();
        return;
    }

    auto l_EnumCycle = [](void* data, int idx, const char** item) -> bool 
    {
        xr_vector<shared_str>* cycles = (xr_vector<shared_str>*)data;
        *item = (*cycles)[idx].c_str();

        return true;
    };

    auto l_EnumWeather = [](void* data, int idx, const char** item) -> bool 
    {
        xr_vector<CEnvDescriptor*>* envs = (xr_vector<CEnvDescriptor*>*)data;
        *item = (*envs)[idx]->m_identifier.c_str();

        return true;
    };

    auto l_EnumIni = [](void* data, int idx, const char** item) -> bool 
    {
        CInifile* ini = (CInifile*)data;
        *item = ini->sections()[idx]->Name.c_str();

        return true;
    };

    auto l_EnumIniWithEmpty = [](void* data, int idx, const char** item) -> bool
    {
        if (idx == 0)
            *item = empty;
        else
        {
            CInifile* ini = (CInifile*)data;
            *item = ini->sections()[idx - 1]->Name.c_str();
        }

        return true;
    };

    CEnvironment& env = GamePersistent().Environment();
    CEnvDescriptor* cur = env.Current[0];
    u64 time = Level().GetEnvironmentGameTime() / 1000;
    ImGui::Text("Time: %02d:%02d:%02d", int(time / (60 * 60) % 24), int(time / 60 % 60), int(time % 60));
    float tf = Level().GetEnvironmentTimeFactor();

    if (ImGui::SliderFloat("Time factor", &tf, 0.0f, 1000.0f, "%.3f", 2.0f))
        Level().SetEnvironmentTimeFactor(tf);

    xr_vector<shared_str> cycles;
    int iCycle = -1;

    for (const auto& el : env.WeatherCycles)
    {
        cycles.push_back(el.first);
        if (el.first == env.CurrentWeatherName)
            iCycle = cycles.size() - 1;
    }

    if (ImGui::Combo("Weather cycle", &iCycle, l_EnumCycle, &cycles, env.WeatherCycles.size()))
        env.SetWeather(cycles[iCycle], true);

    int sel = -1;

    for (int i = 0; i != env.CurrentWeather->size(); i++)
    {
        if (cur->m_identifier == env.CurrentWeather->at(i)->m_identifier)
            sel = i;
    }

    if (ImGui::Combo("Current section", &sel, l_EnumWeather, env.CurrentWeather, env.CurrentWeather->size()))
    {
        env.SetGameTime(env.CurrentWeather->at(sel)->exec_time + 0.5f, tf);
        time = time / (24 * 60 * 60) * 24 * 60 * 60 * 1000;
        time += u64(env.CurrentWeather->at(sel)->exec_time * 1000 + 0.5f);
        Level().SetEnvironmentGameTimeFactor(time, tf);
        env.SetWeather(cycles[iCycle], true);
    }

    ImGui::Separator();
    bool changed = false;
    sel = -1;

    for (int i = 0; i != env.m_ambients_config->sections().size(); i++)
    {
        if (cur->env_ambient->name() == env.m_ambients_config->sections()[i]->Name)
            sel = i;
    }

    if (ImGui::Combo("ambient", &sel, l_EnumIni, env.m_ambients_config, env.m_ambients_config->sections().size()))
    {
        cur->env_ambient = env.AppendEnvAmb(env.m_ambients_config->sections()[sel]->Name);
        changed = true;
    }

    if (ImGui::ColorEdit3("ambient_color", (float*)&cur->ambient))
        changed = true;

    if (ImGui::ColorEdit4("hemisphere_color", (float*)&cur->hemi_color, ImGuiColorEditFlags_AlphaBar))
        changed = true;

    if (ImGui::ColorEdit4("flora_hemi_color", (float*)&cur->flora_hemi, ImGuiColorEditFlags_AlphaBar))
        changed = true;

    if (ImGui::SliderFloat("contrast_depth", &cur->m_fColorCorrection, -5.f, 5.f))
        changed = true;
    
    if (ImGui::SliderFloat("HDR_amount", &cur->fHDR, 0.f, 1.f))
        changed = true;

    if (ImGui::SliderFloat("bloom_treshold", &cur->fBloomTreshold, 0.f, 1.f))
        changed = true;

    if (ImGui::SliderFloat("bloom_saturation", &cur->fBloomSaturation, 0.01f, 1.f))
        changed = true;

    if (ImGui::ColorEdit4("clouds_color", (float*)&cur->clouds_color, ImGuiColorEditFlags_AlphaBar))
        changed = true;

    if (ImGui::SliderFloat("clouds_velocity_0", &cur->clouds_velocity_0, 0.0f, 1.0f))
        changed = true;

    if (ImGui::SliderFloat("clouds_velocity_1", &cur->clouds_velocity_1, 0.0f, 1.0f))
        changed = true;

    if (EditTexture("clouds_texture", cur->clouds_texture_name))
    {
        cur->on_device_create();
        changed = true;
    }

    Fvector temp{};
    char buf[100]{};

    auto l_Convert = [](const Fvector& v) -> Fvector 
    {
        Fvector result;
        result.set(v.z, v.y, v.x);
        return result;
    };

    temp = l_Convert(cur->sky_color);

    if (ImGui::ColorEdit3("sky_color", (float*)&temp))
        changed = true;

    cur->sky_color = l_Convert(temp);

    if (ImGui::SliderFloat("sky_rotation", &cur->sky_rotation, 0.0f, 6.28318f))
        changed = true;

    if (EditTexture("sky_texture", cur->sky_texture_name))
    {
        strconcat(sizeof(buf), buf, cur->sky_texture_name.data(), "#small");
        cur->sky_texture_env_name = buf;
        cur->on_device_create();
        changed = true;
    }

    sel = -1;

    for (int i = 0; i != env.m_suns_config->sections().size(); i++)
    {
        if (cur->lens_flare_id == env.m_suns_config->sections()[i]->Name)
            sel = i;
    }

    if (ImGui::Combo("sun", &sel, l_EnumIni, env.m_suns_config, env.m_suns_config->sections().size()))
    {
        cur->lens_flare_id =
            env.eff_LensFlare->AppendDef(env, env.m_suns_config, env.m_suns_config->sections()[sel]->Name.c_str());
        env.eff_LensFlare->Invalidate();
        changed = true;
    }

    if (ImGui::ColorEdit3("sun_color", (float*)&cur->sun_color))
        changed = true;

    if (ImGui::SliderFloat("sun_altitude", &editor_altitude, -360.0f, 360.0f))
    {
        changed = true;
        if (changed)
            cur->sun_dir.setHP(deg2rad(editor_longitude), deg2rad(editor_altitude));
        else
            editor_altitude = cur->sun_dir.getH();
    }

    if (ImGui::SliderFloat("sun_longitude", &editor_longitude, -360.0f, 360.0f))
    {
        changed = true;
        if (changed)
            cur->sun_dir.setHP(deg2rad(editor_longitude), deg2rad(editor_altitude));
    }

    if (ImGui::SliderFloat("sun_shafts_intensity", &cur->m_fSunShaftsIntensity, 0.0f, 5.0f))
        changed = true;

    if (ImGui::SliderFloat("far_plane", &cur->far_plane, 100.f, 1400.0f))
        changed = true;

    if (ImGui::ColorEdit3("fog_color", (float*)&cur->fog_color))
        changed = true;

    if (ImGui::SliderFloat("fog_density", &cur->fog_density, 0.0f, 0.990f))
        changed = true;

    if (ImGui::SliderFloat("fog_distance", &cur->fog_distance, 25.0f, 1000.0f))
        changed = true;

    if (ImGui::SliderFloat("dof_kernel", &cur->dof_kernel, 0.0f, 10.0f))
        changed = true;

    if (ImGui::SliderFloat("dof_far_vector3", &cur->dof_far_plus, 25.0f, 750.0f))
        changed = true;

    if (ImGui::SliderFloat("fog_height", &cur->m_fFogShadersHeight, 0.f, 10.f))
        changed = true;

    if (ImGui::SliderFloat("fog_max_dist", &cur->m_fFogShadersMaxDist, 0.f, 4.f))
        changed = true;

    if (ImGui::SliderFloat("fog_ground_density", &cur->m_fFogShadersDensity, 0.f, 1.f))
        changed = true;

    if (ImGui::ColorEdit3("rain_color", (float*)&cur->rain_color))
        changed = true;

    if (ImGui::SliderFloat("rain_density", &cur->rain_density, 0.0f, 10.0f))
        changed = true;

    if (ImGui::SliderFloat("rain_angle", &cur->rain_angle, -30.0f, 30.0f))
        changed = true;

    if (ImGui::SliderFloat("rain_length", &cur->rain_length, 0.0f, 10.0f))
        changed = true;

    if (ImGui::SliderFloat("rain_width", &cur->rain_width, 0.0f, 1.0f))
        changed = true;

    if (ImGui::SliderFloat("rain_speed_max", &cur->rain_speed_max, 0.0f, 100.0f))
        changed = true;
    
    if (ImGui::SliderFloat("rain_speed_min", &cur->rain_speed_min, 0.0f, 100.0f))
        changed = true;
    
    if (ImGui::SliderFloat("rain_volumeco", &cur->rain_volume_coefficient, -5.0f, 5.0f))
        changed = true;
    
    if (ImGui::SliderFloat("rain_aanglec", &cur->rain_additional_angle_coefficient, 0.0f, 360.0f))
        changed = true;
    
    sel = 0;

    for (int i = 0; i != env.m_thunderbolt_collections_config->sections().size(); i++)
    {
        if (cur->tb_id == env.m_thunderbolt_collections_config->sections()[i]->Name)
            sel = i + 1;
    }

    if (ImGui::Combo("thunderbolt_collection", &sel, l_EnumIniWithEmpty, env.m_thunderbolt_collections_config,
            env.m_thunderbolt_collections_config->sections().size() + 1))
    {
        cur->tb_id = (sel == 0) ?
            env.eff_Thunderbolt->AppendDef(env, env.m_thunderbolt_collections_config, env.m_thunderbolts_config, "") :
            env.eff_Thunderbolt->AppendDef(env, env.m_thunderbolt_collections_config, env.m_thunderbolts_config,
                env.m_thunderbolt_collections_config->sections()[sel - 1]->Name.c_str());

        changed = true;
    }

    if (ImGui::SliderFloat("thunderbolt_duration", &cur->bolt_duration, 0.0f, 2.0f))
        changed = true;

    if (ImGui::SliderFloat("thunderbolt_period", &cur->bolt_period, 0.0f, 10.0f))
        changed = true;
    
    if (ImGui::SliderFloat("water_intensity", &cur->m_fWaterIntensity, 0.0f, 2.0f))
        changed = true;

    if (ImGui::SliderFloat("wet_surface_factor", &cur->m_fWetSurfaces, 0.0f, 2.0f))
        changed = true;
    
    if (ImGui::SliderFloat("wind_velocity", &cur->wind_velocity, 0.0f, 100.0f))
        changed = true;
    
    if (ImGui::SliderFloat("wind_direction", &cur->wind_direction, 0.0f, 360.0f))
        changed = true;
    
    if (ImGui::SliderFloat("wind_sound_volume", &cur->wind_volume, 0.0f, 7.0f))
        changed = true;
    
    if (ImGui::SliderFloat("tree_amplitude_intensity", &cur->m_fTreeAmplitudeIntensity, 0.01f, 0.100f))
        changed = true;
    
    if (ImGui::SliderFloat("volumetric_intensity_factor", &cur->volumetric_intensity_factor, 0.0f, 2.f))
        changed = true;

    if (ImGui::SliderFloat("volumetric_distance_factor", &cur->volumetric_distance_factor, 0.0f, 2.f))
        changed = true;

    if (ImGui::SliderFloat("swing_normal_amp1", &cur->m_cSwingDesc[0].amp1, 0.01f, 0.3f))
        changed = true;

    if (ImGui::SliderFloat("swing_normal_amp2", &cur->m_cSwingDesc[0].amp2, 0.005f, 0.1f))
        changed = true;

    if (ImGui::SliderFloat("swing_normal_rot1", &cur->m_cSwingDesc[0].rot1, 60.f, 300.f))
        changed = true;

    if (ImGui::SliderFloat("swing_normal_rot2", &cur->m_cSwingDesc[0].rot2, 1.f, 10.f))
        changed = true;

    if (ImGui::SliderFloat("swing_normal_speed", &cur->m_cSwingDesc[0].speed, 2.f, 2.1f))
        changed = true;

    if (ImGui::SliderFloat("swing_fast_amp1", &cur->m_cSwingDesc[1].amp1, 0.35f, 2.1f))
        changed = true;

    if (ImGui::SliderFloat("swing_fast_amp2", &cur->m_cSwingDesc[1].amp2, 0.2f, 0.4f))
        changed = true;

    if (ImGui::SliderFloat("swing_fast_rot1", &cur->m_cSwingDesc[1].rot1, 2.f, 5.f))
        changed = true;

    if (ImGui::SliderFloat("swing_fast_rot2", &cur->m_cSwingDesc[1].rot2, 0.25f, 0.5f))
        changed = true;

    if (ImGui::SliderFloat("swing_fast_speed", &cur->m_cSwingDesc[1].speed, 0.5f, 1.f))
        changed = true;

    if (changed)
        modifiedWeathers.insert(env.CurrentWeatherName);

    if (ImGui::Button("Save"))
    {
        for (auto name : modifiedWeathers)
            SaveWeather(name, env.WeatherCycles[name]);

        modifiedWeathers.clear();
    }

    ImGui::End();
}

void CImGuiCore::UIShowMenu(CUIWindow* w)
{
    if (ImGui::BeginMenu("Add"))
    {
        ImGui::MenuItem("CUIWindow");
        ImGui::MenuItem("CUIStatic");
        ImGui::MenuItem("CUIFrameWindow");
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Delete"))
    {

    }
}

#ifdef ANOMALY
void CImGuiCore::UIWndMiniMap(UIMiniMap::CUIMiniMap* p)
#else
void CImGuiCore::UIWndMiniMap(CUIMiniMap* p)
#endif
{
    bool isCollapsed = collapsed_p[p];
    ImGui::PushID(p);

    if (p->GetChildNum() != 0)
    {
        if (ImGui::Button(isCollapsed ? ">##col" : "v##col"))
            collapsed[p] = !isCollapsed;
        ImGui::SameLine();
    }

    ImGui::SameLine();
    char name[100];
    strconcat(100, name, p->WindowName().data(), ": ", GetType(p));

    if (ImGui::Selectable(name, p == curWnd_p))
        curWnd_p = p;

    if (ImGui::BeginPopupContextItem(""))
    {
        UIShowMenu(p);
        ImGui::EndPopup();
    }

    ImGui::PopID();

    if (isCollapsed)
        return;

    ImGui::Indent(20.0f);

    // for (auto&& el : p->())
    //     wndMiniMap(el);

    ImGui::Unindent(20.0f);
}

void CImGuiCore::UIWndHandler(CUIWindow* w)
{
    bool isCollapsed = collapsed[w];
    ImGui::PushID(w);
    if (w->GetChildNum() != 0)
    {
        if (ImGui::Button(isCollapsed ? ">##col" : "v##col"))
            collapsed[w] = !isCollapsed;
        ImGui::SameLine();
    }

    if (ImGui::Button(w->GetVisible() ? "o##vis" : "-##vis"))
        w->SetVisible(!w->GetVisible());

    ImGui::SameLine();
    char name[100];
    strconcat(100, name, w->WindowName().data(), ": ", GetType(w));

    if (ImGui::Selectable(name, w == curWnd))
        curWnd = w;

    if (ImGui::BeginPopupContextItem(""))
    {
        UIShowMenu(w);
        ImGui::EndPopup();
    }

    ImGui::PopID();

    if (isCollapsed)
        return;

    ImGui::Indent(20.0f);

    for (auto&& el : w->GetChildWndList())
        UIWndHandler(el);

    ImGui::Unindent(20.0f);
};

void CImGuiCore::UIShowWndList(bool& show)
{
    ImguiWnd wnd("UI Browser", &show, ImGuiWindowFlags_MenuBar);
    if (wnd.Collapsed)
        return;

    if (ImGui::BeginMenuBar())
    {
        UIShowMenu(curWnd);
        ImGui::EndMenuBar();
    }

    CUIWindow* w = CurrentGameUI()->TopInputReceiver();
    if (!w)
        w = CurrentGameUI()->UIMainIngameWnd;

    if (!w)
        return;

#ifdef ANOMALY
    UIMiniMap::CUIMiniMap* CM{};
#else
    CUIMiniMap* CM{};
#endif

    if (!CM)
        CM = CurrentGameUI()->UIMainIngameWnd->MiniMap();

    if (!CM)
        return;

    UIWndHandler(w);
    UIWndMiniMap(CM);

    ImGui::Indent(0.0f);
}

bool CImGuiCore::UIEditTexture(shared_str& texName)
{
    char tex[100];
    strncpy(tex, texName.data(), 100);
    bool changed = false;
    static shared_str prevValue;
    ImGui::Text("Texture");
    ImGui::SameLine();
    if (ImGui::InputText("##texture", tex, 100))
    {
        texName = tex;
        changed = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("...##texture"))
    {
        ImGui::OpenPopup("Choose texture");
        prevValue = texName;
    }
    if (ImGui::BeginPopupModal("Choose texture", NULL, 0))
    {
        static xr_vector<xr_string> list = CUITextureMaster::GetTextureList();
        static char filter[100] = {};
        if (ImGui::InputText("##filter", filter, 100))
        {
            auto fullList = CUITextureMaster::GetTextureList();
            const char* p = filter;
            if (p[0] != '\0')
            {
                auto e = std::copy_if(fullList.begin(), fullList.end(), list.begin(),
                    [p](const auto& x) { return strstr(x.c_str(), p) != nullptr; });
                list.resize(e - list.begin());
            }
            else
                list = fullList;
        }

        auto it = std::lower_bound(list.begin(), list.end(), texName.data());
        int cur = (it != list.end()) ? (it - list.begin()) : -1;

        if (ImGui_ListBox(
                "", &cur,
                [](void* data, int idx, const char** out_text) -> bool {
                    xr_vector<xr_string>* textures = (xr_vector<xr_string>*)data;
                    *out_text = (*textures)[idx].c_str();
                    return true;
                },
                &list, list.size(), ImVec2(-1.0f, -50.0f)))
        {
            texName = list[cur].c_str();
            changed = true;
        }

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            texName = list[cur].c_str();
            changed = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            texName = prevValue;
            changed = true;
        }
        ImGui::EndPopup();
    }
    return changed;
}

void CImGuiCore::UIShowWndProp(bool& show)
{
    ImguiWnd wnd("Window Properties", &show);
    if (wnd.Collapsed)
        return;

    if (!curWnd)
        return;

    xr_string type = GetType(curWnd);
    ImGui::Text(type.c_str());
    if (ImGui::CollapsingHeader("CUIWindow"))
    {
        Frect r = curWnd->GetWndRect();
        float width = r.width();
        float height = r.height();
        if (ImGui::InputFloat("Left", &r.left, 1.0f, 10.0f))
        {
            r.right = r.left + width;
            curWnd->SetWndRect(r);
        }
        if (ImGui::InputFloat("Top", &r.top, 1.0f, 10.0f))
        {
            r.bottom = r.top + height;
            curWnd->SetWndRect(r);
        }
        if (ImGui::InputFloat("Width", &width, 1.0f, 10.0f))
        {
            r.right = r.left + width;
            curWnd->SetWndRect(r);
        }
        if (ImGui::InputFloat("Height", &height, 1.0f, 10.0f))
        {
            r.bottom = r.top + height;
            curWnd->SetWndRect(r);
        }
        char name[100];
        auto n = curWnd->WindowName();
        strncpy(name, n.data(), 100);
        if (ImGui::InputText("Name", name, 100))
            curWnd->SetWindowName(name);
    }

    if (type == "CUIStatic" && ImGui::CollapsingHeader("CUIStatic"))
    {
        CUIStatic* s = dynamic_cast<CUIStatic*>(curWnd);

        char text[100];
        auto t = s->GetText();

        strncpy(text, t, 100);
        if (ImGui::InputText("Text", text, 100))
            s->SetText(text);

        ImColor tempColor = toImColor(s->GetTextColor());
        if (ImGui::ColorEdit4("Text color", (float*)&tempColor, ImGuiColorEditFlags_AlphaBar))
            s->SetTextColor(fromImColor(tempColor));

        //"Text offset";
        tempColor = toImColor(s->GetColor());
        if (ImGui::ColorEdit4("Color", (float*)&tempColor, ImGuiColorEditFlags_AlphaBar))
            s->SetColor(fromImColor(tempColor));

        tempColor = toImColor(s->GetTextureColor());
        if (ImGui::ColorEdit4("Texture color", (float*)&tempColor, ImGuiColorEditFlags_AlphaBar))
            s->SetTextureColor(fromImColor(tempColor));

        //"Is Texture";
        shared_str tex = s->TextureName();
        if (UIEditTexture(tex))
            s->InitTexture(tex.data());

        bool b = s->GetStretchTexture();
        if (ImGui::Checkbox("Stretch", &b))
            s->SetStretchTexture(b);

        b = s->Heading();
        if (ImGui::Checkbox("Enable heading", &b))
            s->EnableHeading(b);

        float f = s->GetHeading();
        if (ImGui::SliderFloat("Heading", &f, -3.14159f, 3.14159f))
            s->SetHeading(f);
    }
    if (type == "CUIFrameWindow" && ImGui::CollapsingHeader("CUIFrameWindow"))
    {
        CUIFrameWindow* f = dynamic_cast<CUIFrameWindow*>(curWnd);
        shared_str tex = f->TextureName();
        if (UIEditTexture(tex))
            f->InitTexture(tex.data());
    }
}

void CImGuiCore::UIShowWndHud()
{
    if (!curWnd)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2());
    bool open = true;
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("ALL_SCREEN", &open,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::PushClipRect(ImVec2(), io.DisplaySize, false);

    Frect r;
    curWnd->GetAbsoluteRect(r);
    UI().ClientToScreenScaled(r.lt, r.lt.x, r.lt.y);
    UI().ClientToScreenScaled(r.rb, r.rb.x, r.rb.y);
    ImGui::GetWindowDrawList()->AddRect((const ImVec2&)r.lt, (const ImVec2&)r.rb, 0xFF0000FF);

    ImGui::PopClipRect();
    ImGui::End();
}

void CImGuiCore::UIShowEditor(bool& show)
{
    UIShowWndList(show);
    UIShowWndProp(show);
    UIShowWndHud();
}