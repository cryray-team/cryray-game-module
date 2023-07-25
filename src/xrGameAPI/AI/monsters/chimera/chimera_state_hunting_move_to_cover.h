#pragma once
#include "../state.h"

namespace CHIMERA_HUNTING_COME_HUNTING_MOVE_TO_COVER
{
    template <typename _Object>
    class CStateChimeraHuntingMoveToCover : public CState<_Object>
    {
    protected:
        typedef CState<_Object> inherited;

    public:
        CStateChimeraHuntingMoveToCover(_Object* obj);

        virtual void initialize();
        virtual void execute();
        virtual bool check_start_conditions();
        virtual bool check_completion();
    };
}

namespace CHIMERA2_HUNTING_COME_HUNTING_MOVE_TO_COVER
{
    template <typename _Object>
    class CStateChimera2HuntingMoveToCover : public CState<_Object>
    {
        protected:
            typedef CState<_Object> inherited;

        public:
            CStateChimera2HuntingMoveToCover(_Object* obj);

            virtual void initialize();
            virtual void execute();
            virtual bool check_start_conditions();
            virtual bool check_completion();
    };
}

#include "chimera_state_hunting_move_to_cover_inline.h"
