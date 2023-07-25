#pragma once

#include "ai_space.h"
#include "GamePersistent.h"
#include "Level.h"

#include "script_engine.h"
#include "HUDmanager.h"
#include "Level.h"
#include "UIGameCustom.h"
#include "UIGameSP.h"
#include "ui/UIFrameWindow.h"
#include "ui/UIMainIngameWnd.h"
#include "ui/UITextureMaster.h"
#include "ui/UILines.h"

#ifdef ANOMALY
#include "../xrGameAPI/UI/UICustomMap.h"
#include "../xrGameAPI/UI/UIGlobalMap.h"
#include "../xrGameAPI/UI/UILevelMap.h"
#include "../xrGameAPI/UI/UIMiniMap.h"

using namespace UICustomMap;
using namespace UIGlobalMap;
using namespace UILevelMap;
using namespace UIMiniMap;
#else
#include "../xrGameAPI_IWP/UIMiniMap/UICustomMap.h"
#include "../xrGameAPI_IWP/UIMiniMap/UIGlobalMap.h"
#include "../xrGameAPI_IWP/UIMiniMap/UILevelMap.h"
#include "../xrGameAPI_IWP/UIMiniMap/UIMiniMap.h"
#endif

#include "../xrEngine/xr_input.h"
#include "../xrEngine/Environment.h"
#include "../xrEngine/IGame_Level.h"
#include "../xrEngine/thunderbolt.h"
#include "../xrEngine/xr_efflensflare.h"

#include "../Externals/imgui_api/imgui.h"
#include "../Externals/imgui_api/addons/imguinodegrapheditor/imguinodegrapheditor.h"

#include "../xrCryRayAPI/xrCryRayAPI.h"

#include <Shlwapi.h>

class CImGuiCore
{
    struct ImguiWnd
    {
        ImguiWnd(const char* name, bool* pOpen = nullptr, ImGuiWindowFlags flags = 0)
        {
            Collapsed = !ImGui::Begin(name, pOpen, flags);
        }
        ~ImguiWnd() { ImGui::End(); }

        bool Collapsed;
    };

    bool ImGuiHelpWnd;
    bool ImGuiHelpWndDis;
    bool ImGuiWeatherEditor;
    bool ImGuiUIEditorHelper;

    bool ImGui_ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data,
        int items_count, const ImVec2& size_arg = ImVec2(0, 0))
    {
        if (!ImGui::ListBoxHeader(label, size_arg))
            return false;

        // Assume all items have even height (= 1 line of text). If you need items of different or variable sizes
        // you can create a custom version of ListBox() in your code without using the clipper.
        bool value_changed = false;
        ImGuiListClipper clipper(items_count, ImGui::GetTextLineHeightWithSpacing()); // We know exactly our line height
                                                                                      // here so we pass it as a minor
                                                                                      // optimization, but generally you
                                                                                      // don't need to.
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                const bool item_selected = (i == *current_item);
                const char* item_text;
                if (!items_getter(data, i, &item_text))
                    item_text = "*Unknown item*";

                ImGui::PushID(i);
                if (ImGui::Selectable(item_text, item_selected))
                {
                    *current_item = i;
                    value_changed = true;
                }
                ImGui::PopID();
            }
        }
        ImGui::ListBoxFooter();
        return value_changed;
    }

public:
    enum EditorStage
    {
        None,
        Light,
        Full,
        Count,
    };

    CImGuiCore();
    ~CImGuiCore();

    bool IsEditor();
    bool IsEditorActive();

    void ShowMain();
    void ShowEditor();

    bool EditorKeyPressCallback(int key);

    void SaveWeather(shared_str name, const xr_vector<CEnvDescriptor*>& env);
    void NextTexture(char* tex, int texSize, int offset);

    bool EditTexture(const char* label, shared_str& texName);
    void ShowWeatherEditor(bool& show);

    IC LPCSTR GetType(CUIWindow* w) { return typeid(*w).name() + 6; }

    void UIShowMenu(CUIWindow* w);

#ifdef ANOMALY
    void UIWndMiniMap(UIMiniMap::CUIMiniMap* p);
#else
    void UIWndMiniMap(CUIMiniMap* p);
#endif

    void UIWndHandler(CUIWindow* w);
    void UIShowWndList(bool& show);

    IC ImColor toImColor(u32 bgra) { return bgra; }

    IC u32 fromImColor(const ImColor& c)
    {
        u32 abgr = (ImU32)c;

        return abgr;
    };

    bool UIEditTexture(shared_str& texName);
    void UIShowWndProp(bool& show);
    void UIShowWndHud();
    void UIShowEditor(bool& show);
};

extern CImGuiCore* ImGuiCore;