#pragma once
#include "../state.h"

namespace BURER_STATE_ATTACK
{
    template <typename Object>
    class CStateBurerAttack : public CState<Object>
    {
        typedef CState<Object> inherited;
        typedef CState<Object>* state_ptr;

    public:
        CStateBurerAttack(Object* obj);

        virtual void initialize();
        virtual void execute();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

        virtual void finalize();
        virtual void critical_finalize();
        virtual bool check_control_start_conditions(ControlCom::EControlType type);

    private:
        bool m_wait_state_end;
        bool m_lost_delta_health;
        bool m_allow_anti_aim;
        float m_last_health;
        TTime m_next_runaway_allowed_tick;
    };
}

namespace BURER2_STATE_ATTACK
{
    template <typename _Object>
    class CStateBurer2Attack : public CState<_Object>
    {
        typedef CState<_Object> inherited;
        typedef CState<_Object>* state_ptr;

        bool m_force_gravi;

    public:
        CStateBurer2Attack(_Object* obj);

        virtual void initialize();

        virtual void reselect_state();
        virtual void setup_substates();
        virtual void check_force_state();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    };
}

#include "burer_state_attack_inline.h"
