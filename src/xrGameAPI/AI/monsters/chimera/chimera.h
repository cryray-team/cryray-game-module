#pragma once
#include "../BaseMonster/base_monster.h"
#include "script_export_space.h"

class CChimera : public CBaseMonster
{
public:
    CChimera();
    virtual ~CChimera();

    virtual void Load(LPCSTR section);
    virtual void reinit();
    virtual void UpdateCL();

    virtual void HitEntityInJump(const CEntity* pEntity);
    virtual void jump(Fvector const& position, float factor);

private:
    virtual char* get_monster_class_name() { return "chimera"; }
    virtual EAction CustomVelocityIndex2Action(u32 velocity_index);

    typedef CBaseMonster inherited;

    SVelocityParam m_velocity_rotate;
    SVelocityParam m_velocity_jump_start;

    struct attack_params
    {
        float attack_radius;
        TTime prepare_jump_timeout;
        TTime attack_jump_timeout;
        TTime stealth_timeout;
        float force_attack_distance;
        u32 num_attack_jumps;
        u32 num_prepare_jumps;
    };

    attack_params m_attack_params;

public:
    attack_params const& get_attack_params() const { return m_attack_params; }

    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CChimera)
#undef script_type_list
#define script_type_list save_type_list(CChimera)

class CChimera2 : public CBaseMonster
{
    typedef CBaseMonster inherited;

    bool b_upper_state;

    SVelocityParam m_fsVelocityWalkUpper;
    SVelocityParam m_fsVelocityJumpGround;
    SVelocityParam m_fsVelocityRunAttack;

public:
    CChimera2();
    virtual ~CChimera2();

    virtual void Load(LPCSTR section);
    virtual void reinit();
    virtual void UpdateCL();

    virtual void SetTurnAnimation(bool turn_left);
    virtual void CheckSpecParams(u32 spec_params);
    virtual EAction CustomVelocityIndex2Action(u32 velocity_index);
    virtual void TranslateActionToPathParams();
    virtual void HitEntityInJump(const CEntity* pEntity);

    virtual char* get_monster_class_name() { return "chimera2"; }

    IC void SetUpperState(bool state = true) { b_upper_state = state; }

    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CChimera2)
#undef script_type_list
#define script_type_list save_type_list(CChimera2)
