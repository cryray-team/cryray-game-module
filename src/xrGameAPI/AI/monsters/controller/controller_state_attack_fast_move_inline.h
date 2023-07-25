#pragma once

#define CStateControllerFastMoveAbstract CStateControllerFastMove<_Object>

template <typename _Object>
void CStateControllerFastMoveAbstract::initialize()
{
    inherited::initialize();

    object->set_mental_state(CController::eStateIdle);
}

template <typename _Object>
void CStateControllerFastMoveAbstract::finalize()
{
    inherited::finalize();
    object->set_mental_state(CController::eStateDanger);
}

template <typename _Object>
void CStateControllerFastMoveAbstract::critical_finalize()
{
    inherited::critical_finalize();
    object->set_mental_state(CController::eStateDanger);
}

template <typename _Object>
void CStateControllerFastMoveAbstract::execute()
{
    object->set_action(ACT_RUN);

    // select another cover
}

#undef CStateControllerFastMoveAbstract
