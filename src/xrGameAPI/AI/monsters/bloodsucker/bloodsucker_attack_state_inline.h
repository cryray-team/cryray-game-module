#pragma once
#include "../states/state_move_to_point.h"

namespace BLOODSUCKER_ATTACK
{
    template <typename _Object>
    CBloodsuckerStateAttack<_Object>::CBloodsuckerStateAttack(_Object* obj) : inherited_attack(obj)
    {
        add_state(eStateAttack_Hide, xr_new<CStateMonsterBackstubEnemy2<_Object>>(obj));
        add_state(eStateVampire_Execute, xr_new<BLOODSUCKER_EXECUTE::CStateBloodsuckerVampireExecute<_Object>>(obj));
    }

    template <typename _Object>
    CBloodsuckerStateAttack<_Object>::~CBloodsuckerStateAttack()
    {
    }

    template <typename _Object>
    void CBloodsuckerStateAttack<_Object>::initialize()
    {
        inherited::initialize();
        m_time_stop_invis = 0;
        m_last_health = object->conditions().GetHealth();
    }

    template <typename _Object>
    void CBloodsuckerStateAttack<_Object>::finalize()
    {
        inherited::finalize();
        object->start_invisible_predator();
    }

    template <typename _Object>
    void CBloodsuckerStateAttack<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        object->start_invisible_predator();
    }

    template <typename _Object>
    void CBloodsuckerStateAttack<_Object>::execute()
    {
        if (check_home_point())
            select_state(eStateAttack_MoveToHomePoint);
        else if (check_vampire())
            select_state(eStateVampire_Execute);
        else if (check_steal_state())
            select_state(eStateAttack_Steal);
        else if (check_camp_state())
            select_state(eStateAttackCamp);
        else if (check_find_enemy_state())
            select_state(eStateAttack_FindEnemy);
        else if (check_hiding())
            select_state(eStateAttack_Hide);
        else if (check_run_attack_state())
            select_state(eStateAttack_RunAttack);
        else
        {
            // определить тип атаки
            bool b_melee = false;

            if (prev_substate == eStateAttack_Melee)
            {
                if (!get_state_current()->check_completion())
                {
                    b_melee = true;
                }
            }
            else if (get_state(eStateAttack_Melee)->check_start_conditions())
            {
                b_melee = true;
            }

            if (!b_melee && (prev_substate == eStateAttack_Melee))
            {
                select_state(eStateAttack_Hide);
            }
            else
                // установить целевое состояние
                if (b_melee)
                {
                    // check if enemy is behind me for a long time
                    // [TODO] make specific state and replace run_away state (to avoid ratation jumps)
                    // if (check_behinder())
                    //	select_state(eStateAttack_RunAway);
                    // else
                    select_state(eStateAttack_Melee);
                }
                else
                {
                    select_state(eStateAttack_Run);
                }
        }

        // clear behinder var if not melee state selected
        if (current_substate != eStateAttack_Melee)
        {
            m_time_start_check_behinder = 0;
        }
        else
        {
            object->clear_runaway_invisible();
        }

        get_state_current()->execute();
        prev_substate = current_substate;

        // Notify squad
        CMonsterSquad* squad = monster_squad().get_squad(object);
        if (squad)
        {
            SMemberGoal goal;

            goal.type = MG_AttackEnemy;
            goal.entity = const_cast<CEntityAlive*>(object->EnemyMan.get_enemy());

            squad->UpdateGoal(object, goal);
        }
    }

    template <typename _Object>
    bool CBloodsuckerStateAttack<_Object>::check_vampire()
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

    template <typename _Object>
    bool CBloodsuckerStateAttack<_Object>::check_hiding()
    {
        const bool health_step_lost =
            object->conditions().GetHealth() < m_last_health - bloodsuckerAttackState::bloodsucker::loose_health_diff;

        if (health_step_lost)
        {
            object->start_runaway_invisible();
            m_last_health = object->conditions().GetHealth();
            m_start_with_encircle = true;
            return true;
        }

        // if we get here before 1 sec after last critical hit:
        u32 last_critical_hit_tick = object->get_last_critical_hit_tick();
        if (last_critical_hit_tick && time() < last_critical_hit_tick + 1000)
        {
            object->clear_last_critical_hit_tick();
            m_start_with_encircle = true;
            return true;
        }

        if (current_substate == eStateAttack_Hide)
        {
            return !get_state_current()->check_completion();
        }

        m_start_with_encircle = false;
        return get_state(eStateAttack_Hide)->check_start_conditions();
    }

    template <typename _Object>
    void CBloodsuckerStateAttack<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStateAttack_Hide)
        {
            CStateMonsterBackstubEnemy2<_Object>::StateParams data;

            data.action.action = ACT_RUN;
            data.action.time_out = 0;
            data.completion_dist = 1.f; // get exactly to the point
            data.time_to_rebuild = 200;
            data.accelerated = true;
            data.braking = false;
            data.accel_type = eAT_Aggressive;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;
            data.start_with_encircle = m_start_with_encircle;

            state->fill_data_with(&data, sizeof(CStateMonsterBackstubEnemy2<_Object>::StateParams));
            return;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // CStateMonsterMoveToPointEx with path rebuild options
    //////////////////////////////////////////////////////////////////////////

    template <class _Object>
    void CStateMonsterBackstubEnemy2<_Object>::initialize()
    {
        inherited::initialize();
        object->path().prepare_builder();
        m_last_health = object->conditions().GetHealth();
        m_encircle = data.start_with_encircle;
        m_encircle_end_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::encircle_time;
        m_next_change_behaviour_tick = 0;
    }

    template <class _Object>
    void CStateMonsterBackstubEnemy2<_Object>::execute()
    {
        // on hit, change behaviour
        if (object->conditions().GetHealth() < m_last_health - bloodsuckerAttackState::bloodsucker::loose_health_diff &&
            Device.dwTimeGlobal > m_next_change_behaviour_tick)
        {
            m_next_change_behaviour_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::change_behaviour_time;
            m_last_health = object->conditions().GetHealth();
            m_encircle = !m_encircle;
            if (m_encircle)
            {
                m_encircle_end_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::encircle_time;
            }
        }

        if (Device.dwTimeGlobal > m_encircle_end_tick)
        {
            if (object->EnemyMan.enemy_see_me_now())
            {
                m_encircle = false;
            }
        }

        object->set_action(data.action.action);
        object->anim().SetSpecParams(data.action.spec_params);

        data.point = object->EnemyMan.get_enemy_position();
        data.vertex = 0;

        data.target_direction = Fvector().set(0.f, 0.f, 0.f);
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();
        VERIFY(enemy);

        const SRotation rot = enemy->Orientation();
        data.target_direction.setHP(rot.yaw, rot.pitch);

        object->path().set_target_point(data.point, data.vertex);
        object->path().set_rebuild_time(data.time_to_rebuild);
        object->path().set_distance_to_end(data.completion_dist);
        object->path().set_use_covers();
        object->path().set_cover_params(5.f, 30.f, 1.f, 30.f);

        if (m_encircle)
        {
            object->path().set_use_dest_orient(true);
            object->path().set_dest_direction(data.target_direction);
            object->path().set_try_min_time(false);
        }
        else
        {
            object->path().set_try_min_time(true);
            object->path().set_use_dest_orient(false);
        }

        if (data.accelerated)
        {
            object->anim().accel_activate(EAccelType(data.accel_type));
            object->anim().accel_set_braking(data.braking);
        }

        if (data.action.sound_type != u32(-1))
        {
            object->set_state_sound(data.action.sound_type, data.action.sound_delay == u32(-1));
        }
    }

    template <class _Object>
    bool CStateMonsterBackstubEnemy2<_Object>::check_start_conditions()
    {
        if (!object->Home->at_home(object->EnemyMan.get_enemy_position()))
        {
            return false;
        }

        float dist = object->MeleeChecker.distance_to_enemy(object->EnemyMan.get_enemy());

        return dist > object->MeleeChecker.get_min_distance();
    }

    template <class _Object>
    bool CStateMonsterBackstubEnemy2<_Object>::check_completion()
    {
        if (!object->Home->at_home(object->EnemyMan.get_enemy_position()))
        {
            return true;
        }

        const bool real_path_end = fis_zero(data.completion_dist) ?
            (data.point.distance_to_xz(object->Position()) < ai().level_graph().header().cell_size()) :
            true;

        if (object->control().path_builder().is_path_end(data.completion_dist) && real_path_end)
        {
            // in straight-mode we're done
            if (!m_encircle)
            {
                return true;
            }

            if (object->EnemyMan.see_enemy_now() && !object->EnemyMan.enemy_see_me_now())
            {
                // object->sound().play(MonsterSound::eMonsterSoundSteal);
                return true;
            }
        }

        return false;
    }
}

namespace BLOODSUCKER2_ATTACK
{
    template <typename _Object>
    CBloodsucker2StateAttack<_Object>::CBloodsucker2StateAttack(_Object* obj) : inherited_attack(obj)
    {
        add_state(eStateAttack_Hide, xr_new<CStateMonsterBackstubEnemy1<_Object>>(obj));
    }

    template <typename _Object>
    CBloodsucker2StateAttack<_Object>::~CBloodsucker2StateAttack()
    {
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::initialize()
    {
        inherited::initialize();
        m_time_stop_invis = 0;
        m_last_health = object->conditions().GetHealth();
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::finalize()
    {
        inherited::finalize();
        object->start_invisible_predator();
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        object->start_invisible_predator();
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::execute()
    {
        if (check_home_point())
        {
            select_state(eStateAttack_MoveToHomePoint);
        }
        else if (check_steal_state())
        {
            select_state(eStateAttack_Steal);
        }
        else if (check_camp_state())
        {
            select_state(eStateAttackCamp);
        }
        else if (check_find_enemy_state())
        {
            select_state(eStateAttack_FindEnemy);
        }
        else if (check_hiding())
        {
            select_state(eStateAttack_Hide);
        }
        else if (check_run_attack_state())
        {
            select_state(eStateAttack_RunAttack);
        }
        else
        {
            // определить тип атаки
            bool b_melee = false;

            if (prev_substate == eStateAttack_Melee)
            {
                if (!get_state_current()->check_completion())
                {
                    b_melee = true;
                }
            }
            else if (get_state(eStateAttack_Melee)->check_start_conditions())
            {
                b_melee = true;
            }

            if (!b_melee && (prev_substate == eStateAttack_Melee))
            {
                select_state(eStateAttack_Hide);
            }
            else
                // установить целевое состояние
                if (b_melee)
                {
                    // check if enemy is behind me for a long time
                    // [TODO] make specific state and replace run_away state (to avoid ratation jumps)
                    // if (check_behinder())
                    //	select_state(eStateAttack_RunAway);
                    // else
                    select_state(eStateAttack_Melee);
                }
                else
                {
                    select_state(eStateAttack_Run);
                }
        }

        // clear behinder var if not melee state selected
        if (current_substate != eStateAttack_Melee)
        {
            m_time_start_check_behinder = 0;
        }

        update_invisibility();

        get_state_current()->execute();
        prev_substate = current_substate;

        // Notify squad
        CMonsterSquad* squad = monster_squad().get_squad(object);
        if (squad)
        {
            SMemberGoal goal;

            goal.type = MG_AttackEnemy;
            goal.entity = const_cast<CEntityAlive*>(object->EnemyMan.get_enemy());

            squad->UpdateGoal(object, goal);
        }
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::update_invisibility()
    {
        if (object->state_invisible)
        {
            // check conditions to stop invis
            if (current_substate == eStateAttack_Melee)
            {
                object->stop_invisible_predator();
                m_time_stop_invis = Device.dwTimeGlobal;
            }
        }
        else
        {
            if (Device.dwTimeGlobal > m_time_stop_invis + object->get_invisibility_activate_delay())
            {
                // check conditions to start invis
                if (current_substate == eStateAttack_Hide)
                {
                    object->start_invisible_predator();
                }
                else if (object->EnemyMan.get_enemy()->Position().distance_to(object->Position()) >
                    bloodsuckerAttackState::bloodsucker::invisibility_dist2enemy)
                {
                    object->start_invisible_predator();
                }
            }
        }
    }

    template <typename _Object>
    bool CBloodsucker2StateAttack<_Object>::check_hiding()
    {
        const bool health_step_lost =
            object->conditions().GetHealth() < m_last_health - bloodsuckerAttackState::bloodsucker::loose_health_diff;

        if (health_step_lost)
        {
            m_last_health = object->conditions().GetHealth();
            m_start_with_encircle = true;
            return true;
        }

        // if we get here before 1 sec after last critical hit:
        u32 last_critical_hit_tick = object->get_last_critical_hit_tick();
        if (last_critical_hit_tick && time() < last_critical_hit_tick + 1000)
        {
            object->clear_last_critical_hit_tick();
            m_start_with_encircle = true;
            return true;
        }

        if (current_substate == eStateAttack_Hide)
        {
            return !get_state_current()->check_completion();
        }

        m_start_with_encircle = false;
        return get_state(eStateAttack_Hide)->check_start_conditions();
    }

    template <typename _Object>
    void CBloodsucker2StateAttack<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStateAttack_Hide)
        {
            CStateMonsterBackstubEnemy1<_Object>::StateParams data;

            data.action.action = ACT_RUN;
            data.action.time_out = 0;
            data.completion_dist = 1.f; // get exactly to the point
            data.time_to_rebuild = 200;
            data.accelerated = true;
            data.braking = false;
            data.accel_type = eAT_Aggressive;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;
            data.start_with_encircle = m_start_with_encircle;

            state->fill_data_with(&data, sizeof(CStateMonsterBackstubEnemy1<_Object>::StateParams));
            return;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // CStateMonsterMoveToPointEx with path rebuild options
    //////////////////////////////////////////////////////////////////////////

    template <class _Object>
    void CStateMonsterBackstubEnemy1<_Object>::initialize()
    {
        inherited::initialize();
        object->path().prepare_builder();
        m_last_health = object->conditions().GetHealth();
        m_encircle = data.start_with_encircle;
        m_encircle_end_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::encircle_time;
        m_next_change_behaviour_tick = 0;
    }

    template <class _Object>
    void CStateMonsterBackstubEnemy1<_Object>::execute()
    {
        // on hit, change behaviour
        if (object->conditions().GetHealth() < m_last_health - bloodsuckerAttackState::bloodsucker::loose_health_diff &&
            Device.dwTimeGlobal > m_next_change_behaviour_tick)
        {
            m_next_change_behaviour_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::change_behaviour_time;
            m_last_health = object->conditions().GetHealth();
            m_encircle = !m_encircle;
            if (m_encircle)
            {
                m_encircle_end_tick = Device.dwTimeGlobal + bloodsuckerAttackState::bloodsucker::encircle_time;
            }
        }

        if (Device.dwTimeGlobal > m_encircle_end_tick)
        {
            if (object->EnemyMan.enemy_see_me_now())
            {
                m_encircle = false;
            }
        }

        object->set_action(data.action.action);
        object->anim().SetSpecParams(data.action.spec_params);

        data.point = object->EnemyMan.get_enemy_position();
        data.vertex = 0;

        data.target_direction = Fvector().set(0.f, 0.f, 0.f);
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();
        VERIFY(enemy);

        const SRotation rot = enemy->Orientation();
        data.target_direction.setHP(rot.yaw, rot.pitch);

        object->path().set_target_point(data.point, data.vertex);
        object->path().set_rebuild_time(data.time_to_rebuild);
        object->path().set_distance_to_end(data.completion_dist);
        object->path().set_use_covers();
        object->path().set_cover_params(5.f, 30.f, 1.f, 30.f);

        if (m_encircle)
        {
            object->path().set_use_dest_orient(true);
            object->path().set_dest_direction(data.target_direction);
            object->path().set_try_min_time(false);
        }
        else
        {
            object->path().set_try_min_time(true);
            object->path().set_use_dest_orient(false);
        }

        if (data.accelerated)
        {
            object->anim().accel_activate(EAccelType(data.accel_type));
            object->anim().accel_set_braking(data.braking);
        }

        if (data.action.sound_type != u32(-1))
        {
            object->set_state_sound(data.action.sound_type, data.action.sound_delay == u32(-1));
        }
    }

    template <class _Object>
    bool CStateMonsterBackstubEnemy1<_Object>::check_start_conditions()
    {
        if (!object->Home->at_home(object->EnemyMan.get_enemy_position()))
        {
            return false;
        }

        float dist = object->MeleeChecker.distance_to_enemy(object->EnemyMan.get_enemy());

        return dist > object->MeleeChecker.get_min_distance();
    }

    template <class _Object>
    bool CStateMonsterBackstubEnemy1<_Object>::check_completion()
    {
        if (!object->Home->at_home(object->EnemyMan.get_enemy_position()))
        {
            return true;
        }

        const bool real_path_end = fis_zero(data.completion_dist) ?
            (data.point.distance_to_xz(object->Position()) < ai().level_graph().header().cell_size()) :
            true;

        if (object->control().path_builder().is_path_end(data.completion_dist) && real_path_end)
        {
            // in straight-mode we're done
            if (!m_encircle)
            {
                return true;
            }

            if (object->EnemyMan.see_enemy_now() && !object->EnemyMan.enemy_see_me_now())
            {
                // object->sound().play(MonsterSound::eMonsterSoundSteal);
                return true;
            }
        }

        return false;
    }
}

namespace BLOODSUCKER3_ATTACK
{
	template <typename _Object>
	CBloodsucker3StateAttack<_Object>::CBloodsucker3StateAttack(_Object* obj) : inherited_attack(obj)
	{
		add_state(eStateAttack_Hide, xr_new<CStateMonsterMoveToPointEx<_Object>>(obj));
        add_state(eStateVampire_Execute, xr_new<BLOODSUCKER3_EXECUTE::CStateBloodsucker3VampireExecute<_Object>>(obj));
	}

	template <typename _Object>
	CBloodsucker3StateAttack<_Object>::~CBloodsucker3StateAttack()
	{
	}

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::initialize()
	{
		inherited::initialize();
		m_time_stop_invis = 0;
	}

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::finalize()
	{
		inherited::finalize();
		object->stop_invisible_predator();
	}

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::critical_finalize()
	{
		inherited::critical_finalize();
		object->stop_invisible_predator();
	}

	#define INVIS_ACTIVATE_DELAY 3000
	#define INVIS_DIST_TO_ENEMY 5.f

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::execute()
	{
		bool selected = false;

		if (check_home_point())
		{
			select_state(eStateAttack_MoveToHomePoint);
			selected = true;
		}
		else if (check_vampire())
		{
			select_state(eStateVampire_Execute);
			selected = true;
		}
		else if (check_hiding())
		{
			select_state(eStateAttack_Hide);
			selected = true;
		}
		else if (check_steal_state())
		{
			select_state(eStateAttack_Steal);
			selected = true;
		}
		else if (check_camp_state())
		{
			select_state(eStateAttackCamp);
			selected = true;
		}
		else if (check_find_enemy_state())
		{
			select_state(eStateAttack_FindEnemy);
			selected = true;
		}
		else if (check_run_attack_state())
		{
			select_state(eStateAttack_RunAttack);
			selected = true;
		}

		if (!selected)
		{
			// определить тип атаки
			bool b_melee = false;

			if (prev_substate == eStateAttack_Melee)
			{
				if (!get_state_current()->check_completion())
				{
					b_melee = true;
				}
			}
			else if (get_state(eStateAttack_Melee)->check_start_conditions())
			{
				b_melee = true;
			}

			if (!b_melee && (prev_substate == eStateAttack_Melee))
			{
				select_state(eStateAttack_Hide);
			}
			else
				// установить целевое состояние
				if (b_melee)
				{
					// check if enemy is behind me for a long time
					// [TODO] make specific state and replace run_away state (to avoid ratation jumps)
					// if (check_behinder())
					//	select_state(eStateAttack_RunAway);
					// else
					select_state(eStateAttack_Melee);
				}
				else
					select_state(eStateAttack_Run);
		}

		// clear behinder var if not melee state selected
		if (current_substate != eStateAttack_Melee)
			m_time_start_check_behinder = 0;
		update_invisibility();

		get_state_current()->execute();
		prev_substate = current_substate;

		// Notify squad
		CMonsterSquad* squad = monster_squad().get_squad(object);
		if (squad)
		{
			SMemberGoal goal;

			goal.type = MG_AttackEnemy;
			goal.entity = const_cast<CEntityAlive*>(object->EnemyMan.get_enemy());

			squad->UpdateGoal(object, goal);
		}
		//////////////////////////////////////////////////////////////////////////
	}

	template <typename _Object>
	bool CBloodsucker3StateAttack<_Object>::check_vampire()
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

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::update_invisibility()
	{
		if (object->threaten_time() > 0)
		{
			object->stop_invisible_predator();
			return;
		}

		if (object->state_invisible)
		{
			// check conditions to stop invis
			if (current_substate == eStateAttack_Melee)
			{
				object->stop_invisible_predator();
				m_time_stop_invis = time();
			}
		}
		else
		{
			// check conditions to start invis
			if (current_substate == eStateAttack_Hide)
			{
				object->start_invisible_predator();
			}
			else if ((current_substate == eStateAttack_Run) &&
				(object->EnemyMan.get_enemy()->Position().distance_to(object->Position()) > INVIS_DIST_TO_ENEMY))
			{
				if (m_time_stop_invis + INVIS_ACTIVATE_DELAY < time())
					object->start_invisible_predator();
			}
		}
	}

	template <typename _Object>
	bool CBloodsucker3StateAttack<_Object>::check_hiding()
	{
		if (current_substate == eStateAttack_Hide)
			if (!get_state(eStateAttack_Melee)->check_start_conditions())
				if (!get_state_current()->check_completion())
				{
					return true;
				}

		return false;
	}

	template <typename _Object>
	void CBloodsucker3StateAttack<_Object>::setup_substates()
	{
		state_ptr state = get_state_current();

		if (current_substate == eStateAttack_Hide)
		{
			SStateDataMoveToPointEx data;

			Fvector target_dir = Random.randI(2) ? object->XFORM().i : Fvector().set(object->XFORM().i).invert();
			m_dir_point = Fvector().mad(object->Position(), target_dir, 2.5f);

			data.vertex = 0;
			data.point = m_dir_point;
			data.action.action = ACT_RUN;
			data.action.time_out = 1500; // do not use time out
			data.completion_dist = 1.f; // get exactly to the point
			data.time_to_rebuild = object->get_attack_rebuild_time();
			data.accelerated = true;
			data.braking = false;
			data.accel_type = eAT_Aggressive;
			data.action.sound_type = MonsterSound::eMonsterSoundIdle;
			data.action.sound_delay = object->db().m_dwIdleSndDelay;

			state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));

			return;
		}
	}
}