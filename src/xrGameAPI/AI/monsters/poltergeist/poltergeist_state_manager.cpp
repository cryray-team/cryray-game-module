#include "stdafx.h"
#include "poltergeist.h"
#include "poltergeist2.h"
#include "poltergeist_state_manager.h"

#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"
#include "../control_path_builder_base.h"

#include "poltergeist_state_rest.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_attack.h"
#include "../states/monster_state_panic.h"
#include "poltergeist_state_attack_hidden.h"
#include "../states/monster_state_hear_int_sound.h"
#include "../states/monster_state_hear_danger_sound.h"
#include "../states/monster_state_hitted.h"
#include "entitycondition.h"

CStateManagerPoltergeist::CStateManagerPoltergeist(CPoltergeist* obj) : inherited(obj)
{
    add_state(eStateRest, xr_new<POLTERGEIST_REST::CPoltergeistStateRest<CPoltergeist>>(obj));
    add_state(eStateEat, xr_new<CStateMonsterEat<CPoltergeist>>(obj));
    add_state(eStateAttack_AttackHidden, xr_new<POLTERGEIST_ATTACK_HIDDEN::CStatePoltergeistAttackHidden1<CPoltergeist>>(obj));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CPoltergeist>>(obj));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CPoltergeist>>(obj));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CPoltergeist>>(obj));
    add_state(eStateHearDangerousSound, xr_new<CStateMonsterHearDangerousSound<CPoltergeist>>(obj));
}

CStateManagerPoltergeist2::CStateManagerPoltergeist2(CPoltergeist2* obj) : inherited(obj)
{
    CStateMonsterAttackMoveToHomePoint<CPoltergeist2>* PolterAttack_Hands =
        xr_new<CStateMonsterAttackMoveToHomePoint<CPoltergeist2>>(obj);
    add_state(eStateRest, xr_new<POLTERGEIST2_REST::CPoltergeist2StateRest<CPoltergeist2>>(obj));
    add_state(eStateEat, xr_new<CStateMonsterEat<CPoltergeist2>>(obj));
    add_state(eStateAttack, xr_new<CStateMonsterAttack<CPoltergeist2>>(obj, PolterAttack_Hands));
    add_state(eStateAttack_AttackHidden, xr_new<POLTERGEIST2_ATTACK_HIDDEN::CStatePoltergeistAttackHidden2<CPoltergeist2>>(obj));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CPoltergeist2>>(obj));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CPoltergeist2>>(obj));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CPoltergeist2>>(obj));
    add_state(eStateHearDangerousSound, xr_new<CStateMonsterHearDangerousSound<CPoltergeist2>>(obj));
}

CStateManagerPoltergeist::~CStateManagerPoltergeist() {}
CStateManagerPoltergeist2::~CStateManagerPoltergeist2() {}

void CStateManagerPoltergeist::reinit()
{
    inherited::reinit();

    time_next_flame_attack = 0;
    time_next_tele_attack = 0;
    time_next_scare_attack = 0;
}

void CStateManagerPoltergeist2::reinit()
{
    inherited::reinit();

    time_next_flame_attack = 0;
    time_next_tele_attack = 0;
    time_next_scare_attack = 0;
}

void CStateManagerPoltergeist::execute()
{
    u32 state_id = u32(-1);

    if (object->EnemyMan.get_enemy() && object->detected_enemy())
    {
        state_id = eStateAttack_AttackHidden;
    }
    else
    {
        state_id = eStateRest;
    }

    select_state(state_id);

    // выполнить текущее состояние
    get_state_current()->execute();

    prev_substate = current_substate;
}

void CStateManagerPoltergeist2::execute()
{
    u32 state_id = u32(-1);
    const CEntityAlive* enemy = object->EnemyMan.get_enemy();

    //--' OldSerpskiStalker
    if (m_tele && !object->logic_ghost)
    {
        if (enemy)
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong:
                object->on_activate();
                state_id = eStatePanic;
                break;
            case eWeak:
                if (enemy->conditions().health() < 0.50f) // Видим раненного врага, спустимся на землю и добьем рукой.
                                                          // Относится к атеру, если бежать уже не можем
                { // CStateMonsterAttack<CPoltergeist_cs>(obj, PolterAttack_Hands)
                    object->on_deactivate();
                    state_id = eStateAttack;
                }
                else
                {
                    state_id = eStatePanic; // У врага больше 50 %, летаем и кидаемся предметами
                    object->on_activate();
                }
                break;
            }
        }
        else if (object->HitMemory.is_hit())
        {
            state_id = eStateHitted;
        }
        else if (object->hear_interesting_sound || object->hear_dangerous_sound)
        {
            state_id = eStateHearDangerousSound;
        }
        else
        {
            if (can_eat())
                state_id = eStateEat;
            else
            {
                state_id = eStateRest;
                object->on_activate(); // Летаем дальше, пока голод не упадет
            }
        }

        if (state_id == eStateEat) // Если мы кушаем, проверить дист. до трупа, спустится на землю
        {
            if (object->CorpseMan.get_corpse()->Position().distance_to(object->Position()) < 10.f)
            {
                object->on_deactivate(); // Отключить полет
            }
        }
    }
    else
        state_id = eStateRest; // Если тип полтергейста огненный, на землю не спускаемся - летаем, отключается логика

    if (object->logic_ghost)
    {
        if (enemy)
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong:
            case eWeak:
                if (object->state_invisible == false)
                    state_id = eStateAttack;
                else
                    state_id = eStatePanic;
                break;
            }
        }
        else if (object->HitMemory.is_hit())
            state_id = eStateHitted;
        else if (object->hear_interesting_sound || object->hear_dangerous_sound)
            state_id = eStateHearDangerousSound;
        else
        {
            if (can_eat())
                state_id = eStateEat;
            else
            {
                state_id = eStateRest;
                object->on_activate(); // Летаем дальше, пока голод не упадет
            }
        }

        if (state_id == eStateEat) // Если мы кушаем, проверить дист. до трупа, спустится на землю
        {
            if (object->CorpseMan.get_corpse()->Position().distance_to(object->Position()) < 10.f)
            {
                object->on_deactivate(); // Отключить полет
            }
        }
    }

    select_state(state_id);
    get_state_current()->execute();
    prev_substate = current_substate;
}
