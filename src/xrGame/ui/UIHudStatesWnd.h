#pragma once

#include "UIWindow.h"
#include "../xrGameAPI\alife_space.h"
#include "..\..\xrServerEntities\inventory_space.h"
#include "../xrGameAPI/Actor/ActorDefs.h"
#include "UIProgressShape.h"

class CUIStatic;
class CUITextWnd;
class CUIProgressBar;
class UIProgressShape::CUIProgressShape;
class CUIXml;
class UI_Arrow;
class CActor;
class CHelmet;
class CCustomOutfit;

int const it_max = ALife::infl_max_count - 1;

class CUIHudStatesWnd : public CUIWindow
{
private:
    typedef CUIWindow inherited;

public:
    CUIStatic* m_back{};
    CUIStatic* m_indik[it_max]{};

    CUITextWnd* m_ui_weapon_cur_ammo{};
    CUITextWnd* m_ui_weapon_fmj_ammo{};
    CUITextWnd* m_ui_weapon_ap_ammo{};
    CUITextWnd* m_ui_weapon_third_ammo{}; // Alundaio
    CUITextWnd* m_fire_mode{};
    CUITextWnd* m_ui_grenade{};
    II_BriefInfo m_item_info{};

    CUIStatic* m_ui_weapon_icon{};
    Frect m_ui_weapon_icon_rect{};

    CUIProgressBar* m_ui_health_bar{};
    CUIProgressBar* m_ui_stamina_bar{};

    CUIStatic* m_radia_damage{};

    CUIStatic* m_back_v{};
    UI_Arrow* m_arrow{};
    UI_Arrow* m_arrow_shadow{};
    CUIStatic* m_static_stamina{};
    CUIStatic* m_mini_armor_icon{};
    UIProgressShape::CUIProgressShape* m_progress_self;

    CUIStatic* m_bleeding_soc_lvl_[3]{};
    CUIStatic* m_starvation_soc_lvl_[3]{};
    CUIStatic* m_broken_helmet_soc_lvl_[3]{};
    CUIStatic* m_broken_weapon_soc_lvl_[3]{};
    CUIStatic* m_thirst_soc_lvl_[3]{};
    CUIStatic* m_sleep_soc_lvl_[3]{};
    CUIStatic* m_radiation_soc_lvl_[3]{};

    CUIStatic* m_bleeding_lvls_[4]{};
    CUIStatic* m_starvation_lvls_[4]{};
    CUIStatic* m_overweight_lvl_[4]{};
    CUIStatic* m_broken_helmet_lvl_[4]{};
    CUIStatic* m_broken_weapon_lvl_[4]{};
    CUIStatic* m_broken_armor_lvl_[4]{};
    CUIStatic* m_thirst_lvl_[4]{};
    CUIStatic* m_sleep_lvl_[4]{};

    CUIStatic* m_static_health{};
    CUIStatic* m_back_over_arrow{};
    CUIStatic* m_static_armor{};
    CUIStatic* m_resist_back[it_max];
    CUIStatic* m_bleeding{};
    CUIProgressBar* m_ui_armor_bar{};

    CUIStatic* m_psy_grenn{};
    CUIStatic* m_psy_yellow{};
    CUIStatic* m_psy_red{};

    CUITextWnd* m_weapon_ammo{};
    CUIProgressBar* m_ui_psy_health_bar{};
    CUIStatic* m_ui_weapon_sign_ammo{};

    u32 m_ui_weapon_ammo_color_active{};
    u32 m_ui_weapon_ammo_color_inactive{};

    float m_last_health{};
    float m_health_blink{};

    float m_radia_self{};
    float m_radia_hit{};
    shared_str m_lanim_name{};

    float m_zone_cur_power[ALife::infl_max_count]{};
    float m_zone_feel_radius[ALife::infl_max_count]{};
    ALife::EHitType m_zone_hit_type[ALife::infl_max_count]{};
    float m_zone_threshold[ALife::infl_max_count]{};

    float m_zone_feel_radius_max{};
    u32 m_timer_1sec{};

    bool m_fake_indicators_update{};
    bool m_b_force_update{};
    bool m_cur_state_LA[it_max]{};

    float m_psy_health_blink{};
    float m_last_psyhealth{};

    // Tronex
    bool m_ui_health_bar_show;
    bool m_ui_stamina_bar_show;
    bool m_ui_psy_health_show;

public:
    CUIHudStatesWnd();
    virtual ~CUIHudStatesWnd();

    void InitFromXml(CUIXml& xml, LPCSTR path);
    void InitFromXml—S(CUIXml& xml, LPCSTR path);

    void Load_section();
    virtual void Update();

    void on_connected();
    void reset_ui();
    void UpdateHealth(CActor* actor);
    void SetAmmoIcon(const shared_str& sect_name);
    void UpdateActiveItemInfo(CActor* actor);
    void InitTextures(CUIXml& xml);

    void UpdateZones();
    void UpdateIndicators(CActor* actor);

    void UpdateIndicators5(CActor* actor, float satiety, float satiety_critical, float satiety_koef,
        float bleeding_float, float cur_weight, float max_weight, float max_carry_weight, u16 slot, CHelmet* helmet,
        CCustomOutfit* outfit, float thirst, float thirst_critical, float thirst_koef, float sleep,
        float sleep_critical, float sleep_koef, float radiation);

    void UpdateIndicators7(CActor* actor, float satiety, float satiety_critical, float satiety_koef,
        float bleeding_float, float cur_weight, float max_weight, float max_carry_weight, u16 slot, CHelmet* helmet,
        CCustomOutfit* outfit, float thirst, float thirst_critical, float thirst_koef, float sleep,
        float sleep_critical, float sleep_koef, float radiation);

    float get_zone_cur_power(ALife::EHitType hit_type);
    float get_main_sensor_value() { return m_radia_hit; }

    void DrawZoneIndicators();
    void FakeUpdateIndicatorType(u8 t, float power);
    void EnableFakeIndicators(bool enable);

protected:
    static ALife::EInfluenceType get_indik_type(ALife::EHitType hit_type);

    void Load_section_type(ALife::EInfluenceType type, LPCSTR section);
    void UpdateIndicatorType(CActor* actor, ALife::EInfluenceType type);
    void SwitchLA(bool state, ALife::EInfluenceType type);

}; // class CUIHudStatesWnd