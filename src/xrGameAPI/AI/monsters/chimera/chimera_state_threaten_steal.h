#pragma once
#include "../state.h"

namespace CHIMERA_THREATEN_STEAL
{
    template <typename _Object>
    class CStateChimeraThreatenSteal : public CStateMonsterMoveToPointEx<_Object>
    {
        typedef CStateMonsterMoveToPointEx<_Object> inherited;

    public:
        IC CStateChimeraThreatenSteal(_Object* obj) : inherited(obj) {}
        virtual void initialize();
        virtual void finalize();
        virtual void execute();
        virtual bool check_completion();
        virtual bool check_start_conditions();
    };
}

namespace CHIMERA2_THREATEN_STEAL
{
    template <typename _Object>
    class CStateChimera2ThreatenSteal : public CStateMonsterMoveToPointEx<_Object>
    {
        typedef CStateMonsterMoveToPointEx<_Object> inherited;

    public:
        IC CStateChimera2ThreatenSteal(_Object* obj) : inherited(obj) {}
        virtual void initialize();
        virtual void finalize();
        virtual void execute();
        virtual bool check_completion();
        virtual bool check_start_conditions();
    };
}

#include "chimera_state_threaten_steal_inline.h"
