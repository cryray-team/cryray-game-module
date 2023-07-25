#pragma once
#include "../state.h"

namespace CHIMERA_THREATEN_WALK
{
    template <typename _Object>
    class CStateChimeraThreatenWalk : public CStateMonsterMoveToPointEx<_Object>
    {
        typedef CStateMonsterMoveToPointEx<_Object> inherited;

    public:
        IC CStateChimeraThreatenWalk(_Object* obj) : inherited(obj) {}
        virtual void initialize();
        virtual void execute();
        virtual bool check_completion();
        virtual bool check_start_conditions();
    };
}

namespace CHIMERA2_THREATEN_WALK
{
    template <typename _Object>
    class CStateChimera2ThreatenWalk : public CStateMonsterMoveToPointEx<_Object>
    {
        typedef CStateMonsterMoveToPointEx<_Object> inherited;

    public:
        IC CStateChimera2ThreatenWalk(_Object* obj) : inherited(obj) {}
        virtual void initialize();
        virtual void execute();
        virtual bool check_completion();
        virtual bool check_start_conditions();
    };
}

#include "chimera_state_threaten_walk_inline.h"
