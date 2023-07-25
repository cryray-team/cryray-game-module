#pragma once

namespace CHIMERA_THREATEN_ROAR
{
    template <typename _Object>
    void CStateChimeraThreatenRoar<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    void CStateChimeraThreatenRoar<_Object>::execute()
    {
        object->set_action(ACT_STAND_IDLE);
        object->anim().SetSpecParams(ASP_THREATEN);
        object->set_state_sound(MonsterSound::eMonsterSoundThreaten);
        object->dir().face_target(object->EnemyMan.get_enemy(), 1200);
    }

    #define STATE_TIME_OUT 4000

    template <typename _Object>
    bool CStateChimeraThreatenRoar<_Object>::check_completion()
    {
        if (time_state_started + STATE_TIME_OUT < Device.dwTimeGlobal)
            return true;

        return false;
    }
}

namespace CHIMERA2_THREATEN_ROAR
{
    template <typename _Object>
    void CStateChimera2ThreatenRoar<_Object>::initialize()
    {
        inherited::initialize();
    }

    template <typename _Object>
    void CStateChimera2ThreatenRoar<_Object>::execute()
    {
        object->set_action(ACT_STAND_IDLE);
        object->anim().SetSpecParams(ASP_THREATEN);
        object->set_state_sound(MonsterSound::eMonsterSoundThreaten);
        object->dir().face_target(object->EnemyMan.get_enemy(), 1200);
    }

    #define STATE_TIME_OUT 4000

    template <typename _Object>
    bool CStateChimera2ThreatenRoar<_Object>::check_completion()
    {
        if (time_state_started + STATE_TIME_OUT < Device.dwTimeGlobal)
            return true;

        return false;
    }
}