#pragma once

#include "chimera_state_threaten_steal.h"
#include "chimera_state_threaten_walk.h"
#include "chimera_state_threaten_roar.h"

namespace ChimeraStateThreatenDetails
{
    constexpr float MinDistToEnemy = 3.0f;
    constexpr u32 ThreatenDelay = 10000;
}

namespace CHIMERA_THREATEN
{
    using namespace ChimeraStateThreatenDetails;

    template <typename _Object>
    CStateChimeraThreaten<_Object>::CStateChimeraThreaten(_Object* obj) : inherited(obj)
    {
        m_last_time_threaten = 0;
        add_state(eStateWalk, xr_new<CStateChimeraThreatenWalk<_Object>>(obj));
        add_state(eStateThreaten, xr_new<CStateChimeraThreatenRoar<_Object>>(obj));
        add_state(eStateSteal, xr_new<CStateChimeraThreatenSteal<_Object>>(obj));
    }

    template <typename _Object>
    CStateChimeraThreaten<_Object>::~CStateChimeraThreaten()
    {
    }

    template <typename _Object>
    void CStateChimeraThreaten<_Object>::reinit()
    {
        inherited::reinit();

        m_last_time_threaten = 0;
    }

    template <typename _Object>
    bool CStateChimeraThreaten<_Object>::check_start_conditions()
    {
        if (object->tfGetRelationType(object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy)
            return false;
        if (object->Position().distance_to(object->EnemyMan.get_enemy_position()) < MinDistToEnemy)
            return false;
        if (object->HitMemory.is_hit())
            return false;
        if (object->hear_dangerous_sound)
            return false;
        if (m_last_time_threaten + ThreatenDelay > Device.dwTimeGlobal)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateChimeraThreaten<_Object>::check_completion()
    {
        if (object->Position().distance_to(object->EnemyMan.get_enemy_position()) < MinDistToEnemy)
            return true;
        if (object->HitMemory.is_hit())
            return true;
        if (object->tfGetRelationType(object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy)
            return true;

        return false;
    }

    template <typename _Object>
    void CStateChimeraThreaten<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    void CStateChimeraThreaten<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            select_state(eStateThreaten);
            return;
        }

        if (prev_substate == eStateSteal)
        {
            select_state(eStateThreaten);
            return;
        }

        if (prev_substate == eStateThreaten)
        {
            if (get_state(eStateSteal)->check_start_conditions())
            {
                select_state(eStateSteal);
                return;
            }
            else if (get_state(eStateWalk)->check_start_conditions())
            {
                select_state(eStateWalk);
                return;
            }
        }

        select_state(eStateThreaten);
    }

    template <typename _Object>
    void CStateChimeraThreaten<_Object>::finalize()
    {
        inherited::finalize();
        m_last_time_threaten = Device.dwTimeGlobal;
    }

    template <typename _Object>
    void CStateChimeraThreaten<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        m_last_time_threaten = Device.dwTimeGlobal;
    }
}

namespace CHIMERA2_THREATEN
{
    using namespace ChimeraStateThreatenDetails;

    template <typename _Object>
    CStateChimera2Threaten<_Object>::CStateChimera2Threaten(_Object* obj) : inherited(obj)
    {
        add_state(eStateWalk, xr_new<CHIMERA2_THREATEN_WALK::CStateChimera2ThreatenWalk<_Object>>(obj));
        add_state(eStateThreaten, xr_new<CHIMERA2_THREATEN_ROAR::CStateChimera2ThreatenRoar<_Object>>(obj));
        add_state(eStateSteal, xr_new<CHIMERA2_THREATEN_STEAL::CStateChimera2ThreatenSteal<_Object>>(obj));
    }

    template <typename _Object>
    CStateChimera2Threaten<_Object>::~CStateChimera2Threaten()
    {
    }

    template <typename _Object>
    void CStateChimera2Threaten<_Object>::reinit()
    {
        inherited::reinit();

        m_last_time_threaten = 0;
    }

    template <typename _Object>
    bool CStateChimera2Threaten<_Object>::check_start_conditions()
    {
        if (object->tfGetRelationType(object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy)
            return false;
        if (object->Position().distance_to(object->EnemyMan.get_enemy_position()) < MinDistToEnemy)
            return false;
        if (object->HitMemory.is_hit())
            return false;
        if (object->hear_dangerous_sound)
            return false;
        if (m_last_time_threaten + ThreatenDelay > Device.dwTimeGlobal)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateChimera2Threaten<_Object>::check_completion()
    {
        if (object->Position().distance_to(object->EnemyMan.get_enemy_position()) < MinDistToEnemy)
            return true;
        if (object->HitMemory.is_hit())
            return true;
        if (object->tfGetRelationType(object->EnemyMan.get_enemy()) == ALife::eRelationTypeWorstEnemy)
            return true;

        return false;
    }

    template <typename _Object>
    void CStateChimera2Threaten<_Object>::initialize()
    {
        inherited::initialize();
        object->SetUpperState();
    }

    template <typename _Object>
    void CStateChimera2Threaten<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            select_state(eStateThreaten);
            return;
        }

        if (prev_substate == eStateSteal)
        {
            select_state(eStateThreaten);
            return;
        }

        if (prev_substate == eStateThreaten)
        {
            if (get_state(eStateSteal)->check_start_conditions())
            {
                select_state(eStateSteal);
                return;
            }
            else if (get_state(eStateWalk)->check_start_conditions())
            {
                select_state(eStateWalk);
                return;
            }
        }

        select_state(eStateThreaten);
    }

    template <typename _Object>
    void CStateChimera2Threaten<_Object>::finalize()
    {
        inherited::finalize();
        object->SetUpperState(false);
        m_last_time_threaten = Device.dwTimeGlobal;
    }

    template <typename _Object>
    void CStateChimera2Threaten<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        object->SetUpperState(false);
        m_last_time_threaten = Device.dwTimeGlobal;
    }
}