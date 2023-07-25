#pragma once

namespace CHIMERA_THREATEN_STEAL 
{
    template <typename _Object>
    void CStateChimeraThreatenSteal<_Object>::initialize()
    {
        inherited::initialize();

        data.action.action = ACT_STEAL;

        data.accelerated = true;
        data.braking = false;
        data.accel_type = eAT_Calm;

        data.completion_dist = 2.f;
        data.action.sound_type = MonsterSound::eMonsterSoundIdle;
        data.action.sound_delay = object->db().m_dwIdleSndDelay;
    }

    template <typename _Object>
    void CStateChimeraThreatenSteal<_Object>::finalize()
    {
        inherited::finalize();
    }

    template <typename _Object>
    void CStateChimeraThreatenSteal<_Object>::execute()
    {
        data.point = object->EnemyMan.get_enemy_position();
        data.vertex = object->EnemyMan.get_enemy_vertex();
        data.time_to_rebuild = object->get_attack_rebuild_time();

        inherited::execute();
    }

    #define MIN_DISTANCE_TO_ENEMY 8.f

    template <typename _Object>
    bool CStateChimeraThreatenSteal<_Object>::check_completion()
    {
        if (inherited::check_completion())
            return true;

        float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
        if (dist_to_enemy < MIN_DISTANCE_TO_ENEMY)
            return true;

        return false;
    }

    template <typename _Object>
    bool CStateChimeraThreatenSteal<_Object>::check_start_conditions()
    {
        float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
        if (dist_to_enemy > MIN_DISTANCE_TO_ENEMY)
            return true;
        return false;
    }

    #undef MIN_DISTANCE_TO_ENEMY
}

namespace CHIMERA2_THREATEN_STEAL
{
    template <typename _Object>
    void CStateChimera2ThreatenSteal<_Object>::initialize()
    {
        inherited::initialize();

        object->SetUpperState(false);

        data.action.action = ACT_STEAL;

        data.accelerated = true;
        data.braking = false;
        data.accel_type = eAT_Calm;

        data.completion_dist = 2.f;
        data.action.sound_type = MonsterSound::eMonsterSoundIdle;
        data.action.sound_delay = object->db().m_dwIdleSndDelay;
    }

    template <typename _Object>
    void CStateChimera2ThreatenSteal<_Object>::finalize()
    {
        inherited::finalize();
        object->SetUpperState();
    }

    template <typename _Object>
    void CStateChimera2ThreatenSteal<_Object>::execute()
    {
        data.point = object->EnemyMan.get_enemy_position();
        data.vertex = object->EnemyMan.get_enemy_vertex();
        data.time_to_rebuild = object->get_attack_rebuild_time();

        inherited::execute();
    }

    #define MIN_DISTANCE_TO_ENEMY 8.f

    template <typename _Object>
    bool CStateChimera2ThreatenSteal<_Object>::check_completion()
    {
        if (inherited::check_completion())
            return true;

        float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
        if (dist_to_enemy < MIN_DISTANCE_TO_ENEMY)
            return true;

        return false;
    }

    template <typename _Object>
    bool CStateChimera2ThreatenSteal<_Object>::check_start_conditions()
    {
        float dist_to_enemy = object->EnemyMan.get_enemy_position().distance_to(object->Position());
        if (dist_to_enemy > MIN_DISTANCE_TO_ENEMY)
            return true;

        return false;
    }

    #undef MIN_DISTANCE_TO_ENEMY
}