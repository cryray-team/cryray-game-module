#pragma once

#include "../BaseMonster/base_monster.h"
#include "../controlled_entity.h"

#include "script_export_space.h"

class CAI_Boar : public CBaseMonster, public CControlledEntity<CAI_Boar>
{
    typedef CBaseMonster inherited;
    typedef CControlledEntity<CAI_Boar> CControlled;

public:
    CAI_Boar();
    virtual ~CAI_Boar();

    virtual void Load(LPCSTR section);
    virtual BOOL net_Spawn(CSE_Abstract* DC);
    virtual void reinit();

    virtual void UpdateCL();

    virtual bool CanExecRotationJump() { return true; }

    // look at enemy
    static void BoneCallback(CBoneInstance* B);

    float _velocity;
    float _cur_delta, _target_delta;
    bool look_at_enemy;

    virtual bool ability_can_drag() { return true; }

    virtual char* get_monster_class_name() { return "boar"; }

    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CAI_Boar)
#undef script_type_list
#define script_type_list save_type_list(CAI_Boar)

class CStateManagerBoar2;

class CAI_CBoar2 : public CBaseMonster
{
    typedef CBaseMonster inherited;

public:
    CAI_CBoar2();
    virtual ~CAI_CBoar2();

    virtual void Load(LPCSTR section);
    virtual void CheckSpecParams(u32 spec_params);

    virtual void reinit();

    virtual char* get_monster_class_name() { return "boar2"; }

    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CAI_CBoar2)
#undef script_type_list
#define script_type_list save_type_list(CAI_CBoar2)
