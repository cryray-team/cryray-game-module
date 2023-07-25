#pragma once

#include "../states/state_move_to_point.h"
#include "../states/state_look_point.h"
#include "../states/state_custom_action.h"
#include "cover_point.h"
#include "../monster_cover_manager.h"
#include "../monster_home.h"

#include "actor.h"
#include "actor_memory.h"
#include "visual_memory_manager.h"

namespace BLOODSUCKER_PREDATOR
{
    template <typename _Object>
    CStateBloodsuckerPredator<_Object>::CStateBloodsuckerPredator(_Object* obj) : inherited(obj)
    {
        add_state(eStatePredator_MoveToCover, xr_new<CStateMonsterMoveToPointEx<_Object>>(obj));
        add_state(eStatePredator_LookOpenPlace, xr_new<CStateMonsterLookToPoint<_Object>>(obj));
        add_state(eStatePredator_Camp, xr_new<CStateMonsterCustomAction<_Object>>(obj));
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::reinit()
    {
        inherited::reinit();
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::initialize()
    {
        inherited::initialize();

        object->predator_start();

        select_camp_point();
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            select_state(eStatePredator_MoveToCover);
            return;
        }

        if (prev_substate == eStatePredator_MoveToCover)
        {
            select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_LookOpenPlace)
        {
            select_state(eStatePredator_Camp);
            return;
        }

        select_state(eStatePredator_Camp);
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::finalize()
    {
        inherited::finalize();

        object->predator_stop();
        object->predator_unfreeze();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->unlock_cover(m_target_node);
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::critical_finalize()
    {
        inherited::critical_finalize();

        object->predator_stop();
        object->predator_unfreeze();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->unlock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsuckerPredator<_Object>::check_start_conditions()
    {
        if (Actor()->memory().visual().visible_now(object))
            return false;
        return true;
    }

    template <typename _Object>
    bool CStateBloodsuckerPredator<_Object>::check_completion()
    {
        if (object->HitMemory.get_last_hit_time() > time_state_started)
            return true;
        if (object->EnemyMan.get_enemy() && object->EnemyMan.see_enemy_now() &&
            (object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
            return true;

        return false;
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStatePredator_Camp)
        {
            object->predator_freeze();
            m_time_start_camp = time();
        }
        else
        {
            object->predator_unfreeze();
        }

        if (current_substate == eStatePredator_MoveToCover)
        {
            SStateDataMoveToPointEx data;

            data.vertex = m_target_node;
            data.point = ai().level_graph().vertex_position(data.vertex);
            data.action.action = ACT_RUN;
            data.action.time_out = 0; // do not use time out
            data.completion_dist = 0.f; // get exactly to the point
            data.time_to_rebuild = 0; // do not rebuild
            data.accelerated = true;
            data.braking = true;
            data.accel_type = eAT_Aggressive;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;

            state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
            return;
        }

        if (current_substate == eStatePredator_LookOpenPlace)
        {
            SStateDataLookToPoint data;

            Fvector dir;
            object->CoverMan->less_cover_direction(dir);

            data.point.mad(object->Position(), dir, 10.f);
            data.action.action = ACT_STAND_IDLE;
            data.action.time_out = 2000;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;
            data.face_delay = 0;

            state->fill_data_with(&data, sizeof(SStateDataLookToPoint));
            return;
        }

        if (current_substate == eStatePredator_Camp)
        {
            SStateDataAction data;

            data.action = ACT_STAND_IDLE;
            data.time_out = 0; // do not use time out
            data.sound_type = MonsterSound::eMonsterSoundIdle;
            data.sound_delay = object->db().m_dwIdleSndDelay;

            state->fill_data_with(&data, sizeof(SStateDataAction));

            return;
        }
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::check_force_state()
    {
        if ((current_substate == eStatePredator_Camp) && (m_time_start_camp + TIME_TO_RESELECT_CAMP < time()))
        {
            if (current_substate != u32(-1))
                get_state_current()->critical_finalize();

            prev_substate = u32(-1);
            current_substate = u32(-1);

            CMonsterSquad* squad = monster_squad().get_squad(object);
            squad->unlock_cover(m_target_node);

            select_camp_point();
        }
    }

    template <typename _Object>
    void CStateBloodsuckerPredator<_Object>::select_camp_point()
    {
        m_target_node = u32(-1);
        if (object->Home->has_home())
        {
            m_target_node = object->Home->get_place_in_cover();
            if (m_target_node == u32(-1))
            {
                m_target_node = object->Home->get_place();
            }
        }

        if (m_target_node == u32(-1))
        {
            const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 10.f, 30.f);
            if (point)
            {
                m_target_node = point->level_vertex_id();
            }
        }

        if (m_target_node == u32(-1))
            m_target_node = object->ai_location().level_vertex_id();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->lock_cover(m_target_node);
    }
}

namespace BLOODSUCKER2_PREDATOR
{
    template <typename _Object>
    CStateBloodsucker2Predator<_Object>::CStateBloodsucker2Predator(_Object* obj) : inherited(obj)
    {
        add_state(eStatePredator_MoveToCover, xr_new CStateMonsterMoveToPointEx<_Object>(obj));
        add_state(eStatePredator_LookOpenPlace, xr_new CStateMonsterLookToPoint<_Object>(obj));
        add_state(eStatePredator_Camp, xr_new CStateMonsterCustomAction<_Object>(obj));
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::reinit()
    {
        inherited::reinit();
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::initialize()
    {
        inherited::initialize();

        object->predator_start();

        select_camp_point();
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            select_state(eStatePredator_MoveToCover);
            return;
        }

        if (prev_substate == eStatePredator_MoveToCover)
        {
            select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_LookOpenPlace)
        {
            select_state(eStatePredator_Camp);
            return;
        }

        select_state(eStatePredator_Camp);
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::finalize()
    {
        inherited::finalize();

        object->predator_stop();
        object->predator_unfreeze();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->unlock_cover(m_target_node);
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::critical_finalize()
    {
        inherited::critical_finalize();

        object->predator_stop();
        object->predator_unfreeze();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->unlock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsucker2Predator<_Object>::check_start_conditions()
    {
        if (Actor()->memory().visual().visible_now(object))
            return false;
        return true;
    }

    template <typename _Object>
    bool CStateBloodsucker2Predator<_Object>::check_completion()
    {
        if (object->HitMemory.get_last_hit_time() > time_state_started)
            return true;
        if (object->EnemyMan.get_enemy() && object->EnemyMan.see_enemy_now() &&
            (object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
            return true;

        return false;
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStatePredator_Camp)
        {
            object->predator_freeze();
            m_time_start_camp = time();
        }
        else
        {
            object->predator_unfreeze();
        }

        if (current_substate == eStatePredator_MoveToCover)
        {
            SStateDataMoveToPointEx data;

            data.vertex = m_target_node;
            data.point = ai().level_graph().vertex_position(data.vertex);
            data.action.action = ACT_RUN;
            data.action.time_out = 0; // do not use time out
            data.completion_dist = 0.f; // get exactly to the point
            data.time_to_rebuild = 0; // do not rebuild
            data.accelerated = true;
            data.braking = true;
            data.accel_type = eAT_Aggressive;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;

            state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
            return;
        }

        if (current_substate == eStatePredator_LookOpenPlace)
        {
            SStateDataLookToPoint data;

            Fvector dir;
            object->CoverMan->less_cover_direction(dir);

            data.point.mad(object->Position(), dir, 10.f);
            data.action.action = ACT_STAND_IDLE;
            data.action.time_out = 2000;
            data.action.sound_type = MonsterSound::eMonsterSoundIdle;
            data.action.sound_delay = object->db().m_dwIdleSndDelay;
            data.face_delay = 0;

            state->fill_data_with(&data, sizeof(SStateDataLookToPoint));
            return;
        }

        if (current_substate == eStatePredator_Camp)
        {
            SStateDataAction data;

            data.action = ACT_STAND_IDLE;
            data.time_out = 0; // do not use time out
            data.sound_type = MonsterSound::eMonsterSoundIdle;
            data.sound_delay = object->db().m_dwIdleSndDelay;

            state->fill_data_with(&data, sizeof(SStateDataAction));

            return;
        }
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::check_force_state()
    {
        if ((current_substate == eStatePredator_Camp) && (m_time_start_camp + TIME_TO_RESELECT_CAMP < time()))
        {
            if (current_substate != u32(-1))
                get_state_current()->critical_finalize();

            prev_substate = u32(-1);
            current_substate = u32(-1);

            CMonsterSquad* squad = monster_squad().get_squad(object);
            squad->unlock_cover(m_target_node);

            select_camp_point();
        }
    }

    template <typename _Object>
    void CStateBloodsucker2Predator<_Object>::select_camp_point()
    {
        m_target_node = u32(-1);
        if (object->Home->has_home())
        {
            m_target_node = object->Home->get_place_in_cover();
            if (m_target_node == u32(-1))
            {
                m_target_node = object->Home->get_place();
            }
        }

        if (m_target_node == u32(-1))
        {
            const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 10.f, 30.f);
            if (point)
            {
                m_target_node = point->level_vertex_id();
            }
        }

        if (m_target_node == u32(-1))
            m_target_node = object->ai_location().level_vertex_id();

        CMonsterSquad* squad = monster_squad().get_squad(object);
        squad->lock_cover(m_target_node);
    }
}

namespace BLOODSUCKER3_PREDATOR
{
	template <typename _Object>
	CStateBloodsucker3Predator<_Object>::CStateBloodsucker3Predator(_Object* obj) : inherited(obj)
	{
		add_state(eStatePredator_MoveToCover, xr_new<CStateMonsterMoveToPointEx<_Object>>(obj));
		add_state(eStatePredator_LookOpenPlace, xr_new<CStateMonsterLookToPoint<_Object>>(obj));
		add_state(eStatePredator_Camp, xr_new<CStateMonsterCustomAction<_Object>>(obj));
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::reinit()
	{
		inherited::reinit();
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::initialize()
	{
		inherited::initialize();

		object->predator_start();

		select_camp_point();
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::reselect_state()
	{
		if (prev_substate == u32(-1))
		{
			select_state(eStatePredator_MoveToCover);
			return;
		}

		if (prev_substate == eStatePredator_MoveToCover)
		{
			select_state(eStatePredator_LookOpenPlace);
			return;
		}

		if (prev_substate == eStatePredator_LookOpenPlace)
		{
			select_state(eStatePredator_Camp);
			return;
		}

		select_state(eStatePredator_Camp);
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::finalize()
	{
		inherited::finalize();

		object->predator_stop();
		object->predator_unfreeze();

		CMonsterSquad* squad = monster_squad().get_squad(object);
		squad->unlock_cover(m_target_node);
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::critical_finalize()
	{
		inherited::critical_finalize();

		object->predator_stop();
		object->predator_unfreeze();

		CMonsterSquad* squad = monster_squad().get_squad(object);
		squad->unlock_cover(m_target_node);
	}

	template <typename _Object>
	bool CStateBloodsucker3Predator<_Object>::check_start_conditions()
	{
		if (Actor()->memory().visual().visible_now(object))
			return false;
		return true;
	}

	template <typename _Object>
	bool CStateBloodsucker3Predator<_Object>::check_completion()
	{
		if (object->HitMemory.get_last_hit_time() > time_state_started)
			return true;
		if (object->EnemyMan.get_enemy() && object->EnemyMan.see_enemy_now() &&
			(object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
			return true;

		return false;
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::setup_substates()
	{
		state_ptr state = get_state_current();

		if (current_substate == eStatePredator_Camp)
		{
			object->predator_freeze();
			m_time_start_camp = time();
		}
		else
		{
			object->predator_unfreeze();
		}

		if (current_substate == eStatePredator_MoveToCover)
		{
			SStateDataMoveToPointEx data;

			data.vertex = m_target_node;
			data.point = ai().level_graph().vertex_position(data.vertex);
			data.action.action = ACT_RUN;
			data.action.time_out = 0; // do not use time out
			data.completion_dist = 0.f; // get exactly to the point
			data.time_to_rebuild = 0; // do not rebuild
			data.accelerated = true;
			data.braking = true;
			data.accel_type = eAT_Aggressive;
			data.action.sound_type = MonsterSound::eMonsterSoundIdle;
			data.action.sound_delay = object->db().m_dwIdleSndDelay;

			state->fill_data_with(&data, sizeof(SStateDataMoveToPointEx));
			return;
		}

		if (current_substate == eStatePredator_LookOpenPlace)
		{
			SStateDataLookToPoint data;

			Fvector dir;
			object->CoverMan->less_cover_direction(dir);

			data.point.mad(object->Position(), dir, 10.f);
			data.action.action = ACT_STAND_IDLE;
			data.action.time_out = 2000;
			data.action.sound_type = MonsterSound::eMonsterSoundIdle;
			data.action.sound_delay = object->db().m_dwIdleSndDelay;
			data.face_delay = 0;

			state->fill_data_with(&data, sizeof(SStateDataLookToPoint));
			return;
		}

		if (current_substate == eStatePredator_Camp)
		{
			SStateDataAction data;

			data.action = ACT_STAND_IDLE;
			data.time_out = 0; // do not use time out
			data.sound_type = MonsterSound::eMonsterSoundIdle;
			data.sound_delay = object->db().m_dwIdleSndDelay;

			state->fill_data_with(&data, sizeof(SStateDataAction));

			return;
		}
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::check_force_state()
	{
		if ((current_substate == eStatePredator_Camp) && (m_time_start_camp + TIME_TO_RESELECT_CAMP < time()))
		{
			if (current_substate != u32(-1))
				get_state_current()->critical_finalize();

			prev_substate = u32(-1);
			current_substate = u32(-1);

			CMonsterSquad* squad = monster_squad().get_squad(object);
			squad->unlock_cover(m_target_node);

			select_camp_point();
		}
	}

	template <typename _Object>
	void CStateBloodsucker3Predator<_Object>::select_camp_point()
	{
		m_target_node = u32(-1);
		if (object->Home->has_home())
		{
			m_target_node = object->Home->get_place_in_cover();
			if (m_target_node == u32(-1))
			{
				m_target_node = object->Home->get_place();
			}
		}

		if (m_target_node == u32(-1))
		{
			const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 10.f, 30.f);
			if (point)
			{
				m_target_node = point->level_vertex_id();
			}
		}

		if (m_target_node == u32(-1))
			m_target_node = object->ai_location().level_vertex_id();

		CMonsterSquad* squad = monster_squad().get_squad(object);
		squad->lock_cover(m_target_node);
	}
}