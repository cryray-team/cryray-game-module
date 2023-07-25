#pragma once
#include "bloodsucker_vampire_execute.h"
#include "../states/state_hide_from_point.h"
#include "bloodsucker_vampire_approach.h"
#include "bloodsucker_vampire_hide.h"

namespace BLOODSUCKER_VAMPIRE
{
    template <typename _Object>
    CStateBloodsuckerVampire<_Object>::CStateBloodsuckerVampire(_Object* obj) : inherited(obj)
    {
        add_state(eStateVampire_ApproachEnemy, xr_new<CStateBloodsuckerVampireApproach<_Object>>(obj));
        add_state(eStateVampire_Execute, xr_new<CStateBloodsuckerVampireExecute<_Object>>(obj));
        add_state(eStateVampire_RunAway, xr_new<CStateMonsterHideFromPoint<_Object>>(obj));
        add_state(eStateVampire_Hide, xr_new<CStateBloodsuckerVampireHide<_Object>>(obj));
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::reinit()
    {
        inherited::reinit();
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::initialize()
    {
        inherited::initialize();
        object->set_visibility_state(CAI_Bloodsucker::partial_visibility);

        enemy = object->EnemyMan.get_enemy();

        object->sound().play(CAI_Bloodsucker::eVampireStartHunt);
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::reselect_state()
    {
        u32 state_id = u32(-1);

        // check if we can start execute
        if (prev_substate == eStateVampire_ApproachEnemy)
        {
            if (get_state(eStateVampire_Execute)->check_start_conditions())
                state_id = eStateVampire_Execute;
        }

        // check if we executed
        if (prev_substate == eStateVampire_Execute)
            state_id = eStateVampire_Hide;

        // check if reach time in vampire state is out - then hide
        if (prev_substate == eStateVampire_ApproachEnemy)
            state_id = eStateVampire_Hide;

        // check if we hiding - then hide again
        if (prev_substate == eStateVampire_Hide)
            state_id = eStateVampire_Hide;

        // else just
        if (state_id == u32(-1))
            state_id = eStateVampire_ApproachEnemy;

        select_state(state_id);
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::check_force_state()
    {
        // check if we can start execute
        if (prev_substate == eStateVampire_ApproachEnemy)
        {
            if (get_state(eStateVampire_Execute)->check_start_conditions())
                current_substate = u32(-1);
        }
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::finalize()
    {
        inherited::finalize();

        object->set_visibility_state(CAI_Bloodsucker::full_visibility);
        CAI_Bloodsucker::m_time_last_vampire = Device.dwTimeGlobal;
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::critical_finalize()
    {
        inherited::critical_finalize();

        object->set_visibility_state(CAI_Bloodsucker::full_visibility);
        CAI_Bloodsucker::m_time_last_vampire = Device.dwTimeGlobal;
    }

    template <typename _Object>
    bool CStateBloodsuckerVampire<_Object>::check_start_conditions()
    {
        if (!object->WantVampire())
            return false;
        if (object->berserk_always)
            return false;

        // является ли враг актером
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();
        if (!dynamic_cast<CActor const*>(enemy))
            return false;
        if (!object->EnemyMan.see_enemy_now())
            return false;
        if (object->CControlledActor::is_controlling())
            return false;

        const CActor* actor = dynamic_cast<const CActor*>(enemy);
        VERIFY(actor);
        if (actor->input_external_handler_installed())
            return false;

        if (CAI_Bloodsucker::m_time_last_vampire + object->m_vampire_min_delay > Device.dwTimeGlobal)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateBloodsuckerVampire<_Object>::check_completion()
    {
        // если убежал
        if ((current_substate == eStateVampire_Hide) && get_state_current()->check_completion())
            return true;

        // если враг изменился
        if (enemy != object->EnemyMan.get_enemy())
            return true;

        // если актера уже контролит другой кровосос
        if ((current_substate != eStateVampire_Execute) && object->CControlledActor::is_controlling())
            return true;

        return false;
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStateVampire_RunAway)
        {
            SStateHideFromPoint data;
            data.point = object->EnemyMan.get_enemy_position();
            data.accelerated = true;
            data.braking = false;
            data.accel_type = eAT_Aggressive;
            data.distance = RUN_AWAY_DISTANCE;
            data.action.action = ACT_RUN;
            data.action.sound_type = MonsterSound::eMonsterSoundAggressive;
            data.action.sound_delay = object->db().m_dwAttackSndDelay;
            data.action.time_out = 15000;

            state->fill_data_with(&data, sizeof(SStateHideFromPoint));

            return;
        }
    }

    template <typename _Object>
    void CStateBloodsuckerVampire<_Object>::remove_links(CObject* object)
    {
        if (enemy == object)
            enemy = 0;
    }
}

namespace BLOODSUCKER3_VAMPIRE
{
	template <typename _Object>
	CStateBloodsucker3Vampire<_Object>::CStateBloodsucker3Vampire(_Object* obj) : inherited(obj)
	{
		add_state(eStateVampire_ApproachEnemy, xr_new<CStateBloodsucker3VampireApproach<_Object>>(obj));
		add_state(eStateVampire_Execute, xr_new<CStateBloodsucker3VampireExecute<_Object>>(obj));
		add_state(eStateVampire_RunAway, xr_new<CStateMonsterHideFromPoint<_Object>>(obj));
		add_state(eStateVampire_Hide, xr_new<CStateBloodsucker3VampireHide<_Object>>(obj));
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::reinit()
	{
		inherited::reinit();
	}
	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::initialize()
	{
		inherited::initialize();
		object->set_visibility_state(CAI_Bloodsucker3::partial_visibility);

		enemy = object->EnemyMan.get_enemy();

		object->sound().play(CAI_Bloodsucker3::eVampireStartHunt);
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::reselect_state()
	{
		u32 state_id = u32(-1);

		// check if we can start execute
		if (prev_substate == eStateVampire_ApproachEnemy)
		{
			if (get_state(eStateVampire_Execute)->check_start_conditions())
				state_id = eStateVampire_Execute;
		}

		// check if we executed
		if (prev_substate == eStateVampire_Execute)
			state_id = eStateVampire_Hide;

		// check if reach time in vampire state is out - then hide
		if (prev_substate == eStateVampire_ApproachEnemy)
			state_id = eStateVampire_Hide;

		// check if we hiding - then hide again
		if (prev_substate == eStateVampire_Hide)
			state_id = eStateVampire_Hide;

		// else just
		if (state_id == u32(-1))
			state_id = eStateVampire_ApproachEnemy;

		select_state(state_id);
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::check_force_state()
	{
		// check if we can start execute
		if (prev_substate == eStateVampire_ApproachEnemy)
		{
			if (get_state(eStateVampire_Execute)->check_start_conditions())
				current_substate = u32(-1);
		}
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::finalize()
	{
		inherited::finalize();

		object->set_visibility_state(CAI_Bloodsucker3::full_visibility);
		CAI_Bloodsucker3::m_time_last_vampire = Device.dwTimeGlobal;
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::critical_finalize()
	{
		inherited::critical_finalize();

		object->set_visibility_state(CAI_Bloodsucker3::full_visibility);
		CAI_Bloodsucker3::m_time_last_vampire = Device.dwTimeGlobal;
	}

	template <typename _Object>
	bool CStateBloodsucker3Vampire<_Object>::check_start_conditions()
	{
		if (!object->WantVampire())
			return false;
		if (object->berserk_always)
			return false;

		// является ли враг актером
		const CEntityAlive* enemy = object->EnemyMan.get_enemy();
		if (!dynamic_cast<CActor const*>(enemy))
			return false;
		if (!object->EnemyMan.see_enemy_now())
			return false;
		if (object->CControlledActor::is_controlling())
			return false;

		const CActor* actor = dynamic_cast<const CActor*>(enemy);
		VERIFY(actor);
		if (actor->input_external_handler_installed())
			return false;

		if (CAI_Bloodsucker::m_time_last_vampire + object->m_vampire_min_delay > Device.dwTimeGlobal)
			return false;

		return true;
	}

	template <typename _Object>
	bool CStateBloodsucker3Vampire<_Object>::check_completion()
	{
		// если убежал
		if ((current_substate == eStateVampire_Hide) && get_state_current()->check_completion())
			return true;

		// если враг изменился
		if (enemy != object->EnemyMan.get_enemy())
			return true;

		// если актера уже контролит другой кровосос
		if ((current_substate != eStateVampire_Execute) && object->CControlledActor::is_controlling())
			return true;

		return false;
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::setup_substates()
	{
		state_ptr state = get_state_current();

		if (current_substate == eStateVampire_RunAway)
		{
			SStateHideFromPoint data;
			data.point = object->EnemyMan.get_enemy_position();
			data.accelerated = true;
			data.braking = false;
			data.accel_type = eAT_Aggressive;
			data.distance = RUN_AWAY_DISTANCE;
			data.action.action = ACT_RUN;
			data.action.sound_type = MonsterSound::eMonsterSoundAggressive;
			data.action.sound_delay = object->db().m_dwAttackSndDelay;
			data.action.time_out = 15000;

			state->fill_data_with(&data, sizeof(SStateHideFromPoint));

			return;
		}
	}

	template <typename _Object>
	void CStateBloodsucker3Vampire<_Object>::remove_links(CObject* object)
	{
		if (enemy == object)
			enemy = 0;
	}
}