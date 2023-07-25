#pragma once

#include "controller_psy_hit.h"

#define CStateControllerTubeAbstract CStateControllerTube<_Object>

template <typename _Object>
void CStateControllerTubeAbstract::execute()
{
    object->control().activate(ControlCom::eComCustom1);
    object->set_action(ACT_STAND_IDLE);
}

#define SEE_ENEMY_DURATION 1000

template <typename _Object>
bool CStateControllerTubeAbstract::check_start_conditions()
{
    if (object->EnemyMan.see_enemy_duration() < SEE_ENEMY_DURATION)
        return false;
    if (!object->m_psy_hit->check_start_conditions())
        return false;

    return true;
}

template <typename _Object>
bool CStateControllerTubeAbstract::check_completion()
{
    return (!object->m_psy_hit->is_active());
}

#undef CStateControllerTubeAbstract
