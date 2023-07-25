#pragma once
#include "../states/state_custom_action.h"

namespace BLOODSUCKER_STATE_JUMP
{
    template <typename _Object>
    CStateCaptureJumpBloodsucker<_Object>::CStateCaptureJumpBloodsucker(_Object* obj) : inherited(obj)
    {
        add_state(eStateCustom, xr_new<CStateMonsterCustomAction<_Object>>(obj));
    }

    template <typename _Object>
    CStateCaptureJumpBloodsucker<_Object>::~CStateCaptureJumpBloodsucker()
    {
    }

    template <typename _Object>
    void CStateCaptureJumpBloodsucker<_Object>::execute()
    {
        // check alife control
        select_state(eStateCustom);

        get_state_current()->execute();
        prev_substate = current_substate;
    }

    template <typename _Object>
    void CStateCaptureJumpBloodsucker<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();
        if (current_substate == eStateCustom)
        {
            SStateDataAction data;

            data.action = ACT_STAND_IDLE;
            data.time_out = 0; // do not use time out
            /*data.sound_type	= MonsterSound::eMonsterSoundIdle;
            data.sound_delay = object->db().m_dwIdleSndDelay;
            */
            state->fill_data_with(&data, sizeof(SStateDataAction));

            return;
        }
    }
}

namespace BLOODSUCKER2_STATE_JUMP
{
    template <typename _Object>
    CStateCaptureJumpBloodsucker2<_Object>::CStateCaptureJumpBloodsucker2(_Object* obj) : inherited(obj)
    {
        add_state(eStateCustom, xr_new<CStateMonsterCustomAction<_Object>>(obj));
    }

    template <typename _Object>
    CStateCaptureJumpBloodsucker2<_Object>::~CStateCaptureJumpBloodsucker2()
    {
    }

    template <typename _Object>
    void CStateCaptureJumpBloodsucker2<_Object>::execute()
    {
        // check alife control
        select_state(eStateCustom);

        get_state_current()->execute();
        prev_substate = current_substate;
    }

    template <typename _Object>
    void CStateCaptureJumpBloodsucker2<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();
        if (current_substate == eStateCustom)
        {
            SStateDataAction data;

            data.action = ACT_STAND_IDLE;
            data.time_out = 0; // do not use time out
            state->fill_data_with(&data, sizeof(SStateDataAction));

            return;
        }
    }
}