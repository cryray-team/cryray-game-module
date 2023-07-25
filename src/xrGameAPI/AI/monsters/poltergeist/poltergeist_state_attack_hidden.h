#pragma once
#include "../state.h"

namespace POLTERGEIST_ATTACK_HIDDEN
{
    template <typename _Object>
    class CStatePoltergeistAttackHidden1 : public CState<_Object>
    {
    protected:
        typedef CState<_Object> inherited;
        typedef CState<_Object>* state_ptr;

    public:
        CStatePoltergeistAttackHidden1(_Object* obj);
        virtual ~CStatePoltergeistAttackHidden1() {}

        virtual void initialize();
        virtual void execute();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

        bool check_home_point();

    private:
        void select_target_for_move();

        u32 m_fly_side_select_tick;
        float m_fly_radius_factor;
        bool m_fly_left;
        Fvector m_target;
        u32 m_target_vertex;
    };
}

namespace POLTERGEIST2_ATTACK_HIDDEN
{
    template <typename _Object>
    class CStatePoltergeistAttackHidden2 : public CState<_Object>
    {
    protected:
        typedef CState<_Object> inherited;
        typedef CState<_Object>* state_ptr;

        struct
        {
            Fvector point;
            u32 node;
        } m_target;

    public:
        CStatePoltergeistAttackHidden2(_Object* obj) : inherited(obj) {}
        virtual ~CStatePoltergeistAttackHidden2() {}

        virtual void initialize();
        virtual void execute();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

    private:
        void select_target_point();
    };
}
#include "poltergeist_state_attack_hidden_inline.h"
