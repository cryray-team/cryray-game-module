#include "stdafx.h"

#include "UIMainIngameWnd.h"
#include "UIMessagesWindow.h"
#include "../UIZoneMap.h"

#include <dinput.h>
#include "../actor.h"
#include "../ActorCondition.h"
#include "../EntityCondition.h"
#include "../CustomOutfit.h"
#include "../ActorHelmet.h"
#include "../PDA.h"
#include "../xrServerEntities/character_info.h"
#include "../inventory.h"
#include "../UIGameSP.h"
#include "../weaponmagazined.h"
#include "../missile.h"
#include "../Grenade.h"
#include "../xrServerEntities/xrServer_objects_ALife.h"
#include "../alife_simulator.h"
#include "../alife_object_registry.h"
#include "../game_cl_base.h"
#include "../level.h"
#include "../seniority_hierarchy_holder.h"

#include "../date_time.h"
#include "../xrServerEntities/xrServer_Objects_ALife_Monsters.h"
#include "../xrEngine/LightAnimLibrary.h"

#include "UIInventoryUtilities.h"
#include "UIHelper.h"
#include "UIMotionIcon.h"

#include "UIXmlInit.h"
#include "UIPdaMsgListItem.h"
#include "UIPdaWnd.h"
#include "../alife_registry_wrappers.h"

#include "../string_table.h"

#ifdef DEBUG
#include "../attachable_item.h"
#include "../xrEngine/xr_input.h"
#endif

#include "UIScrollView.h"
#include "map_hint.h"
#include "../game_news.h"

#include "static_cast_checked.hpp"
#include "game_cl_capture_the_artefact.h"
#include "UIHudStatesWnd.h"
#include "UIActorMenu.h"

#include "Include/Kinematics.h"
#include "ui/UIArtefactPanel.h"
#include "ui/TypeHuds.h"

using namespace UIHud;
using namespace InventoryUtilities;

CUIMainIngameWnd::CUIMainIngameWnd() : m_pPickUpItem(nullptr), m_pMPChatWnd(nullptr), m_pMPLogWnd(nullptr)
{
    UIZoneMap = xr_new<CUIZoneMap>();
    m_artefactPanel = xr_new<UIArtefactPanel::CUIArtefactPanel>();
}

#include "UIProgressShape.h"
extern UIProgressShape::CUIProgressShape* g_MissileForceShape;

CUIMainIngameWnd::~CUIMainIngameWnd()
{
    DestroyFlashingIcons();
    xr_delete(UIZoneMap);
    xr_delete(m_artefactPanel);
    HUD_SOUND_ITEM::DestroySound(m_contactSnd);
    xr_delete(g_MissileForceShape);
    xr_delete(UIWeaponJammedIcon);
    xr_delete(UIInvincibleIcon);
}

void CUIMainIngameWnd::Init()
{
    CUIXml uiXml;

    uiXml.Load(CONFIG_PATH, UI_PATH, ArrayTypeHud[type_hud_token]);

    CUIXmlInit xml_init;
    xml_init.InitWindow(uiXml, "main", 0, this);

    Enable(false);

    UIPickUpItemIcon = UIHelper::CreateStatic(uiXml, "pick_up_item", this);
    UIPickUpItemIcon->SetShader(GetEquipmentIconsShader());

    m_iPickUpItemIconWidth = UIPickUpItemIcon->GetWidth();
    m_iPickUpItemIconHeight = UIPickUpItemIcon->GetHeight();
    m_iPickUpItemIconX = UIPickUpItemIcon->GetWndRect().left;
    m_iPickUpItemIconY = UIPickUpItemIcon->GetWndRect().top;

    // индикаторы
    UIZoneMap->Init();

    // Подсказки, которые возникают при наведении прицела на объект
    UIStaticQuickHelp = UIHelper::CreateTextWnd(uiXml, "quick_info", this);

    uiXml.SetLocalRoot(uiXml.GetRoot());

    m_UIIcons = xr_new<CUIScrollView>();
    m_UIIcons->SetAutoDelete(true);
    xml_init.InitScrollView(uiXml, "icons_scroll_view", 0, m_UIIcons);
    AttachChild(m_UIIcons);

    m_ind_thirst = UIHelper::CreateStatic(uiXml, "indicator_thirst", this, false);
    m_ind_slepping = UIHelper::CreateStatic(uiXml, "indicator_sleep", this, false);
    m_ind_starvation = UIHelper::CreateStatic(uiXml, "indicator_starvation", this, false);
    m_ind_helmet_broken = UIHelper::CreateStatic(uiXml, "indicator_helmet_broken", this, false);
    m_ind_weapon_broken = UIHelper::CreateStatic(uiXml, "indicator_weapon_broken", this, false);

    m_ind_bleeding = UIHelper::CreateStatic(uiXml, "indicator_bleeding", this, false);
    m_ind_radiation = UIHelper::CreateStatic(uiXml, "indicator_radiation", this, false);
    m_ind_outfit_broken = UIHelper::CreateStatic(uiXml, "indicator_outfit_broken", this, false);
    m_ind_overweight = UIHelper::CreateStatic(uiXml, "indicator_overweight", this, false);

    m_ind_boost_psy = UIHelper::CreateStatic(uiXml, "indicator_booster_psy", this);
    m_ind_boost_radia = UIHelper::CreateStatic(uiXml, "indicator_booster_radia", this);
    m_ind_boost_chem = UIHelper::CreateStatic(uiXml, "indicator_booster_chem", this);
    m_ind_boost_wound = UIHelper::CreateStatic(uiXml, "indicator_booster_wound", this);
    m_ind_boost_weight = UIHelper::CreateStatic(uiXml, "indicator_booster_weight", this);
    m_ind_boost_health = UIHelper::CreateStatic(uiXml, "indicator_booster_health", this);
    m_ind_boost_power = UIHelper::CreateStatic(uiXml, "indicator_booster_power", this);
    m_ind_boost_rad = UIHelper::CreateStatic(uiXml, "indicator_booster_rad", this);
    m_ind_boost_psy->Show(false);
    m_ind_boost_radia->Show(false);
    m_ind_boost_chem->Show(false);
    m_ind_boost_wound->Show(false);
    m_ind_boost_weight->Show(false);
    m_ind_boost_health->Show(false);
    m_ind_boost_power->Show(false);
    m_ind_boost_rad->Show(false);

    UIWeaponJammedIcon = UIHelper::CreateStatic(uiXml, "weapon_jammed_static", NULL);
    UIWeaponJammedIcon->Show(false);

    UIInvincibleIcon = UIHelper::CreateStatic(uiXml, "invincible_static", NULL);
    UIInvincibleIcon->Show(false);

    shared_str warningStrings[7] = {"jammed", "radiation", "wounds", "starvation", "fatigue",
        "invincible"
        "artefact"};

    // Загружаем пороговые значения для индикаторов
    EWarningIcons j = ewiWeaponJammed;
    while (j < ewiInvincible)
    {
        // Читаем данные порогов для каждого индикатора
        shared_str cfgRecord =
            pSettings->r_string("main_ingame_indicators_thresholds", *warningStrings[static_cast<int>(j) - 1]);
        u32 count = _GetItemCount(*cfgRecord);

        char singleThreshold[8];
        float f = 0;
        for (u32 k = 0; k < count; ++k)
        {
            _GetItem(*cfgRecord, k, singleThreshold);
            sscanf(singleThreshold, "%f", &f);

            m_Thresholds[j].push_back(f);
        }

        j = static_cast<EWarningIcons>(j + 1);
    }

    // Flashing icons initialize
    uiXml.SetLocalRoot(uiXml.NavigateToNode("flashing_icons"));
    InitFlashingIcons(&uiXml);

    uiXml.SetLocalRoot(uiXml.GetRoot());

    UIMotionIcon = xr_new<CUIMotionIcon>();
    UIMotionIcon->SetAutoDelete(true);
    UIZoneMap->MapFrame().AttachChild(UIMotionIcon);
    UIMotionIcon->Init(UIZoneMap->MapFrame().GetWndRect());

    m_artefactPanel->InitFromXML(uiXml, "artefact_panel", 0);
    this->AttachChild(m_artefactPanel);

    UIStaticDiskIO = UIHelper::CreateStatic(uiXml, "disk_io", this);

    m_ui_hud_states = xr_new<CUIHudStatesWnd>();
    m_ui_hud_states->SetAutoDelete(true);
    AttachChild(m_ui_hud_states);
    m_ui_hud_states->InitFromXml(uiXml, "hud_states");

    for (int i = 0; i < 4; i++)
    {
        m_quick_slots_icons.push_back(xr_new<CUIStatic>());
        m_quick_slots_icons.back()->SetAutoDelete(true);
        AttachChild(m_quick_slots_icons.back());
        string32 path;
        xr_sprintf(path, "quick_slot%d", i);
        CUIXmlInit::InitStatic(uiXml, path, 0, m_quick_slots_icons.back());
        xr_sprintf(path, "%s:counter", path);
        UIHelper::CreateStatic(uiXml, path, m_quick_slots_icons.back());
    }

    m_QuickSlotText1 = UIHelper::CreateTextWnd(uiXml, "quick_slot0_text", this);
    m_QuickSlotText2 = UIHelper::CreateTextWnd(uiXml, "quick_slot1_text", this);
    m_QuickSlotText3 = UIHelper::CreateTextWnd(uiXml, "quick_slot2_text", this);
    m_QuickSlotText4 = UIHelper::CreateTextWnd(uiXml, "quick_slot3_text", this);

    HUD_SOUND_ITEM::LoadSound("maingame_ui", "snd_new_contact", m_contactSnd, SOUND_TYPE_IDLE);
}

float UIStaticDiskIO_start_time = 0.0f;
void CUIMainIngameWnd::Draw()
{
    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());

    bool IOActive = (FS.dwOpenCounter > 0) && !(psActorFlags.test(AF_DISABLE_MINIMAP));
    if (IOActive)
        UIStaticDiskIO_start_time = Device.fTimeGlobal;

    if ((UIStaticDiskIO_start_time + 1.0f) < Device.fTimeGlobal)
        UIStaticDiskIO->Show(false);
    else
    {
        u32 alpha = clampr(iFloor(255.f * (1.f - (Device.fTimeGlobal - UIStaticDiskIO_start_time) / 1.f)), 0, 255);
        UIStaticDiskIO->Show(true);
        UIStaticDiskIO->SetTextureColor(color_rgba(255, 255, 255, alpha));
    }

    FS.dwOpenCounter = 0;

    if (!pActor || !pActor->g_Alive())
        return;

    UIMotionIcon->SetNoise((s16)(0xffff & iFloor(pActor->m_snd_noise * 100)));
    UIMotionIcon->Draw();

    UIZoneMap->visible = true;
    UIZoneMap->Render();

    bool tmp{};

    tmp = UIMotionIcon->IsShown();
    UIMotionIcon->Show(false);

    CUIWindow::Draw();

    UIMotionIcon->Show(tmp);

    RenderQuickInfos();
}

void CUIMainIngameWnd::SetMPChatLog(CUIWindow* pChat, CUIWindow* pLog)
{
    m_pMPChatWnd = pChat;
    m_pMPLogWnd = pLog;
}

void CUIMainIngameWnd::Update()
{
    CUIWindow::Update();
    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());

    if (m_pMPChatWnd)
        m_pMPChatWnd->Update();

    if (m_pMPLogWnd)
        m_pMPLogWnd->Update();

    if (!pActor)
        return;

    UIZoneMap->Update();

    UpdatePickUpItem();

    if (Device.dwFrame % 10)
        return;

    game_PlayerState* lookat_player = Game().local_player;
    if (Level().IsDemoPlayStarted())
    {
        lookat_player = Game().lookat_player();
    }
    bool b_God = (GodMode() || (!lookat_player)) ? true : lookat_player->testFlag(GAME_PLAYER_FLAG_INVINCIBLE);
    if (b_God)
    {
        SetWarningIconColor(ewiInvincible, 0xffffffff);
    }
    else
    {
        SetWarningIconColor(ewiInvincible, 0x00ffffff);
    }

    UpdateMainIndicators();
}

void CUIMainIngameWnd::RenderQuickInfos()
{
    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());
    if (!pActor)
        return;

    static CGameObject* pObject = NULL;
    LPCSTR actor_action = pActor->GetDefaultActionForObject();
    UIStaticQuickHelp->Show(NULL != actor_action);

    if (NULL != actor_action)
    {
        if (stricmp(actor_action, UIStaticQuickHelp->GetText()))
            UIStaticQuickHelp->SetTextST(actor_action);
    }

    if (pObject != pActor->ObjectWeLookingAt())
    {
        UIStaticQuickHelp->SetTextST(actor_action ? actor_action : " ");
        UIStaticQuickHelp->ResetColorAnimation();
        pObject = pActor->ObjectWeLookingAt();
    }
}

void CUIMainIngameWnd::ReceiveNews(GAME_NEWS_DATA* news)
{
    VERIFY(news->texture_name.size());

    CurrentGameUI()->m_pMessagesWnd->AddIconedPdaMessage(news);
    CurrentGameUI()->UpdatePda();
}

void CUIMainIngameWnd::SetWarningIconColorUI(CUIStatic* s, const u32 cl)
{
    int bOn = (cl >> 24);
    bool bIsShown = s->IsShown();

    if (bOn)
    {
        s->SetTextureColor(cl);
    }

    if (bOn && !bIsShown)
    {
        m_UIIcons->AddWindow(s, false);
        s->Show(true);
    }

    if (!bOn && bIsShown)
    {
        m_UIIcons->RemoveWindow(s);
        s->Show(false);
    }
}

void CUIMainIngameWnd::SetWarningIconColor(EWarningIcons icon, const u32 cl)
{
    bool bMagicFlag = true;

    // Задаем цвет требуемой иконки
    switch (icon)
    {
    case ewiAll: bMagicFlag = false;
    case ewiWeaponJammed:
        SetWarningIconColorUI(UIWeaponJammedIcon, cl);
        if (bMagicFlag)
            break;

    case ewiInvincible:
        SetWarningIconColorUI(UIInvincibleIcon, cl);
        if (bMagicFlag)
            break;
        break;
    case ewiArtefact: break;

    default: R_ASSERT(!"Unknown warning icon type");
    }
}

void CUIMainIngameWnd::TurnOffWarningIcon(EWarningIcons icon) { SetWarningIconColor(icon, 0x00ffffff); }

void CUIMainIngameWnd::SetFlashIconState_(EFlashingIcons type, bool enable)
{
    // Включаем анимацию требуемой иконки
    FlashingIcons_it icon = m_FlashingIcons.find(type);
    R_ASSERT2(icon != m_FlashingIcons.end(), "Flashing icon with this type not existed");
    icon->second->Show(enable);
}

void CUIMainIngameWnd::InitFlashingIcons(CUIXml* node)
{
    const char* const flashingIconNodeName = "flashing_icon";
    int staticsCount = node->GetNodesNum("", 0, flashingIconNodeName);

    CUIXmlInit xml_init;
    CUIStatic* pIcon = NULL;
    // Пробегаемся по всем нодам и инициализируем из них статики
    for (int i = 0; i < staticsCount; ++i)
    {
        pIcon = xr_new<CUIStatic>();
        xml_init.InitStatic(*node, flashingIconNodeName, i, pIcon);
        shared_str iconType = node->ReadAttrib(flashingIconNodeName, i, "type", "none");

        // Теперь запоминаем иконку и ее тип
        EFlashingIcons type = efiPdaTask;

        if (iconType == "pda")
            type = efiPdaTask;
        else if (iconType == "mail")
            type = efiMail;
        else
            R_ASSERT(!"Unknown type of mainingame flashing icon");

        R_ASSERT2(m_FlashingIcons.find(type) == m_FlashingIcons.end(), "Flashing icon with this type already exists");

        CUIStatic*& val = m_FlashingIcons[type];
        val = pIcon;

        AttachChild(pIcon);
        pIcon->Show(false);
    }
}

void CUIMainIngameWnd::DestroyFlashingIcons()
{
    for (FlashingIcons_it it = m_FlashingIcons.begin(); it != m_FlashingIcons.end(); ++it)
    {
        DetachChild(it->second);
        xr_delete(it->second);
    }

    m_FlashingIcons.clear();
}

void CUIMainIngameWnd::UpdateFlashingIcons()
{
    for (FlashingIcons_it it = m_FlashingIcons.begin(); it != m_FlashingIcons.end(); ++it)
    {
        it->second->Update();
    }
}

void CUIMainIngameWnd::AnimateContacts(bool b_snd)
{
    UIZoneMap->Counter_ResetClrAnimation();

    CPda* pda = Actor()->GetPDA();

    if (b_snd && !(psActorFlags.test(AF_DISABLE_MINIMAP)) && !UIZoneMap->disabled && pda)
        HUD_SOUND_ITEM::PlaySound(m_contactSnd, Fvector().set(0, 0, 0), 0, true);
}

void CUIMainIngameWnd::SetPickUpItem(CInventoryItem* PickUpItem) { m_pPickUpItem = PickUpItem; };

void CUIMainIngameWnd::UpdatePickUpItem()
{
    if (!m_pPickUpItem || !Level().CurrentViewEntity() || !smart_cast<CActor*>(Level().CurrentViewEntity()))
    {
        UIPickUpItemIcon->Show(false);
        return;
    };

    shared_str sect_name = m_pPickUpItem->object().cNameSect();

    // properties used by inventory menu
    int m_iGridWidth = pSettings->r_u32(sect_name, "inv_grid_width");
    int m_iGridHeight = pSettings->r_u32(sect_name, "inv_grid_height");

    int m_iXPos = pSettings->r_u32(sect_name, "inv_grid_x");
    int m_iYPos = pSettings->r_u32(sect_name, "inv_grid_y");

    if (pSettings->line_exist(sect_name.c_str(), "icons_texture"))
    {
        LPCSTR icons_texture = pSettings->r_string(sect_name.c_str(), "icons_texture");
        UIPickUpItemIcon->SetShader(InventoryUtilities::GetCustomIconTextureShader(icons_texture));
    }
    else
        UIPickUpItemIcon->SetShader(GetEquipmentIconsShader());

    float scale_x = m_iPickUpItemIconWidth / float(m_iGridWidth * INV_GRID_WIDTH);

    float scale_y = m_iPickUpItemIconHeight / float(m_iGridHeight * INV_GRID_HEIGHT);

    scale_x = (scale_x > 1) ? 1.0f : scale_x;
    scale_y = (scale_y > 1) ? 1.0f : scale_y;

    float scale = scale_x < scale_y ? scale_x : scale_y;

    Frect texture_rect{};
    texture_rect.lt.set(m_iXPos * INV_GRID_WIDTH, m_iYPos * INV_GRID_HEIGHT);
    texture_rect.rb.set(m_iGridWidth * INV_GRID_WIDTH, m_iGridHeight * INV_GRID_HEIGHT);
    texture_rect.rb.add(texture_rect.lt);
    UIPickUpItemIcon->GetStaticItem()->SetTextureRect(texture_rect);
    UIPickUpItemIcon->SetStretchTexture(true);

    UIPickUpItemIcon->SetWidth(m_iGridWidth * INV_GRID_WIDTH * scale * UI().get_current_kx());
    UIPickUpItemIcon->SetHeight(m_iGridHeight * INV_GRID_HEIGHT * scale);

    UIPickUpItemIcon->SetWndPos(
        Fvector2().set(m_iPickUpItemIconX + (m_iPickUpItemIconWidth - UIPickUpItemIcon->GetWidth()) / 2.0f,
            m_iPickUpItemIconY + (m_iPickUpItemIconHeight - UIPickUpItemIcon->GetHeight()) / 2.0f));

    UIPickUpItemIcon->SetTextureColor(color_rgba(255, 255, 255, 192));
    UIPickUpItemIcon->Show(true);
};

void CUIMainIngameWnd::OnConnected()
{
    UIZoneMap->SetupCurrentMap();
    if (m_ui_hud_states)
    {
        m_ui_hud_states->on_connected();
    }
}

void CUIMainIngameWnd::OnSectorChanged(int sector) { UIZoneMap->OnSectorChanged(sector); }

void CUIMainIngameWnd::reset_ui()
{
    m_pPickUpItem = nullptr;

    UIMotionIcon->ResetVisibility();

    if (m_ui_hud_states)
    {
        m_ui_hud_states->reset_ui();
    }
}

void CUIMainIngameWnd::ShowZoneMap(bool status) { UIZoneMap->visible = status; }

void CUIMainIngameWnd::DrawZoneMap() { UIZoneMap->Render(); }

void CUIMainIngameWnd::UpdateZoneMap() { UIZoneMap->Update(); }

CUIZoneMap* CUIMainIngameWnd::GetZoneMap() { return UIZoneMap; }

void CUIMainIngameWnd::UpdateMainIndicators()
{
    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());
    if (!pActor)
        return;

    UpdateQuickSlots();
    CurrentGameUI()->GetPdaMenu().UpdateRankingWnd();

    u8 flags = 0;
    flags |= LA_CYCLIC;
    flags |= LA_ONLYALPHA;
    flags |= LA_TEXTURECOLOR;

    float bleeding = pActor->conditions().BleedingSpeed();
    float radiation = pActor->conditions().GetRadiation();

    float satiety = pActor->conditions().GetSatiety();
    float satiety_critical = pActor->conditions().SatietyCritical();
    float satiety_koef =
        (satiety - satiety_critical) / (satiety >= satiety_critical ? 1 - satiety_critical : satiety_critical);

    float sleep = pActor->conditions().GetSleep();
    float sleep_critical = pActor->conditions().SleepCritical();
    float sleep_koef = (sleep - sleep_critical) / (sleep >= sleep_critical ? 1 - sleep_critical : sleep_critical);

    float thirst = pActor->conditions().GetThirst();
    float thirst_critical = pActor->conditions().ThirstCritical();
    float thirst_koef = (thirst - thirst_critical) / (thirst >= thirst_critical ? 1 - thirst_critical : thirst_critical);

    CCustomOutfit* outfit = smart_cast<CCustomOutfit*>(pActor->inventory().ItemFromSlot(OUTFIT_SLOT));
    CHelmet* helmet = smart_cast<CHelmet*>(pActor->inventory().ItemFromSlot(HELMET_SLOT));

    u16 slot = pActor->inventory().GetActiveSlot();
    float cur_weight = pActor->inventory().TotalWeight();
    float max_weight = pActor->MaxWalkWeight();
    float max_carry_weight = pActor->MaxCarryWeight();

    bool InitArrayIndex0_8 = (type_hud_token == 0 || type_hud_token == 8);
    bool InitArrayIndex9 = (type_hud_token == 9);
    bool InitArrayIndex1_6 = (type_hud_token == 1 || type_hud_token == 6); 
    bool InitArrayIndex2 = (type_hud_token == 2);
    bool InitArrayIndex3 = (type_hud_token == 3);
    bool InitArrayIndex4 = (type_hud_token == 4);

    //-' Жажда:
    if (m_ind_thirst != nullptr)
    {
        if (thirst_koef > 0.5)
            m_ind_thirst->Show(false);
        else
        {
            m_ind_thirst->Show(true);
            if (thirst_koef > 0.0f)
                m_ind_thirst->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[12] :
                        InitArrayIndex1_6                   ? ArrayIndex1_6[9] :
                        InitArrayIndex4                     ? ArrayIndex4[6] :
                        InitArrayIndex9                     ? ArrayIndex9[12] :
                                                              "null");
            else if (thirst_koef > -0.5f)
                m_ind_thirst->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[13] :
                        InitArrayIndex1_6                   ? ArrayIndex1_6[10] :
                        InitArrayIndex4                     ? ArrayIndex4[7] :
                        InitArrayIndex9                     ? ArrayIndex9[13] :
                                                              "null");
            else
                m_ind_thirst->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[14] :
                        InitArrayIndex1_6                   ? ArrayIndex1_6[11] :
                        InitArrayIndex4                     ? ArrayIndex4[8] :
                        InitArrayIndex9                     ? ArrayIndex9[14] :
                                                              "null");
        }
    }

    //-' Сонливость:
    if (m_ind_slepping != nullptr)
    {
        if (sleep_koef > 0.5)
            m_ind_slepping->Show(false);
        else
        {
            m_ind_slepping->Show(true);
            if (sleep_koef > 0.0f)
                m_ind_slepping->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[9] :
                        InitArrayIndex1_6                     ? ArrayIndex1_6[6] :
                        InitArrayIndex4                       ? ArrayIndex4[3] :
                        InitArrayIndex9                       ? ArrayIndex9[9] :
                                                                "null");
            else if (sleep_koef > -0.5f)
                m_ind_slepping->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[10] :
                        InitArrayIndex1_6                     ? ArrayIndex1_6[7] :
                        InitArrayIndex4                       ? ArrayIndex4[4] :
                        InitArrayIndex9                       ? ArrayIndex9[10] :
                                                                "null");
            else
                m_ind_slepping->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[11] :
                        InitArrayIndex1_6                     ? ArrayIndex1_6[8] :
                        InitArrayIndex4                       ? ArrayIndex4[5] :
                        InitArrayIndex9                       ? ArrayIndex9[11] :
                                                                "null");
        }
    }

    //-' Голод:
    if (m_ind_starvation != nullptr)
    {
        if (satiety_koef > 0.5)
            m_ind_starvation->Show(false);
        else
        {
            m_ind_starvation->Show(true);
            if (satiety_koef > 0.0f)
                m_ind_starvation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[6] :
                        InitArrayIndex2                         ? ArrayIndex2[6] :
                        InitArrayIndex3                         ? ArrayIndex3[3] :
                        InitArrayIndex1_6                       ? ArrayIndex1_6[3] :
                        InitArrayIndex4                         ? ArrayIndex4[0] :
                        InitArrayIndex9                         ? ArrayIndex9[6] :
                                                                  "null");

            else if (satiety_koef > -0.5f)
                m_ind_starvation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[7] :
                        InitArrayIndex2                         ? ArrayIndex2[7] :
                        InitArrayIndex3                         ? ArrayIndex3[4] :
                        InitArrayIndex1_6                       ? ArrayIndex1_6[4] :
                        InitArrayIndex4                         ? ArrayIndex4[1] :
                        InitArrayIndex9                         ? ArrayIndex9[7] :
                                                                  "null");
            else
                m_ind_starvation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[8] :
                        InitArrayIndex2                         ? ArrayIndex2[8] :
                        InitArrayIndex3                         ? ArrayIndex3[5] :
                        InitArrayIndex1_6                       ? ArrayIndex1_6[5] :
                        InitArrayIndex4                         ? ArrayIndex4[2] :
                        InitArrayIndex9                         ? ArrayIndex9[8] :
                                                                  "null");
        }
    }

    //-' Сломанный шлем:
    if (m_ind_helmet_broken != nullptr)
    {
        m_ind_helmet_broken->Show(false);
        if (helmet)
        {
            float condition = helmet->GetCondition();
            if (condition < 0.75f)
            {
                m_ind_helmet_broken->Show(true);
                if (condition > 0.5f)
                    m_ind_helmet_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[15] :
                            InitArrayIndex2                            ? ArrayIndex2[9] :
                            InitArrayIndex3                            ? ArrayIndex3[6] :
                            InitArrayIndex1_6                          ? ArrayIndex1_6[12] :
                            InitArrayIndex4                            ? ArrayIndex4[9] :
                            InitArrayIndex9                            ? ArrayIndex9[15] :
                                                                         "null");
                else if (condition > 0.25f)
                    m_ind_helmet_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[16] :
                            InitArrayIndex2                            ? ArrayIndex2[10] :
                            InitArrayIndex3                            ? ArrayIndex3[7] :
                            InitArrayIndex1_6                          ? ArrayIndex1_6[13] :
                            InitArrayIndex4                            ? ArrayIndex4[10] :
                            InitArrayIndex9                            ? ArrayIndex9[16] :
                                                                         "null");
                else
                    m_ind_helmet_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[17] :
                            InitArrayIndex2                            ? ArrayIndex2[11] :
                            InitArrayIndex3                            ? ArrayIndex3[8] :
                            InitArrayIndex1_6                          ? ArrayIndex1_6[14] :
                            InitArrayIndex4                            ? ArrayIndex4[11] :
                            InitArrayIndex9                            ? ArrayIndex9[17] :
                                                                         "null");
            }
        }
    }

    //-' Сломанный ствол:
    if (m_ind_weapon_broken != nullptr)
    {
        m_ind_weapon_broken->Show(false);
        if (slot == INV_SLOT_2 || slot == INV_SLOT_3)
        {
            CWeapon* weapon = smart_cast<CWeapon*>(pActor->inventory().ItemFromSlot(slot));
            if (weapon)
            {
                float condition = weapon->GetCondition();
                float start_misf_cond = weapon->GetMisfireStartCondition();
                float end_misf_cond = weapon->GetMisfireEndCondition();
                if (condition < start_misf_cond)
                {
                    m_ind_weapon_broken->Show(true);
                    if (condition > (start_misf_cond + end_misf_cond) / 2)
                        m_ind_weapon_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[21] :
                                InitArrayIndex2                            ? ArrayIndex2[15] :
                                InitArrayIndex3                            ? ArrayIndex3[12] :
                                InitArrayIndex1_6                          ? ArrayIndex1_6[15] :
                                InitArrayIndex4                            ? ArrayIndex4[12] :
                                InitArrayIndex9                            ? ArrayIndex9[21] :
                                                                             "null");
                    else if (condition > end_misf_cond)
                        m_ind_weapon_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[22] :
                                InitArrayIndex2                            ? ArrayIndex2[16] :
                                InitArrayIndex3                            ? ArrayIndex3[13] :
                                InitArrayIndex1_6                          ? ArrayIndex1_6[16] :
                                InitArrayIndex4                            ? ArrayIndex4[13] :
                                InitArrayIndex9                            ? ArrayIndex9[22] :
                                                                             "null");
                    else
                        m_ind_weapon_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[23] :
                                InitArrayIndex2                            ? ArrayIndex2[17] :
                                InitArrayIndex3                            ? ArrayIndex3[14] :
                                InitArrayIndex1_6                          ? ArrayIndex1_6[17] :
                                InitArrayIndex4                            ? ArrayIndex4[14] :
                                InitArrayIndex9                            ? ArrayIndex9[23] :
                                                                             "null");
                }
            }
        }
    }

    if (type_hud_token == 4)
        return;

    //-' Кровотечение:
    if (m_ind_bleeding != nullptr)
    {
        if (fis_zero(bleeding, EPS))
        {
            m_ind_bleeding->Show(false);
            m_ind_bleeding->ResetColorAnimation();
        }
        else
        {
            m_ind_bleeding->Show(true);
            if (bleeding < 0.35f)
            {
                m_ind_bleeding->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[0] :
                        InitArrayIndex2                         ? ArrayIndex2[0] :
                        InitArrayIndex9                       ? ArrayIndex9[0] :
                                                                  "null");

                m_ind_bleeding->SetColorAnimation("ui_slow_blinking_alpha", flags);
            }
            else if (bleeding < 0.7f)
            {
                m_ind_bleeding->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[1] :
                        InitArrayIndex2                         ? ArrayIndex2[1] :
                        InitArrayIndex9                       ? ArrayIndex9[1] :
                                                                  "null");

                m_ind_bleeding->SetColorAnimation("ui_medium_blinking_alpha", flags);
            }
            else
            {
                m_ind_bleeding->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[2] :
                        InitArrayIndex2                         ? ArrayIndex2[2] :
                        InitArrayIndex9                       ? ArrayIndex9[2] :
                                                                  "null");

                m_ind_bleeding->SetColorAnimation("ui_fast_blinking_alpha", flags);
            }
        }
    }

    //-' Радиация:
    if (m_ind_radiation != nullptr)
    {
        if (fis_zero(radiation, EPS))
        {
            m_ind_radiation->Show(false);
            m_ind_radiation->ResetColorAnimation();
        }
        else
        {
            m_ind_radiation->Show(true);
            if (radiation < 0.35f)
            {
                m_ind_radiation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[3] :
                        InitArrayIndex2                          ? ArrayIndex2[3] :
                        InitArrayIndex3                          ? ArrayIndex3[0] :
                        InitArrayIndex1_6                       ? ArrayIndex1_6[0] :
                        InitArrayIndex9                        ? ArrayIndex9[3] :
                                                                   "null");

                m_ind_radiation->SetColorAnimation("ui_slow_blinking_alpha", flags);
            }
            else if (radiation < 0.7f)
            {
                m_ind_radiation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[4] :
                        InitArrayIndex2                          ? ArrayIndex2[4] :
                        InitArrayIndex3                          ? ArrayIndex3[1] :
                        InitArrayIndex1_6                        ? ArrayIndex1_6[1] :
                        InitArrayIndex9                        ? ArrayIndex9[4] :
                                                                   "null");

                m_ind_radiation->SetColorAnimation("ui_medium_blinking_alpha", flags);
            }
            else
            {
                m_ind_radiation->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[5] :
                        InitArrayIndex2                          ? ArrayIndex2[5] :
                        InitArrayIndex3                          ? ArrayIndex3[2] :
                        InitArrayIndex1_6                        ? ArrayIndex1_6[2] :
                        InitArrayIndex9                        ? ArrayIndex9[5] :
                                                                   "null");

                m_ind_radiation->SetColorAnimation("ui_fast_blinking_alpha", flags);
            }
        }
    }

    //-' Сломанная броня:
    if (m_ind_outfit_broken != nullptr)
    {
        m_ind_outfit_broken->Show(false);
        if (outfit)
        {
            float condition = outfit->GetCondition();
            if (condition < 0.75f)
            {
                m_ind_outfit_broken->Show(true);
                if (condition > 0.5f)
                    m_ind_outfit_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[18] :
                            InitArrayIndex2                              ? ArrayIndex2[12] :
                            InitArrayIndex3                              ? ArrayIndex3[9] :
                            InitArrayIndex9                            ? ArrayIndex9[18] :
                                                                           "null");
                else if (condition > 0.25f)
                    m_ind_outfit_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[19] :
                            InitArrayIndex2                              ? ArrayIndex2[13] :
                            InitArrayIndex3                              ? ArrayIndex3[10] :
                            InitArrayIndex9                            ? ArrayIndex9[19] :
                                                                           "null");
                else
                    m_ind_outfit_broken->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[20] :
                            InitArrayIndex2                              ? ArrayIndex2[14] :
                            InitArrayIndex3                              ? ArrayIndex3[11] :
                            InitArrayIndex9                            ? ArrayIndex9[20] :
                                                                           "null");
            }
        }
    }

    //-' Перевес:
    if (m_ind_overweight != nullptr)
    {
        m_ind_overweight->Show(false);
        if (cur_weight >= max_carry_weight && !(psActorFlags.test(AF_GODMODE | AF_GODMODE_RT)))
        {
            m_ind_overweight->Show(true);
            if (cur_weight >= max_weight)
            {
                m_ind_overweight->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[24] :
                        InitArrayIndex2                           ? ArrayIndex2[18] :
                        InitArrayIndex3                           ? ArrayIndex3[15] :
                        InitArrayIndex1_6                       ? ArrayIndex1_6[18] :
                        InitArrayIndex9                         ? ArrayIndex9[24] :
                                                                    "null");
            }
            else
            {
                if (max_carry_weight / max_weight >= 0.5f)
                    m_ind_overweight->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[25] :
                            InitArrayIndex2                           ? ArrayIndex2[19] :
                            InitArrayIndex3                           ? ArrayIndex3[16] :
                            InitArrayIndex1_6                       ? ArrayIndex1_6[19] :
                            InitArrayIndex9                         ? ArrayIndex9[25] :
                                                                        "null");
                else
                    m_ind_overweight->InitTexture(InitArrayIndex0_8 ? ArrayIndex0_8[26] :
                            InitArrayIndex2                           ? ArrayIndex2[20] :
                            InitArrayIndex3                           ? ArrayIndex3[17] :
                            InitArrayIndex1_6                       ? ArrayIndex1_6[20] :
                            InitArrayIndex9                         ? ArrayIndex9[26] :
                                                                        "null");
            }
        }
    }
}

void CUIMainIngameWnd::UpdateQuickSlots()
{
    string32 tmp;
    LPCSTR str = CStringTable().translate("quick_use_str_1").c_str();
    strncpy_s(tmp, sizeof(tmp), str, 3);
    if (tmp[2] == ',')
        tmp[1] = '\0';
    m_QuickSlotText1->SetTextST(tmp);

    str = CStringTable().translate("quick_use_str_2").c_str();
    strncpy_s(tmp, sizeof(tmp), str, 3);
    if (tmp[2] == ',')
        tmp[1] = '\0';
    m_QuickSlotText2->SetTextST(tmp);

    str = CStringTable().translate("quick_use_str_3").c_str();
    strncpy_s(tmp, sizeof(tmp), str, 3);
    if (tmp[2] == ',')
        tmp[1] = '\0';
    m_QuickSlotText3->SetTextST(tmp);

    str = CStringTable().translate("quick_use_str_4").c_str();
    strncpy_s(tmp, sizeof(tmp), str, 3);
    if (tmp[2] == ',')
        tmp[1] = '\0';
    m_QuickSlotText4->SetTextST(tmp);

    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());
    if (!pActor)
        return;

    for (u8 i = 0; i < 4; i++)
    {
        CUIStatic* wnd = smart_cast<CUIStatic*>(m_quick_slots_icons[i]->FindChild("counter"));
        if (wnd)
        {
            shared_str item_name = g_quick_use_slots[i];
            if (item_name.size())
            {
                u32 count = pActor->inventory().dwfGetSameItemCount(item_name.c_str(), true);
                string32 str;
                xr_sprintf(str, "x%d", count);
                wnd->TextItemControl()->SetText(str);
                wnd->Show(true);

                CUIStatic* main_slot = m_quick_slots_icons[i];
                if (pSettings->line_exist(item_name.c_str(), "icons_texture"))
                {
                    LPCSTR icons_texture = pSettings->r_string(item_name.c_str(), "icons_texture");
                    main_slot->SetShader(InventoryUtilities::GetCustomIconTextureShader(icons_texture));
                }
                else
                    main_slot->SetShader(InventoryUtilities::GetEquipmentIconsShader());
                Frect texture_rect;
                texture_rect.x1 = pSettings->r_float(item_name, "inv_grid_x") * INV_GRID_WIDTH;
                texture_rect.y1 = pSettings->r_float(item_name, "inv_grid_y") * INV_GRID_HEIGHT;
                texture_rect.x2 = pSettings->r_float(item_name, "inv_grid_width") * INV_GRID_WIDTH;
                texture_rect.y2 = pSettings->r_float(item_name, "inv_grid_height") * INV_GRID_HEIGHT;
                texture_rect.rb.add(texture_rect.lt);
                main_slot->SetTextureRect(texture_rect);
                main_slot->TextureOn();
                main_slot->SetStretchTexture(true);
                if (!count)
                {
                    wnd->SetTextureColor(color_rgba(255, 255, 255, 0));
                    wnd->TextItemControl()->SetTextColor(color_rgba(255, 255, 255, 0));
                    m_quick_slots_icons[i]->SetTextureColor(color_rgba(255, 255, 255, 100));
                }
                else
                {
                    wnd->SetTextureColor(color_rgba(255, 255, 255, 255));
                    wnd->TextItemControl()->SetTextColor(color_rgba(255, 255, 255, 255));
                    m_quick_slots_icons[i]->SetTextureColor(color_rgba(255, 255, 255, 255));
                }
            }
            else
            {
                wnd->Show(false);
                m_quick_slots_icons[i]->SetTextureColor(color_rgba(255, 255, 255, 0));
                //				m_quick_slots_icons[i]->Show(false);
            }
        }
    }
}

void CUIMainIngameWnd::DrawMainIndicatorsForInventory()
{
    CActor* pActor = smart_cast<CActor*>(Level().CurrentViewEntity());
    if (!pActor)
        return;

    UpdateQuickSlots();
    UpdateBoosterIndicators(pActor->conditions().GetCurBoosterInfluences());

    for (int i = 0; i < 4; i++)
        m_quick_slots_icons[i]->Draw();

    m_QuickSlotText1->Draw();
    m_QuickSlotText2->Draw();
    m_QuickSlotText3->Draw();
    m_QuickSlotText4->Draw();

    if (m_ind_boost_psy->IsShown())
    {
        m_ind_boost_psy->Update();
        m_ind_boost_psy->Draw();
    }

    if (m_ind_boost_radia->IsShown())
    {
        m_ind_boost_radia->Update();
        m_ind_boost_radia->Draw();
    }

    if (m_ind_boost_chem->IsShown())
    {
        m_ind_boost_chem->Update();
        m_ind_boost_chem->Draw();
    }

    if (m_ind_boost_wound->IsShown())
    {
        m_ind_boost_wound->Update();
        m_ind_boost_wound->Draw();
    }

    if (m_ind_boost_weight->IsShown())
    {
        m_ind_boost_weight->Update();
        m_ind_boost_weight->Draw();
    }

    if (m_ind_boost_health->IsShown())
    {
        m_ind_boost_health->Update();
        m_ind_boost_health->Draw();
    }

    if (m_ind_boost_power->IsShown())
    {
        m_ind_boost_power->Update();
        m_ind_boost_power->Draw();
    }

    if (m_ind_boost_rad->IsShown())
    {
        m_ind_boost_rad->Update();
        m_ind_boost_rad->Draw();
    }

    m_ui_hud_states->DrawZoneIndicators();
}

void CUIMainIngameWnd::UpdateBoosterIndicators(const xr_map<EBoostParams, SBooster> influences)
{
    m_ind_boost_psy->Show(false);
    m_ind_boost_radia->Show(false);
    m_ind_boost_chem->Show(false);
    m_ind_boost_wound->Show(false);
    m_ind_boost_weight->Show(false);
    m_ind_boost_health->Show(false);
    m_ind_boost_power->Show(false);
    m_ind_boost_rad->Show(false);

    LPCSTR str_flag = "ui_slow_blinking_alpha";
    u8 flags = 0;
    flags |= LA_CYCLIC;
    flags |= LA_ONLYALPHA;
    flags |= LA_TEXTURECOLOR;

    xr_map<EBoostParams, SBooster>::const_iterator b = influences.begin(), e = influences.end();
    for (; b != e; b++)
    {
        switch (b->second.m_type)
        {
        case eBoostHpRestore: {
            m_ind_boost_health->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_health->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_health->ResetColorAnimation();
        }
        break;
        case eBoostPowerRestore: {
            m_ind_boost_power->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_power->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_power->ResetColorAnimation();
        }
        break;
        case eBoostRadiationRestore: {
            m_ind_boost_rad->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_rad->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_rad->ResetColorAnimation();
        }
        break;
        case eBoostBleedingRestore: {
            m_ind_boost_wound->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_wound->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_wound->ResetColorAnimation();
        }
        break;
        case eBoostMaxWeight: {
            m_ind_boost_weight->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_weight->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_weight->ResetColorAnimation();
        }
        break;
        case eBoostRadiationImmunity:
        case eBoostRadiationProtection: {
            m_ind_boost_radia->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_radia->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_radia->ResetColorAnimation();
        }
        break;
        case eBoostTelepaticImmunity:
        case eBoostTelepaticProtection: {
            m_ind_boost_psy->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_psy->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_psy->ResetColorAnimation();
        }
        break;
        case eBoostChemicalBurnImmunity:
        case eBoostChemicalBurnProtection: {
            m_ind_boost_chem->Show(true);
            if (b->second.fBoostTime <= 3.0f)
                m_ind_boost_chem->SetColorAnimation(str_flag, flags);
            else
                m_ind_boost_chem->ResetColorAnimation();
        }
        break;
        }
    }
}

UIMiniMap::CUIMiniMap* CUIMainIngameWnd::MiniMap() { return UIZoneMap->MiniMap(); }