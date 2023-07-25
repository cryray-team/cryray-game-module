#pragma once
#include "../state.h"

namespace CHIMERA_HUNTING_COME_OUT
{
    template <typename _Object>
    class CStateChimeraHuntingComeOut : public CState<_Object>
    {
    protected:
        typedef CState<_Object> inherited;

    public:
        CStateChimeraHuntingComeOut(_Object* obj);

        virtual void reselect_state();
        virtual bool check_start_conditions();
        virtual bool check_completion();
    };
}

namespace CHIMERA2_HUNTING_COME_OUT
{
    template <typename _Object>
    class CStateChimera2HuntingComeOut : public CState<_Object>
    {
    protected:
        typedef CState<_Object> inherited;

    public:
        CStateChimera2HuntingComeOut(_Object* obj);

        virtual void reselect_state();
        virtual bool check_start_conditions();
        virtual bool check_completion();
    };
}

#include "chimera_state_hunting_come_out_inline.h"
