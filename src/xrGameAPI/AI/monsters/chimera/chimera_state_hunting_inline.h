#pragma once

#include "chimera_state_hunting_move_to_cover.h"
#include "chimera_state_hunting_come_out.h"

namespace CHIMERA_HUNTING
{
    template <typename _Object>
    CStateChimeraHunting<_Object>::CStateChimeraHunting(_Object* obj) : inherited(obj)
    {
        add_state(eStateMoveToCover, xr_new<CStateChimeraHuntingMoveToCover<_Object>>(obj));
        add_state(eStateComeOut, xr_new<CStateChimeraHuntingComeOut<_Object>>(obj));
    }

    template <typename _Object>
    bool CStateChimeraHunting<_Object>::check_start_conditions()
    {
        return true;
    }

    template <typename _Object>
    bool CStateChimeraHunting<_Object>::check_completion()
    {
        return false;
    }

    template <typename _Object>
    void CStateChimeraHunting<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
            select_state(eStateMoveToCover);
        else if (prev_substate == eStateMoveToCover)
            select_state(eStateComeOut);
        else
            select_state(eStateMoveToCover);
    }
}

namespace CHIMERA2_HUNTING
{
    template <typename _Object>
    CStateChimera2Hunting<_Object>::CStateChimera2Hunting(_Object* obj) : inherited(obj)
    {
        add_state(eStateMoveToCover, xr_new<CStateChimera2HuntingMoveToCover<_Object>>(obj));
        add_state(eStateComeOut, xr_new<CStateChimera2HuntingComeOut<_Object>>(obj));
    }

    template <typename _Object>
    bool CStateChimera2Hunting<_Object>::check_start_conditions()
    {
        return true;
    }

    template <typename _Object>
    bool CStateChimera2Hunting<_Object>::check_completion()
    {
        return false;
    }

    template <typename _Object>
    void CStateChimera2Hunting<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
            select_state(eStateMoveToCover);
        else if (prev_substate == eStateMoveToCover)
            select_state(eStateComeOut);
        else
            select_state(eStateMoveToCover);
    }
}