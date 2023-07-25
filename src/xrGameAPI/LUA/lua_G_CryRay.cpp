#include "Stdafx.h"
#include "Actor.h"
#include "ai_space.h"
#include "lua_G_CryRay.h"
#include "script_engine.h"

namespace lua_G_CryRay
{
    void KillActorScript()
    {
        LPCSTR game_over1{};
        luabind::functor<LPCSTR> m_functor1;
        R_ASSERT(ai().script_engine().functor("_g_cryray.KillActor", m_functor1));
        game_over1 = m_functor1();
    }

    void GameOverScript()
    {
        LPCSTR game_over1{};
        luabind::functor<LPCSTR> m_functor1;
        R_ASSERT(ai().script_engine().functor("_g_cryray.GameOverScript", m_functor1));
        game_over1 = m_functor1();
    }

    void UseDoorScript()
    {
        LPCSTR animation_door;
        luabind::functor<LPCSTR> m_functor;
        R_ASSERT(ai().script_engine().functor("_g_cryray.EffectUseDoor", m_functor));
        animation_door = m_functor();
    }

    void ControllerPsyHitScript()
    {
        LPCSTR _execute;
        luabind::functor<LPCSTR> m_function;
        R_ASSERT(ai().script_engine().functor("_g_cryray.ControllerPsyHit", m_function));
        _execute = m_function();
    }

    void SnorkHitInJumpScript()
    {
        LPCSTR _execute;
        luabind::functor<LPCSTR> m_function;
        R_ASSERT(ai().script_engine().functor("_g_cryray.SnorkHitInJump", m_function));
        _execute = m_function();
    }

    void PseudoGigantThreatenExecuteScript()
    {
        LPCSTR _execute;
        luabind::functor<LPCSTR> m_function;
        R_ASSERT(ai().script_engine().functor("_g_cryray.PseudoGigantThreatenExecute", m_function));
        _execute = m_function();
    }

    void CDetectorCheckerScript()
    {
        LPCSTR detector_checker;
        luabind::functor<LPCSTR> read_only;
        R_ASSERT(ai().script_engine().functor("_g_cryray.DetectorUsed", read_only));
        detector_checker = read_only();
    }

    void CFlashCardScript()
    {
        LPCSTR flash_use;
        luabind::functor<LPCSTR> m_functor_flash;
        R_ASSERT(ai().script_engine().functor("_g_cryray.FlashInfoUse", m_functor_flash));
        flash_use = m_functor_flash();
    }

    void UpdateReactionArtefactScript()
    {
        LPCSTR artefact_reaction;
        luabind::functor<LPCSTR> read_only;
        R_ASSERT(ai().script_engine().functor("_g_cryray.UpdateReactionArtefact", read_only));
        artefact_reaction = read_only();
    }

    void ActorShowPDAONScript()
    {
        LPCSTR status1;
        luabind::functor<LPCSTR> m_functor;
        R_ASSERT(ai().script_engine().functor("_g_cryray.PdaUIShowOn", m_functor));
        status1 = m_functor();
    }

    void ActorShowPDAOFFScript()
    {
        LPCSTR status0;
        luabind::functor<LPCSTR> m_functor;
        R_ASSERT(ai().script_engine().functor("_g_cryray.PdaUIShowOff", m_functor));
        status0 = m_functor();
    }

    void WeaponJammedScript()
    {
        LPCSTR jam;
        luabind::functor<LPCSTR> jammed;
        R_ASSERT(ai().script_engine().functor("_g_cryray.WeaponJammed", jammed));
        jam = jammed();
    }

    void ActorShowMenuOnScript()
    {
        LPCSTR status1;
        luabind::functor<LPCSTR> m_functor;
        R_ASSERT(ai().script_engine().functor("_g_cryray.ActorShowMenuOn", m_functor));
        status1 = m_functor();
    }

    void ActorShowMenuOffScript()
    {
        LPCSTR status0;
        luabind::functor<LPCSTR> m_functor;
        R_ASSERT(ai().script_engine().functor("_g_cryray.ActorShowMenuOff", m_functor));
        status0 = m_functor();
    }

    float m_hit_explode_actor = 0.f;

    float GetHitExplode() { return m_hit_explode_actor; }

    void get_gist_to_explode_hit(float v)
    {
        m_hit_explode_actor = v;
        Msg(" ::[%s]v = %f", __FUNCTION__, m_hit_explode_actor);
    };

    void DistToExplodeHit()
    {
        LPCSTR explode;
        luabind::functor<LPCSTR> hit;
        R_ASSERT(ai().script_engine().functor("_g_cryray.DistToExplodeHit", hit));
        explode = hit();
    }

    float get_blender_mode_main() { return g_pGamePersistent->m_pGShaderConstants->m_blender_mode.x; }

    float set_blender_mode_main(float blender_num = 0.f)
    {
        g_pGamePersistent->m_pGShaderConstants->m_blender_mode.x = blender_num;
        return g_pGamePersistent->m_pGShaderConstants->m_blender_mode.x;
    }

    float get_blender_mode_second() { return g_pGamePersistent->m_pGShaderConstants->m_blender_mode.y; }

    float set_blender_mode_second(float blender_num = 0.f)
    {
        g_pGamePersistent->m_pGShaderConstants->m_blender_mode.y = blender_num;
        return g_pGamePersistent->m_pGShaderConstants->m_blender_mode.y;
    }

    void set_shader_params(const Fmatrix& m_params)
    {
        g_pGamePersistent->m_pGShaderConstants->m_script_params = m_params;
    }

    Fmatrix get_shader_params() { return g_pGamePersistent->m_pGShaderConstants->m_script_params; }
    }