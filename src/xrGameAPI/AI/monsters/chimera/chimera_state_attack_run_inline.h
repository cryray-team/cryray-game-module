#pragma once

namespace CHIMERA2_STATE_ATTACK_RUN
{
    template <typename _Object>
    CStateChimera2AttackRun<_Object>::CStateChimera2AttackRun(_Object* obj) : inherited(obj)
    {
        add_state(eStateAttack_MoveToHomePoint, xr_new<CStateMonsterAttackMoveToHomePoint<CChimera2>>(obj));
        add_state(eStateAttack_Run_chimera, xr_new<CStateMonsterAttackRun<CChimera2>>(obj));
        add_state(eStateAttack_Melee_chimera, xr_new<CStateMonsterAttackMelee<CChimera2>>(obj));
    }

    template <typename _Object>
    CStateChimera2AttackRun<_Object>::~CStateChimera2AttackRun()
    {
    }

    template <typename _Object>
    bool CStateChimera2AttackRun<_Object>::check_home_point()
    {
        if (prev_substate != eStateAttack_MoveToHomePoint)
        {
            if (get_state(eStateAttack_MoveToHomePoint)->check_start_conditions())
                return true;
        }
        else
        {
            if (!get_state(eStateAttack_MoveToHomePoint)->check_completion())
                return true;
        }

        return false;
    }

    template <typename _Object>
    void CStateChimera2AttackRun<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    void CStateChimera2AttackRun<_Object>::execute()
    {
        object->anim().clear_override_animation();

        if (check_home_point())
        {
            select_state(eStateAttack_MoveToHomePoint);
            get_state_current()->execute();
            prev_substate = current_substate;
            return;
        }

        EMonsterState state_id = eStateUnknown;
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();

        if (current_substate == eStateAttack_Melee_chimera)
        {
            if (get_state(eStateAttack_Melee_chimera)->check_completion())
                state_id = eStateAttack_Run_chimera;
            else
                state_id = eStateAttack_Melee_chimera;
        }
        else
        {
            if (get_state(eStateAttack_Melee_chimera)->check_start_conditions())
                state_id = eStateAttack_Melee_chimera;
            else
                state_id = eStateAttack_Run_chimera;
        }

        select_state(state_id);
        get_state_current()->execute();
        prev_substate = current_substate;
    }

    template <typename _Object>
    void CStateChimera2AttackRun<_Object>::choose_action()
    {
    }

    template <typename _Object>
    void CStateChimera2AttackRun<_Object>::finalize()
    {
        inherited::finalize();
    }

    template <typename _Object>
    void CStateChimera2AttackRun<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
    }
}