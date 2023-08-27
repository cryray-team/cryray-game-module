#include "stdafx.h"
#include "bloodsucker_state_manager.h"
#include "bloodsucker.h"
#include "bloodsucker2.h"
#include "bloodsucker3.h"

#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"
#include "../control_path_builder_base.h"

#include "../states/monster_state_rest.h"
#include "../states/monster_state_attack.h"
#include "../states/monster_state_panic.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_hear_int_sound.h"
#include "../states/monster_state_hear_danger_sound.h"
#include "../states/monster_state_hitted.h"

#include "bloodsucker_vampire.h"
#include "bloodsucker_predator.h"
#include "bloodsucker_state_capture_jump.h"
#include "bloodsucker_attack_state.h"

CStateManagerBloodsucker::CStateManagerBloodsucker(CAI_Bloodsucker* monster) : inherited(monster)
{
    add_state(eStateRest, xr_new<CStateMonsterRest<CAI_Bloodsucker>>(monster));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CAI_Bloodsucker>>(monster));
    add_state(eStateAttack, xr_new<CStateMonsterAttack<CAI_Bloodsucker>>(monster));
    add_state(eStateEat, xr_new<CStateMonsterEat<CAI_Bloodsucker>>(monster));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CAI_Bloodsucker>>(monster));
    add_state(eStateHearDangerousSound, xr_new<CStateMonsterHearDangerousSound<CAI_Bloodsucker>>(monster));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CAI_Bloodsucker>>(monster));
    add_state(eStateVampire_Execute, xr_new<BLOODSUCKER_EXECUTE::CStateBloodsuckerVampireExecute<CAI_Bloodsucker>>(monster));
}

CStateManagerBloodsucker2::CStateManagerBloodsucker2(CAI_Bloodsucker2* monster) : inherited(monster)
{
    add_state(eStateCustom, xr_new<BLOODSUCKER2_STATE_JUMP::CStateCaptureJumpBloodsucker2<CAI_Bloodsucker2>>(monster));
    add_state(eStateRest, xr_new<CStateMonsterRest<CAI_Bloodsucker2>>(monster));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CAI_Bloodsucker2>>(monster));
    add_state(eStateAttack, xr_new<BLOODSUCKER2_ATTACK::CBloodsucker2StateAttack<CAI_Bloodsucker2>>(monster));
    add_state(eStateEat, xr_new<CStateMonsterEat<CAI_Bloodsucker2>>(monster));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CAI_Bloodsucker2>>(monster));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CAI_Bloodsucker2>>(monster));
}

CStateManagerBloodsucker3::CStateManagerBloodsucker3(CAI_Bloodsucker3* monster) : inherited(monster)
{
    add_state(eStateRest, xr_new<CStateMonsterRest<CAI_Bloodsucker3>>(monster));
    add_state(eStatePanic, xr_new<CStateMonsterPanic<CAI_Bloodsucker3>>(monster));
    add_state(eStateAttack, xr_new<BLOODSUCKER3_ATTACK::CBloodsucker3StateAttack<CAI_Bloodsucker3>>(monster));
    add_state(eStateEat, xr_new<CStateMonsterEat<CAI_Bloodsucker3>>(monster));
    add_state(eStateHearInterestingSound, xr_new<CStateMonsterHearInterestingSound<CAI_Bloodsucker3>>(monster));
    add_state(eStateHitted, xr_new<CStateMonsterHitted<CAI_Bloodsucker3>>(monster));
    add_state(eStateVampire_Execute, xr_new<BLOODSUCKER3_EXECUTE::CStateBloodsucker3VampireExecute<CAI_Bloodsucker3>>(monster));
}

CStateManagerBloodsucker::~CStateManagerBloodsucker() {}
CStateManagerBloodsucker2::~CStateManagerBloodsucker2() {}
CStateManagerBloodsucker3::~CStateManagerBloodsucker3() {}

void CStateManagerBloodsucker::drag_object()
{
    CEntityAlive* const ph_obj = object->m_cob;
    if (!ph_obj)
    {
        return;
    }

    IKinematics* const kinematics = smart_cast<IKinematics*>(ph_obj->Visual());
    if (!kinematics)
    {
        return;
    }

    CMonsterSquad* const squad = monster_squad().get_squad(object);
    if (squad)
    {
        squad->lock_corpse(ph_obj);
    }

    {
        const u16 drag_bone = kinematics->LL_BoneID(object->m_str_cel);
        object->character_physics_support()->movement()->PHCaptureObject(ph_obj, drag_bone);
    }

    IPHCapture* const capture = object->character_physics_support()->movement()->PHCapture();

    if (capture && !capture->Failed() && object->is_animated())
    {
        object->start_drag();
    }
}

void CStateManagerBloodsucker2::drag_object()
{
    CEntityAlive* const ph_obj = object->m_cob;
    if (!ph_obj)
    {
        return;
    }

    IKinematics* const kinematics = smart_cast<IKinematics*>(ph_obj->Visual());
    if (!kinematics)
    {
        return;
    }

    CMonsterSquad* const squad = monster_squad().get_squad(object);
    if (squad)
    {
        squad->lock_corpse(ph_obj);
    }

    {
        const u16 drag_bone = kinematics->LL_BoneID(object->m_str_cel);
        object->character_physics_support()->movement()->PHCaptureObject(ph_obj, drag_bone);
    }

    IPHCapture* const capture = object->character_physics_support()->movement()->PHCapture();

    if (capture && !capture->Failed() && object->is_animated())
    {
        object->start_drag();
    }
}

void CStateManagerBloodsucker::update() { inherited::update(); }
void CStateManagerBloodsucker2::update() { inherited::update(); }

bool CStateManagerBloodsucker::check_vampire()
{
    if (prev_substate != eStateVampire_Execute)
    {
        if (get_state(eStateVampire_Execute)->check_start_conditions())
            return true;
    }
    else
    {
        if (!get_state(eStateVampire_Execute)->check_completion())
            return true;
    }

    return false;
}

bool CStateManagerBloodsucker3::check_vampire()
{
    if (prev_substate != eStateVampire_Execute)
    {
        if (get_state(eStateVampire_Execute)->check_start_conditions())
            return true;
    }
    else
    {
        if (!get_state(eStateVampire_Execute)->check_completion())
            return true;
    }

    return false;
}

void CStateManagerBloodsucker::execute()
{
    u32 state_id = u32(-1);

    const CEntityAlive* enemy = object->EnemyMan.get_enemy();

    if (enemy)
    {
        if (check_vampire())
        {
            state_id = eStateVampire_Execute;
        }
        else
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong: state_id = eStatePanic; break;
            case eWeak: state_id = eStateAttack; break;
            }
        }
    }
    else if (object->HitMemory.is_hit())
    {
        state_id = eStateHitted;
    }
    else if (object->hear_interesting_sound)
    {
        state_id = eStateHearInterestingSound;
    }
    else if (object->hear_dangerous_sound)
    {
        state_id = eStateHearDangerousSound;
    }
    else
    {
        if (can_eat())
            state_id = eStateEat;
        else
            state_id = eStateRest;
    }

    select_state(state_id);

    // выполнить текущее состояние
    get_state_current()->execute();

    prev_substate = current_substate;
}

void CStateManagerBloodsucker2::execute()
{
    u32 state_id = u32(-1);

    const CEntityAlive* enemy = object->EnemyMan.get_enemy();

    if (!object->is_drag_anim_jump() && !object->is_animated())
    {
        if (enemy)
        {
            {
                switch (object->EnemyMan.get_danger_type())
                {
                case eStrong: state_id = eStatePanic; break;
                case eWeak: state_id = eStateAttack; break;
                }
            }
        }
        else if (object->HitMemory.is_hit())
        {
            state_id = eStateHitted;
        }
        else if (object->hear_interesting_sound)
        {
            state_id = eStateHearInterestingSound;
        }
        else
        {
            if (can_eat())
                state_id = eStateEat;
            else
                state_id = eStateRest;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Additional
        ///////////////////////////////////////////////////////////////////////////////

        // check if start interesting sound state
        // 		if ( (prev_substate != eStateHearInterestingSound) && (state_id == eStateHearInterestingSound) )
        // 		{
        // 			object->start_invisible_predator();
        // 		}
        // 		else
        // 		// check if stop interesting sound state
        // 		if ( (prev_substate == eStateHearInterestingSound) && (state_id != eStateHearInterestingSound) )
        // 		{
        // 			object->stop_invisible_predator();
        // 		}

        select_state(state_id);

        // выполнить текущее состояние
        get_state_current()->execute();

        prev_substate = current_substate;
    }
    else
    {
        state_id = eStateCustom;

        if (object->is_drag_anim_jump())
        {
            select_state(state_id);

            // выполнить текущее состояние
            get_state_current()->execute();

            prev_substate = current_substate;

            drag_object();
        }
    }
}

void CStateManagerBloodsucker3::execute()
{
    u32 state_id = u32(-1);

    const CEntityAlive* enemy = object->EnemyMan.get_enemy();

    if (enemy)
    {
        if (check_vampire())
        {
            state_id = eStateVampire_Execute;
        }
        else
        {
            switch (object->EnemyMan.get_danger_type())
            {
            case eStrong: state_id = eStatePanic; break;
            case eWeak: state_id = eStateAttack; break;
            }
        }
    }
    else if (object->HitMemory.is_hit())
    {
        state_id = eStateHitted;
    }
    else if (object->hear_dangerous_sound || object->hear_interesting_sound)
    {
        state_id = eStateHearInterestingSound;
    }
    else
    {
        if (can_eat())
            state_id = eStateEat;
        else
            state_id = eStateRest;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Additional
    ///////////////////////////////////////////////////////////////////////////////

    // check if start interesting sound state
    if ((prev_substate != eStateHearInterestingSound) && (state_id == eStateHearInterestingSound))
    {
        object->predator_start();
    }
    else
        // check if stop interesting sound state
        if ((prev_substate == eStateHearInterestingSound) && (state_id != eStateHearInterestingSound))
        {
            object->predator_stop();
        }
    ///////////////////////////////////////////////////////////////////////////////

    select_state(state_id);

    if ((current_substate == eStateAttack) && (current_substate != prev_substate))
    {
        object->predator_stop();
        object->start_threaten = true;
    }

    // выполнить текущее состояние
    get_state_current()->execute();

    prev_substate = current_substate;
}
