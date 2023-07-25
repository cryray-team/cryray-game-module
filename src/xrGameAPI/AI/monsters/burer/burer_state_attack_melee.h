#pragma once

#include "../state.h"
#include "../states/monster_state_attack.h"

namespace BURER_ATTACK_MELEE
{
    template <typename _Object>
    class CStateBurerAttackMelee : public CStateMonsterAttack<_Object>
    {
        typedef CStateMonsterAttack<_Object> inherited;

    public:
        CStateBurerAttackMelee(_Object* obj);
        virtual bool check_start_conditions();
        virtual bool check_completion();
    };
}

namespace BURER2_ATTACK_MELEE
{
    template <typename _Object>
    class CStateBurer2AttackMelee : public CStateMonsterAttack<_Object>
    {
        typedef CStateMonsterAttack<_Object> inherited;

    public:
        CStateBurer2AttackMelee(_Object* obj);
        virtual bool check_start_conditions();
        virtual bool check_completion();
    };
}

#include "burer_state_attack_melee_inline.h"
