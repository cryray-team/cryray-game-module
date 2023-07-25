#pragma once
#include "../state.h"

namespace BURER_ATTACK_GRAVE
{
    template <typename _Object>
    class CStateBurerAttackGravi : public CState<_Object>
    {
        typedef CState<_Object> inherited;

    public:
        CStateBurerAttackGravi(_Object* obj);

        virtual void initialize();
        virtual void execute();
        virtual void finalize();
        virtual void critical_finalize();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

        virtual bool check_start_conditions();
        virtual bool check_completion();

    private:
        // выполнять состояние
        void ExecuteGraviStart();
        void ExecuteGraviContinue();
        void ExecuteGraviFire();

    private:
        enum
        {
            ACTION_GRAVI_STARTED,
            ACTION_GRAVI_CONTINUE,
            ACTION_GRAVI_FIRE,
            ACTION_WAIT_ANIM_END,
            ACTION_COMPLETED,

        } m_action;

        u32 m_time_gravi_started;
        TTime m_next_gravi_allowed_tick;
        TTime m_anim_end_tick;
    };
}

namespace BURER2_ATTACK_GRAVE
{
    template <typename _Object>
    class CStateBurer2AttackGravi : public CState<_Object>
    {
        typedef CState<_Object> inherited;

        enum
        {
            ACTION_GRAVI_STARTED,
            ACTION_GRAVI_CONTINUE,
            ACTION_GRAVI_FIRE,
            ACTION_WAIT_TRIPLE_END,
            ACTION_COMPLETED,
        } m_action;

        u32 time_gravi_started;

    public:
        CStateBurer2AttackGravi(_Object* obj);

        virtual void initialize();
        virtual void execute();
        virtual void finalize();
        virtual void critical_finalize();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }

        virtual bool check_start_conditions();
        virtual bool check_completion();

    private:
        // выполнять состояние
        void ExecuteGraviStart();
        void ExecuteGraviContinue();
        void ExecuteGraviFire();
    };
}

#include "burer_state_attack_gravi_inline.h"
