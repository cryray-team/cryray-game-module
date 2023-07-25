#pragma once
#include "../state.h"

namespace CHIMERA_THREATEN_ROAR
{
    template <typename _Object>
    class CStateChimeraThreatenRoar : public CState<_Object>
    {
        typedef CState<_Object> inherited;

    public:
        IC CStateChimeraThreatenRoar(_Object* obj) : inherited(obj) {}

        virtual void initialize();
        virtual void execute();
        virtual bool check_completion();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    };
}

namespace CHIMERA2_THREATEN_ROAR
{
    template <typename _Object>
    class CStateChimera2ThreatenRoar : public CState<_Object>
    {
        typedef CState<_Object> inherited;

    public:
        IC CStateChimera2ThreatenRoar(_Object* obj) : inherited(obj) {}

        virtual void initialize();
        virtual void execute();
        virtual bool check_completion();
        virtual void remove_links(CObject* object) { inherited::remove_links(object); }
    };
}

#include "chimera_state_threaten_roar_inline.h"
