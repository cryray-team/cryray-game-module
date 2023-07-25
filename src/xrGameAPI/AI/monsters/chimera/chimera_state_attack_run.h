#pragma once
#include "../state.h"

namespace CHIMERA2_STATE_ATTACK_RUN
{
    template <typename _Object>
    class CStateChimera2AttackRun : public CState<_Object>
    {
        typedef CState<_Object> inherited;
        typedef CState<_Object>* state_ptr;

        TTime m_time_action_change;
        EAction action;

    public:
        CStateChimera2AttackRun(_Object* obj);
        virtual ~CStateChimera2AttackRun();

        virtual void initialize();
        virtual void execute();
        virtual void finalize();
        virtual void critical_finalize();
        bool check_home_point();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

    private:
        void choose_action();
    };
}

#include "chimera_state_attack_run_inline.h"