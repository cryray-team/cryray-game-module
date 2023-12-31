﻿#pragma once

#include "PhysicsShell.h"
#include "weaponammo.h"
#include "PHShellCreator.h"

#include "ShootingObject.h"
#include "hud_item_object.h"
#include "Include/KinematicsAnimated.h"
#include "firedeps.h"
#include "game_cl_single.h"
#include "first_bullet_controller.h"

#include "CameraRecoil.h"

class CEntity;
class ENGINE_API CMotionDef;
class CSE_ALifeItemWeapon;
class CSE_ALifeItemWeaponAmmo;
class CWeaponMagazined;
class CParticlesObject;
class CUIWindow;
class CBinocularsVision;
class CNightVisionEffector;

extern float f_weapon_deterioration;

struct PickParam
{
    collide::rq_result RQ;
    float power;
    u32 pass;
};

struct SafemodeAnm
{
    LPCSTR name;
    float power, speed;
};

class CWeapon : public CHudItemObject, public CShootingObject
{
private:
    typedef CHudItemObject inherited;

public:
    CWeapon();
    virtual ~CWeapon();

    // Generic
    virtual void Load(LPCSTR section);

    virtual BOOL net_Spawn(CSE_Abstract* DC);
    virtual void net_Destroy();
    virtual void net_Export(NET_Packet& P);
    virtual void net_Import(NET_Packet& P);

    virtual CWeapon* cast_weapon() { return this; }

    virtual CWeaponMagazined* cast_weapon_magazined() { return 0; }

    // serialization
    virtual void save(NET_Packet& output_packet);
    virtual void load(IReader& input_packet);

    virtual BOOL net_SaveRelevant() { return inherited::net_SaveRelevant(); }

    float CWeapon::GetSecondVPFov() const;
    IC float GetZRotatingFactor() const { return m_zoom_params.m_fZoomRotationFactor; }
    IC float GetSecondVPZoomFactor() const { return m_zoom_params.m_fSecondVPFovFactor; }
    IC float IsSecondVPZoomPresent() const { return GetSecondVPZoomFactor() > 0.005f; }

    void UpdateSecondVP();

    virtual void UpdateCL();
    virtual void shedule_Update(u32 dt);

    virtual void renderable_Render();
    virtual void render_hud_mode();
    virtual bool need_renderable();

    virtual void render_item_ui();
    virtual bool render_item_ui_query();

    virtual void OnH_B_Chield();
    virtual void OnH_A_Chield();
    virtual void OnH_B_Independent(bool just_before_destroy);
    virtual void OnH_A_Independent();
    virtual void OnEvent(NET_Packet& P, u16 type); // {inherited::OnEvent(P,type);}

    virtual void Hit(SHit* pHDS);

    virtual void reinit();
    virtual void reload(LPCSTR section);
    virtual void create_physic_shell();
    virtual void activate_physic_shell();
    virtual void setup_physic_shell();

    virtual void SwitchState(u32 S);

    virtual void OnActiveItem();
    virtual void OnHiddenItem();
    virtual void SendHiddenItem(); // same as OnHiddenItem but for client... (sends message to a server)...

    virtual bool NeedBlendAnm();

public:
    virtual bool can_kill() const;
    virtual CInventoryItem* can_kill(CInventory* inventory) const;
    virtual const CInventoryItem* can_kill(const xr_vector<const CGameObject*>& items) const;
    virtual bool ready_to_kill() const;
    virtual bool NeedToDestroyObject() const;
    virtual ALife::_TIME_ID TimePassedAfterIndependant() const;

protected:
    // âðåìÿ óäàëåíèÿ îðóæèÿ
    ALife::_TIME_ID m_dwWeaponRemoveTime;
    ALife::_TIME_ID m_dwWeaponIndependencyTime;

    virtual bool IsHudModeNow();
    virtual bool SOParentIsActor() { return ParentIsActor(); }
    u8 last_idx;

public:
    void signal_HideComplete();
    virtual bool Action(u16 cmd, u32 flags);

    enum EWeaponStates
    {
        eFire = eLastBaseState + 1,
        eFire2,
        eReload,
        eMisfire,
        eMagEmpty,
        eSwitch,
        eSwitchMode,
    };

    enum EWeaponSubStates
    {
        eSubstateReloadBegin = 0,
        eSubstateReloadInProcess,
        eSubstateReloadEnd,
    };

    enum
    {
        undefined_ammo_type = u8(-1)
    };

    IC BOOL IsValid() const { return m_ammoElapsed.type1; }

    // Does weapon need's update?
    BOOL IsUpdating();

    BOOL IsMisfire() const;
    BOOL CheckForMisfire();

    BOOL AutoSpawnAmmo() const { return m_bAutoSpawnAmmo; };

    bool IsTriStateReload() const { return m_bTriStateReload; }

    EWeaponSubStates GetReloadState() const { return (EWeaponSubStates)m_sub_state; }

protected:
    bool m_bTriStateReload;

    // a misfire happens, you'll need to rearm weapon
    bool bMisfire;
    bool bClearJamOnly; // used for "reload" misfire animation

    BOOL m_bAutoSpawnAmmo;
    virtual bool AllowBore();

public:
    u8 m_sub_state;

    bool IsGrenadeLauncherAttached() const;
    bool IsScopeAttached() const;
    bool IsSilencerAttached() const;

    virtual bool GrenadeLauncherAttachable();
    virtual bool ScopeAttachable();
    virtual bool SilencerAttachable();

    ALife::EWeaponAddonStatus get_GrenadeLauncherStatus() const { return m_eGrenadeLauncherStatus; }

    ALife::EWeaponAddonStatus get_ScopeStatus() const { return m_eScopeStatus; }

    ALife::EWeaponAddonStatus get_SilencerStatus() const { return m_eSilencerStatus; }

    virtual bool UseScopeTexture() { return true; };

    // îáíîâëåíèå âèäèìîñòè äëÿ êîñòî÷åê àääîíîâ
    void UpdateAddonsVisibility();
    void UpdateHUDAddonsVisibility();
    // èíèöèàëèçàöèÿ ñâîéñòâ ïðèñîåäèíåííûõ àääîíîâ
    virtual void InitAddons();

    // äëÿ îòîáðîàæåíèÿ èêîíîê àïãðåéäîâ â èíòåðôåéñå
    int GetScopeX() { return pSettings->r_s32(m_scopes[m_cur_addon.scope], "scope_x"); }

    int GetScopeY() { return pSettings->r_s32(m_scopes[m_cur_addon.scope], "scope_y"); }

    int GetSilencerX() { return pSettings->r_s32(m_silencers[m_cur_addon.silencer], "silencer_x"); }

    int GetSilencerY() { return pSettings->r_s32(m_silencers[m_cur_addon.silencer], "silencer_y"); }

    int GetGrenadeLauncherX() { return pSettings->r_s32(m_launchers[m_cur_addon.launcher], "grenade_launcher_x"); }

    int GetGrenadeLauncherY() { return pSettings->r_s32(m_launchers[m_cur_addon.launcher], "grenade_launcher_y"); }

    const shared_str GetGrenadeLauncherName() const
    {
        if (m_launchers.size() < 1)
        {
            return {};
        }

        return pSettings->r_string(m_launchers[m_cur_addon.launcher], "grenade_launcher_name");
    }

    const shared_str GetScopeName() const
    {
        if (m_scopes.size() < 1)
        {
            return {};
        }

        return pSettings->r_string(m_scopes[m_cur_addon.scope], "scope_name");
    }

    const shared_str GetSilencerName() const
    {
        if (m_silencers.size() < 1)
        {
            return {};
        }

        return pSettings->r_string(m_silencers[m_cur_addon.silencer], "silencer_name");
    }

    const shared_str GetGrenadeLauncherBoneName() const
    {
        return READ_IF_EXISTS(pSettings, r_string, GetGrenadeLauncherName(), "addon_bone", "wpn_launcher");
    }
    const shared_str GetScopeBoneName() const
    {
        return READ_IF_EXISTS(pSettings, r_string, GetScopeName(), "addon_bone", "wpn_scope");
    }
    const shared_str GetSilencerBoneName() const
    {
        return READ_IF_EXISTS(pSettings, r_string, GetSilencerName(), "addon_bone", "wpn_silencer");
    }

    bool SetBoneVisible(IKinematics* m_model, const shared_str& bone_name, BOOL bVisibility, BOOL bSilent);

    IC void ForceUpdateAmmo() { m_BriefInfo_CalcFrame = 0; }

    u8 GetAddonsState() const { return m_flagsAddOnState; };

    void SetAddonsState(u8 st) { m_flagsAddOnState = st; } // dont use!!! for buy menu only!!!

protected:
    // ñîñòîÿíèå ïîäêëþ÷åííûõ àääîíîâ
    u8 m_flagsAddOnState;

    // âîçìîæíîñòü ïîäêëþ÷åíèÿ ðàçëè÷íûõ àääîíîâ
    ALife::EWeaponAddonStatus m_eScopeStatus;
    ALife::EWeaponAddonStatus m_eSilencerStatus;
    ALife::EWeaponAddonStatus m_eGrenadeLauncherStatus;

    struct current_addon_t
    {
        union
        {
            u16 data;
            struct
            {
                u16 scope : 6; // 2^6 possible scope sections
                u16 silencer : 5; // 2^5 possible silencer/launcher sections
                u16 launcher : 5;
            };
        };
    };

public:
    current_addon_t m_cur_addon;
    virtual void SyncronizeWeaponToServer();

protected:
    struct SZoomParams
    {
        bool m_bZoomEnabled;
        bool m_bHideCrosshairInZoom;
        bool m_bZoomDofEnabled;
        bool m_bIsZoomModeNow;
        float m_fCurrentZoomFactor;
        float m_fZoomRotateTime;
        float m_fBaseZoomFactor;
        float m_fScopeZoomFactor;
        float m_fScopeZoomFactorMin;
        float m_fZoomRotationFactor;
        float m_fSecondVPFovFactor;
        Fvector m_ZoomDof;
        Fvector4 m_ReloadDof;
        Fvector4 m_ReloadEmptyDof; // Swartz: reload when empty mag. DOF
        BOOL m_bUseDynamicZoom;
        shared_str m_sUseZoomPostprocess;
        shared_str m_sUseBinocularVision;
        CBinocularsVision* m_pVision;
        CNightVisionEffector* m_pNight_vision;
        bool m_bNightVisionAllow;
    } m_zoom_params;

    float m_fRTZoomFactor; // run-time zoom factor
    CUIWindow* m_UIScope;

public:
    void AllowNightVision(bool value) { m_zoom_params.m_bNightVisionAllow = value; };
    bool AllowNightVision() { return m_zoom_params.m_bNightVisionAllow; };
    IC bool IsZoomEnabled() const { return m_zoom_params.m_bZoomEnabled; }

    ref_sound sndZoom;
    void zoom_sound_inс();
    void zoom_sound_dec();

    virtual void ZoomInc();
    virtual void ZoomDec();
    virtual void OnZoomIn();
    virtual void OnZoomOut();
    IC bool IsZoomed() const { return m_zoom_params.m_bIsZoomModeNow; };
    CUIWindow* ZoomTexture();

    bool ZoomHideCrosshair();

    IC float GetZoomFactor() const { return m_zoom_params.m_fCurrentZoomFactor; }

    IC void SetZoomFactor(float f) { m_zoom_params.m_fCurrentZoomFactor = f; }

    virtual float CurrentZoomFactor();
    // ïîêàçûâàåò, ÷òî îðóæèå íàõîäèòñÿ â ñîîñòîÿíèè ïîâîðîòà äëÿ ïðèáëèæåííîãî ïðèöåëèâàíèÿ
    bool IsRotatingToZoom() const { return (m_zoom_params.m_fZoomRotationFactor < 1.f); }

    virtual u8 GetCurrentHudOffsetIdx();

    // Tronex script exports
    void AmmoTypeForEach(const luabind::functor<bool>& funct);
    float GetMagazineWeightScript() const { return GetMagazineWeight(m_magazine); }
    int GetAmmoCount_forType_Script(LPCSTR type) const { return GetAmmoCount_forType(type); }
    LPCSTR GetGrenadeLauncherNameScript() const { return *GetGrenadeLauncherName(); }
    LPCSTR GetSilencerNameScript() const { return *GetSilencerName(); }
    LPCSTR GetScopeNameScript() const { return *GetScopeName(); }
    float GetFireDispersionScript() const { return fireDispersionBase; }
    float RPMScript() const { return fOneShotTime; }
    float ModeRPMScript() const { return fModeShotTime; }

    virtual float Weight() const;
    virtual u32 Cost() const;

public:
    virtual EHandDependence HandDependence() const { return eHandDependence; }

    bool IsSingleHanded() const { return m_bIsSingleHanded; }

public:
    IC LPCSTR strap_bone0() const { return m_strap_bone0; }

    IC LPCSTR strap_bone1() const { return m_strap_bone1; }

    IC void strapped_mode(bool value) { m_strapped_mode = value; }

    IC bool strapped_mode() const { return m_strapped_mode; }

protected:
    LPCSTR m_strap_bone0;
    LPCSTR m_strap_bone1;
    Fmatrix m_StrapOffset;
    bool m_strapped_mode;
    bool m_can_be_strapped;
    float m_fSafeModeRotateTime;
    SafemodeAnm m_safemode_anm[2];

    Fmatrix m_Offset;
    Fvector m_hud_offset[2];
    // 0-èñïîëüçóåòñÿ áåç ó÷àñòèÿ ðóê, 1-îäíà ðóêà, 2-äâå ðóêè
    EHandDependence eHandDependence;
    bool m_bIsSingleHanded;

public:
    // çàãðóæàåìûå ïàðàìåòðû
    Fvector vLoadedFirePoint;
    Fvector vLoadedFirePoint2;
    bool m_bCanBeLowered;

private:
    firedeps m_current_firedeps;
    // collide::rq_results RQS;
    // PickParam PP;

public:
    // virtual collide::rq_result& GetRQ() { return PP.RQ; }
    // virtual void net_Relcase(CObject* object);
    Fmatrix m_shoot_shake_mat;

protected:
    virtual void UpdateFireDependencies_internal();
    void UpdateUIScope();
    void SwitchZoomType();
    float GetHudFov();
    virtual void UpdatePosition(const Fmatrix& transform); //.
    virtual void UpdateXForm();
    virtual void UpdateHudAdditional(Fmatrix& trans);
    IC void UpdateFireDependencies()
    {
        if (dwFP_Frame == Device.dwFrame)
            return;
        UpdateFireDependencies_internal();
    };

    virtual void LoadFireParams(LPCSTR section);

public:
    IC const Fvector& get_LastFP()
    {
        UpdateFireDependencies();
        return m_current_firedeps.vLastFP;
    }

    IC const Fvector& get_LastFP2()
    {
        UpdateFireDependencies();
        return m_current_firedeps.vLastFP2;
    }

    IC const Fvector& get_LastFD()
    {
        UpdateFireDependencies();
        return m_current_firedeps.vLastFD;
    }

    IC const Fvector& get_LastSP()
    {
        UpdateFireDependencies();
        return m_current_firedeps.vLastSP;
    }

    virtual const Fvector& get_CurrentFirePoint() { return get_LastFP(); }

    virtual const Fvector& get_CurrentFirePoint2() { return get_LastFP2(); }

    virtual const Fmatrix& get_ParticlesXFORM()
    {
        UpdateFireDependencies();
        return m_current_firedeps.m_FireParticlesXForm;
    }

    virtual void ForceUpdateFireParticles();
    virtual void debug_draw_firedeps();

protected:
    virtual void SetDefaults();

    virtual bool MovingAnimAllowedNow();
    virtual void OnStateSwitch(u32 S, u32 oldState);
    virtual void OnAnimationEnd(u32 state);

    // òðàññèðîâàíèå ïîëåòà ïóëè
    virtual void FireTrace(const Fvector& P, const Fvector& D);
    virtual float GetWeaponDeterioration();

    virtual void FireStart();

    virtual void FireEnd();

    virtual void Reload();
    void StopShooting();

    // îáðàáîòêà âèçóàëèçàöèè âûñòðåëà
    virtual void OnShot(){};
    virtual void AddShotEffector();
    virtual void RemoveShotEffector();
    virtual void ClearShotEffector();
    virtual void StopShotEffector();

public:
    float GetBaseDispersion(float cartridge_k);
    float GetFireDispersion(bool with_cartridge, bool for_crosshair = false);
    virtual float GetFireDispersion(float cartridge_k, bool for_crosshair = false);

    virtual int ShotsFired() { return 0; }

    virtual int GetCurrentFireMode() { return 1; }

    // ïàðàìåòû îðóæèÿ â çàâèñèìîòè îò åãî ñîñòîÿíèÿ èñïðàâíîñòè
    float GetConditionDispersionFactor() const;
    float GetConditionMisfireProbability() const;
    virtual float GetConditionToShow() const;

public:
    CameraRecoil cam_recoil; // simple mode (walk, run)
    CameraRecoil zoom_cam_recoil; // using zoom =(ironsight or scope)

protected:
    // ôàêòîð óâåëè÷åíèÿ äèñïåðñèè ïðè ìàêñèìàëüíîé èçíîøåíîñòè
    //(íà ñêîëüêî ïðîöåíòîâ óâåëè÷èòñÿ äèñïåðñèÿ)
    float fireDispersionConditionFactor;
    // âåðîÿòíîñòü îñå÷êè ïðè ìàêñèìàëüíîé èçíîøåíîñòè

    // modified by Peacemaker [17.10.08]
    //	float					misfireProbability;
    //	float					misfireConditionK;
    float misfireStartCondition; // èçíîøåííîñòü, ïðè êîòîðîé ïîÿâëÿåòñÿ øàíñ îñå÷êè
    float misfireEndCondition; // èçíîøåíîñòü ïðè êîòîðîé øàíñ îñå÷êè ñòàíîâèòñÿ êîíñòàíòíûì
    float misfireStartProbability; // øàíñ îñå÷êè ïðè èçíîøåíîñòè áîëüøå ÷åì misfireStartCondition
    float misfireEndProbability; // øàíñ îñå÷êè ïðè èçíîøåíîñòè áîëüøå ÷åì misfireEndCondition
    float conditionDecreasePerQueueShot; // óâåëè÷åíèå èçíîøåíîñòè ïðè âûñòðåëå î÷åðåäüþ
    float conditionDecreasePerShot; // óâåëè÷åíèå èçíîøåíîñòè ïðè îäèíî÷íîì âûñòðåëå

public:
    float GetMisfireStartCondition() const { return misfireStartCondition; };

    float GetMisfireEndCondition() const { return misfireEndCondition; };

protected:
    struct SPDM
    {
        float m_fPDM_disp_base;
        float m_fPDM_disp_vel_factor;
        float m_fPDM_disp_accel_factor;
        float m_fPDM_disp_crouch;
        float m_fPDM_disp_crouch_no_acc;
        float m_fPDM_disp_buckShot;
    };

    SPDM m_pdm;

    float m_crosshair_inertion;
    first_bullet_controller m_first_bullet_controller;

protected:
    // äëÿ îòäà÷è îðóæèÿ
    Fvector m_vRecoilDeltaAngle;

    // äëÿ ñòàëêåðîâ, ÷òîá îíè çíàëè ýôôåêòèâíûå ãðàíèöû èñïîëüçîâàíèÿ
    // îðóæèÿ
    float m_fMinRadius;
    float m_fMaxRadius;
    float m_fZoomRotateModifier;

protected:
    // äëÿ âòîðîãî ñòâîëà
    void StartFlameParticles2();
    void StopFlameParticles2();
    void UpdateFlameParticles2();

protected:
    shared_str m_sFlameParticles2;
    // îáúåêò ïàðòèêëîâ äëÿ ñòðåëüáû èç 2-ãî ñòâîëà
    CParticlesObject* m_pFlameParticles2;

protected:
    int GetAmmoCount(u8 ammo_type) const;

public:
    IC int GetAmmoElapsed() const { return m_ammoElapsed.type1; }

    IC int GetAmmoMagSize() const { return iMagazineSize; }
    IC int GetAmmoMagSize_ui() const { return iMagazineSize_ui; }

    int GetSuitableAmmoTotal(bool use_item_to_spawn = false) const;

    void SetAmmoElapsed(int ammo_count);

    virtual void OnMagazineEmpty();
    void SpawnAmmo(u32 boxCurr = 0xffffffff, LPCSTR ammoSect = NULL, u32 ParentID = 0xffffffff);
    bool SwitchAmmoType(u32 flags);

    float m_APk;

    virtual float Get_PDM_Base() const { return m_pdm.m_fPDM_disp_base; };

    virtual float Get_Silencer_PDM_Base() const { return cur_silencer_koef.pdm_base; };

    virtual float Get_Scope_PDM_Base() const { return cur_scope_koef.pdm_base; };

    virtual float Get_Launcher_PDM_Base() const { return cur_launcher_koef.pdm_base; };

    virtual float Get_PDM_BuckShot() const { return m_pdm.m_fPDM_disp_buckShot; };

    virtual float Get_PDM_Vel_F() const { return m_pdm.m_fPDM_disp_vel_factor; };

    virtual float Get_Silencer_PDM_Vel() const { return cur_silencer_koef.pdm_vel; };

    virtual float Get_Scope_PDM_Vel() const { return cur_scope_koef.pdm_vel; };

    virtual float Get_Launcher_PDM_Vel() const { return cur_launcher_koef.pdm_vel; };

    virtual float Get_PDM_Accel_F() const { return m_pdm.m_fPDM_disp_accel_factor; };

    virtual float Get_Silencer_PDM_Accel() const { return cur_silencer_koef.pdm_accel; };

    virtual float Get_Scope_PDM_Accel() const { return cur_scope_koef.pdm_accel; };

    virtual float Get_Launcher_PDM_Accel() const { return cur_launcher_koef.pdm_accel; };

    virtual float Get_PDM_Crouch() const { return m_pdm.m_fPDM_disp_crouch; };

    virtual float Get_PDM_Crouch_NA() const { return m_pdm.m_fPDM_disp_crouch_no_acc; };

    virtual float GetCrosshairInertion() const { return m_crosshair_inertion; };

    virtual float Get_Silencer_CrosshairInertion() const { return cur_silencer_koef.crosshair_inertion; };

    virtual float Get_Scope_CrosshairInertion() const { return cur_scope_koef.crosshair_inertion; };

    virtual float Get_Launcher_CrosshairInertion() const { return cur_launcher_koef.crosshair_inertion; };

    float GetFirstBulletDisp() const { return m_first_bullet_controller.get_fire_dispertion(); };

protected:
    // äëÿ ïîäñ÷åòà â GetSuitableAmmoTotal
    mutable int m_iAmmoCurrentTotal;
    mutable u32 m_BriefInfo_CalcFrame; // êàäð íà êîòîðîì ïðîñ÷èòàëè êîë-âî ïàòðîíîâ
    bool m_bAmmoWasSpawned;

    virtual bool IsNecessaryItem(const shared_str& item_sect);

public:
    struct ammo_type_t
    {
        union
        {
            u8 data;
            struct
            {
                u8 type1 : 4; // Type1 is normal ammo unless in grenade mode it's swapped
                u8 type2 : 4; // Type2 is grenade ammo unless in grenade mode it's swapped
            };
        };
    };
    ammo_type_t m_ammoType;

    struct ammo_elapsed_t
    {
        union
        {
            u16 data;
            struct
            {
                u16 type1 : 8; // Type1 is normal ammo unless in grenade mode it's swapped
                u16 type2 : 8; // Type2 is grenade ammo unless in grenade mode it's swapped
            };
        };
    };
    ammo_elapsed_t m_ammoElapsed;

    int iMagazineSize; // size (in bullets) of magazine
    int iMagazineSize2;
    int iMagazineSize_ui;

    bool m_bGrenadeMode;

    xr_vector<shared_str> m_ammoTypes;

    DEFINE_VECTOR(shared_str, SCOPES_VECTOR, SCOPES_VECTOR_IT);
    SCOPES_VECTOR m_scopes;
    SCOPES_VECTOR m_silencers;
    SCOPES_VECTOR m_launchers;

    bool m_altAimPos;
    u8 m_zoomtype;

    CWeaponAmmo* m_pCurrentAmmo;

    bool m_bHasTracers;
    u8 m_u8TracerColorID;
    u8 m_set_next_ammoType_on_reload;
    // Multitype ammo support
    xr_vector<CCartridge> m_magazine;
    CCartridge m_DefaultCartridge;
    CCartridge m_lastCartridge;
    float m_fCurrentCartirdgeDisp;

    bool unlimited_ammo();
    IC bool can_be_strapped() const { return m_can_be_strapped; };

    const decltype(m_magazine)& GetMagazine() { return m_magazine; };
    float GetMagazineWeight(const decltype(m_magazine)& mag) const;

    virtual float GetHitPower() { return fvHitPower[g_SingleGameDifficulty]; };
    virtual float GetHitPowerCritical() { return fvHitPowerCritical[g_SingleGameDifficulty]; };
    virtual float GetHitImpulse() { return fHitImpulse; };
    virtual float GetFireDistance() { return fireDistance; };

protected:
    u32 m_ef_main_weapon_type;
    u32 m_ef_weapon_type;

public:
    virtual u32 ef_main_weapon_type() const;
    virtual u32 ef_weapon_type() const;

    // Alundaio
    int GetAmmoCount_forType(shared_str const& ammo_type) const;
    virtual void set_ef_main_weapon_type(u32 type) { m_ef_main_weapon_type = type; };
    virtual void set_ef_weapon_type(u32 type) { m_ef_weapon_type = type; };
    virtual void SetAmmoType(u8 type) { m_ammoType.type1 = type; };
    u8 GetAmmoType() { return m_ammoType.type1; };
    //-Alundaio

protected:
    // This is because when scope is attached we can't ask scope for these params
    // therefore we should hold them by ourself :-((
    float m_addon_holder_range_modifier;
    float m_addon_holder_fov_modifier;

public:
    virtual void modify_holder_params(float& range, float& fov) const;

    virtual bool use_crosshair() const { return true; }

    bool show_crosshair();
    bool show_indicators();
    virtual BOOL ParentMayHaveAimBullet();

private:
    virtual bool install_upgrade_ammo_class(LPCSTR section, bool test);
    bool install_upgrade_disp(LPCSTR section, bool test);
    bool install_upgrade_hit(LPCSTR section, bool test);
    bool install_upgrade_hud(LPCSTR section, bool test);
    bool install_upgrade_addon(LPCSTR section, bool test);

protected:
    virtual bool install_upgrade_impl(LPCSTR section, bool test);

private:
    float m_hit_probability[egdCount];

public:
    const float& hit_probability() const;

private:
    Fvector m_overriden_activation_speed;
    bool m_activation_speed_is_overriden;
    virtual bool ActivationSpeedOverriden(Fvector& dest, bool clear_override);

public:
    float m_fLR_ShootingFactor; // Фактор горизонтального сдвига худа при стрельбе [-1; +1]
    float m_fUD_ShootingFactor; // Фактор вертикального сдвига худа при стрельбе [-1; +1]
    float m_fBACKW_ShootingFactor; // Фактор сдвига худа в сторону лица при стрельбе [0; +1]
public:  
    void AddHUDShootingEffect();
    virtual void SetActivationSpeedOverride(Fvector const& speed);
    virtual float GetInertionAimFactor() { return 1.f - m_zoom_params.m_fZoomRotationFactor; };
    //--> [От 1.0 - Инерция от бедра, до 0.0 - Инерция при зумме] Какую инерцию использовать

    virtual void DumpActiveParams(shared_str const& section_name, CInifile& dst_ini) const;

    virtual shared_str const GetAnticheatSectionName() const { return cNameSect(); };
};

namespace TargetColors
{
    constexpr D3DCOLOR EnemyColor = D3DCOLOR_RGBA(0xff, 0, 0, 0x80);
    constexpr D3DCOLOR NeutralColor = D3DCOLOR_RGBA(0xff, 0xff, 0x80, 0x80);
    constexpr D3DCOLOR FriendColor = D3DCOLOR_RGBA(0, 0xff, 0, 0x80);
} // namespace TargetColors