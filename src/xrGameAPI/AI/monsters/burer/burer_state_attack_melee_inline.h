#pragma once

namespace BURER_ATTACK_MELEE
{
    #define MIN_DIST_MELEE_ATTACK 5.f
    #define MAX_DIST_MELEE_ATTACK 9.f

    template <typename _Object>
    CStateBurerAttackMelee<_Object>::CStateBurerAttackMelee(_Object* obj) : inherited(obj)
    {
    }

    template <typename _Object>
    bool CStateBurerAttackMelee<_Object>::check_start_conditions()
    {
        float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
        if (dist > MIN_DIST_MELEE_ATTACK)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateBurerAttackMelee<_Object>::check_completion()
    {
        float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
        if (dist < MAX_DIST_MELEE_ATTACK)
            return false;

        return true;
    }
}

namespace BURER2_ATTACK_MELEE
{
    #define MIN_DIST_MELEE_ATTACK 5.f
    #define MAX_DIST_MELEE_ATTACK 9.f

    template <typename _Object>
    CStateBurer2AttackMelee<_Object>::CStateBurer2AttackMelee(_Object* obj) : inherited(obj)
    {
    }

    template <typename _Object>
    bool CStateBurer2AttackMelee<_Object>::check_start_conditions()
    {
        float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
        if (dist > MIN_DIST_MELEE_ATTACK)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateBurer2AttackMelee<_Object>::check_completion()
    {
        float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
        if (dist < MAX_DIST_MELEE_ATTACK)
            return false;

        return true;
    }
}