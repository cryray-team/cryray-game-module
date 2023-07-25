#pragma once

namespace CHIMERA_HUNTING_COME_HUNTING_MOVE_TO_COVER
{
    template <typename _Object>
    CStateChimeraHuntingMoveToCover<_Object>::CStateChimeraHuntingMoveToCover(_Object* obj) : inherited(obj)
    {
    }

    template <typename _Object>
    void CStateChimeraHuntingMoveToCover<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    bool CStateChimeraHuntingMoveToCover<_Object>::check_completion()
    {
        return false;
    }

    template <typename _Object>
    void CStateChimeraHuntingMoveToCover<_Object>::execute()
    {
    }
}

namespace CHIMERA2_HUNTING_COME_HUNTING_MOVE_TO_COVER
{
    template <typename _Object>
    CStateChimera2HuntingMoveToCover<_Object>::CStateChimera2HuntingMoveToCover(_Object* obj) : inherited(obj)
    {
    }

    template <typename _Object>
    void CStateChimera2HuntingMoveToCover<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    bool CStateChimera2HuntingMoveToCover<_Object>::check_completion()
    {
        return false;
    }

    template <typename _Object>
    void CStateChimera2HuntingMoveToCover<_Object>::execute()
    {
    }
}