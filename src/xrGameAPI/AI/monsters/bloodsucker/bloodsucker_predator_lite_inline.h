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

namespace BLOODSUCKER_PREDATOR_LITE
{
    template <typename _Object>
    CStateBloodsuckerPredatorLite<_Object>::CStateBloodsuckerPredatorLite(_Object* obj) : inherited(obj)
    {
        add_state(eStatePredator_Camp, xr_new<CStateMonsterCustomAction<_Object>>(obj));
        add_state(eStatePredator_MoveToCover, xr_new<CStateMonsterMoveToPointEx<_Object>>(obj));
        add_state(eStatePredator_LookOpenPlace, xr_new<CStateMonsterLookToPoint<_Object>>(obj));
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::reinit()
    {
        inherited::reinit();
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::initialize()
    {
        inherited::initialize();

        object->predator_start();

        m_target_node = u32(-1);
        m_freezed = false;
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            if (enemy_see_me())
                select_state(eStatePredator_MoveToCover);
            else
                select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_MoveToCover)
        {
            if (enemy_see_me())
            {
                select_state(eStatePredator_MoveToCover);
                object->set_berserk();
            }
            else
                select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_LookOpenPlace)
        {
            select_state(eStatePredator_Camp);
            return;
        }

        if (prev_substate == eStatePredator_Camp)
        {
            select_state(eStatePredator_MoveToCover);
            return;
        }

        select_state(eStatePredator_MoveToCover);
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::finalize()
    {
        inherited::finalize();
        object->predator_stop();
        if (m_freezed)
            object->predator_unfreeze();

        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        object->predator_stop();
        if (m_freezed)
            object->predator_unfreeze();

        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsuckerPredatorLite<_Object>::check_completion()
    {
        if (object->EnemyMan.see_enemy_now() &&
            (object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
        {
            object->set_berserk();
            return true;
        }
        if (object->conditions().health() > 0.9f)
            return true;

        return false;
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStatePredator_Camp)
        {
            object->predator_freeze();
            m_freezed = true;
        }
        else
        {
            object->predator_unfreeze();
            m_freezed = false;
        }

        if (current_substate == eStatePredator_MoveToCover)
        {
            select_camp_point();

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
    void CStateBloodsuckerPredatorLite<_Object>::check_force_state()
    {
        if (prev_substate == eStatePredator_Camp)
        {
            if (object->HitMemory.get_last_hit_time() > time_state_started)
            {
                if (object->EnemyMan.get_enemy() &&
                    (object->EnemyMan.get_enemy()->Position().distance_to(object->Position()) < 10.f))
                {
                    object->set_berserk();
                }
                else
                    current_substate = u32(-1);
            }
        }
    }

    template <typename _Object>
    void CStateBloodsuckerPredatorLite<_Object>::select_camp_point()
    {
        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);

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
            const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 20.f, 30.f);
            if (point)
            {
                m_target_node = point->level_vertex_id();
            }
        }

        if (m_target_node == u32(-1))
            m_target_node = object->ai_location().level_vertex_id();

        monster_squad().get_squad(object)->lock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsuckerPredatorLite<_Object>::enemy_see_me()
    {
        return object->EnemyMan.enemy_see_me_now();
    }
}

namespace BLOODSUCKER2_PREDATOR_LITE
{
    template <typename _Object>
    CStateBloodsucker2PredatorLite<_Object>::CStateBloodsucker2PredatorLite(_Object* obj) : inherited(obj)
    {
        add_state(eStatePredator_Camp, xr_new CStateMonsterCustomAction<_Object>(obj));
        add_state(eStatePredator_MoveToCover, xr_new CStateMonsterMoveToPointEx<_Object>(obj));
        add_state(eStatePredator_LookOpenPlace, xr_new CStateMonsterLookToPoint<_Object>(obj));
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::reinit()
    {
        inherited::reinit();
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::initialize()
    {
        inherited::initialize();

        object->predator_start();

        m_target_node = u32(-1);
        m_freezed = false;
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::reselect_state()
    {
        if (prev_substate == u32(-1))
        {
            if (enemy_see_me())
                select_state(eStatePredator_MoveToCover);
            else
                select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_MoveToCover)
        {
            if (enemy_see_me())
            {
                select_state(eStatePredator_MoveToCover);
                object->set_berserk();
            }
            else
                select_state(eStatePredator_LookOpenPlace);
            return;
        }

        if (prev_substate == eStatePredator_LookOpenPlace)
        {
            select_state(eStatePredator_Camp);
            return;
        }

        if (prev_substate == eStatePredator_Camp)
        {
            select_state(eStatePredator_MoveToCover);
            return;
        }

        select_state(eStatePredator_MoveToCover);
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::finalize()
    {
        inherited::finalize();
        object->predator_stop();
        if (m_freezed)
            object->predator_unfreeze();

        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        object->predator_stop();
        if (m_freezed)
            object->predator_unfreeze();

        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsucker2PredatorLite<_Object>::check_completion()
    {
        if (object->EnemyMan.see_enemy_now() &&
            (object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
        {
            object->set_berserk();
            return true;
        }
        if (object->conditions().health() > 0.9f)
            return true;

        return false;
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::setup_substates()
    {
        state_ptr state = get_state_current();

        if (current_substate == eStatePredator_Camp)
        {
            object->predator_freeze();
            m_freezed = true;
        }
        else
        {
            object->predator_unfreeze();
            m_freezed = false;
        }

        if (current_substate == eStatePredator_MoveToCover)
        {
            select_camp_point();

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
    void CStateBloodsucker2PredatorLite<_Object>::check_force_state()
    {
        if (prev_substate == eStatePredator_Camp)
        {
            if (object->HitMemory.get_last_hit_time() > time_state_started)
            {
                if (object->EnemyMan.get_enemy() &&
                    (object->EnemyMan.get_enemy()->Position().distance_to(object->Position()) < 10.f))
                {
                    object->set_berserk();
                }
                else
                    current_substate = u32(-1);
            }
        }
    }

    template <typename _Object>
    void CStateBloodsucker2PredatorLite<_Object>::select_camp_point()
    {
        if (m_target_node != u32(-1))
            monster_squad().get_squad(object)->unlock_cover(m_target_node);

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
            const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 20.f, 30.f);
            if (point)
            {
                m_target_node = point->level_vertex_id();
            }
        }

        if (m_target_node == u32(-1))
            m_target_node = object->ai_location().level_vertex_id();

        monster_squad().get_squad(object)->lock_cover(m_target_node);
    }

    template <typename _Object>
    bool CStateBloodsucker2PredatorLite<_Object>::enemy_see_me()
    {
        return object->EnemyMan.enemy_see_me_now();
    }
}

namespace BLOODSUCKER3_PREDATOR_LITE
{
	template <typename _Object>
	CStateBloodsucker3PredatorLite<_Object>::CStateBloodsucker3PredatorLite(_Object* obj) : inherited(obj)
	{
		add_state(eStatePredator_Camp, xr_new<CStateMonsterCustomAction<_Object>>(obj));
		add_state(eStatePredator_MoveToCover, xr_new<CStateMonsterMoveToPointEx<_Object>>(obj));
		add_state(eStatePredator_LookOpenPlace, xr_new<CStateMonsterLookToPoint<_Object>>(obj));
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::reinit()
	{
		inherited::reinit();
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::initialize()
	{
		inherited::initialize();

		object->predator_start();

		m_target_node = u32(-1);
		m_freezed = false;
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::reselect_state()
	{
		if (prev_substate == u32(-1))
		{
			if (enemy_see_me())
				select_state(eStatePredator_MoveToCover);
			else
				select_state(eStatePredator_LookOpenPlace);
			return;
		}

		if (prev_substate == eStatePredator_MoveToCover)
		{
			if (enemy_see_me())
			{
				select_state(eStatePredator_MoveToCover);
				object->set_berserk();
			}
			else
				select_state(eStatePredator_LookOpenPlace);
			return;
		}

		if (prev_substate == eStatePredator_LookOpenPlace)
		{
			select_state(eStatePredator_Camp);
			return;
		}

		if (prev_substate == eStatePredator_Camp)
		{
			select_state(eStatePredator_MoveToCover);
			return;
		}

		select_state(eStatePredator_MoveToCover);
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::finalize()
	{
		inherited::finalize();
		object->predator_stop();
		if (m_freezed)
			object->predator_unfreeze();

		if (m_target_node != u32(-1))
			monster_squad().get_squad(object)->unlock_cover(m_target_node);
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::critical_finalize()
	{
		inherited::critical_finalize();
		object->predator_stop();
		if (m_freezed)
			object->predator_unfreeze();

		if (m_target_node != u32(-1))
			monster_squad().get_squad(object)->unlock_cover(m_target_node);
	}

	template <typename _Object>
	bool CStateBloodsucker3PredatorLite<_Object>::check_completion()
	{
		if (object->EnemyMan.see_enemy_now() &&
			(object->Position().distance_to(object->EnemyMan.get_enemy()->Position()) < 4.f))
		{
			object->set_berserk();
			return true;
		}
		if (object->conditions().health() > 0.9f)
			return true;

		return false;
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::setup_substates()
	{
		state_ptr state = get_state_current();

		if (current_substate == eStatePredator_Camp)
		{
			object->predator_freeze();
			m_freezed = true;
		}
		else
		{
			object->predator_unfreeze();
			m_freezed = false;
		}

		if (current_substate == eStatePredator_MoveToCover)
		{
			select_camp_point();

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
	void CStateBloodsucker3PredatorLite<_Object>::check_force_state()
	{
		if (prev_substate == eStatePredator_Camp)
		{
			if (object->HitMemory.get_last_hit_time() > time_state_started)
			{
				if (object->EnemyMan.get_enemy() &&
					(object->EnemyMan.get_enemy()->Position().distance_to(object->Position()) < 10.f))
				{
					object->set_berserk();
				}
				else
					current_substate = u32(-1);
			}
		}
	}

	template <typename _Object>
	void CStateBloodsucker3PredatorLite<_Object>::select_camp_point()
	{
		if (m_target_node != u32(-1))
			monster_squad().get_squad(object)->unlock_cover(m_target_node);

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
			const CCoverPoint* point = object->CoverMan->find_cover(object->Position(), 20.f, 30.f);
			if (point)
			{
				m_target_node = point->level_vertex_id();
			}
		}

		if (m_target_node == u32(-1))
			m_target_node = object->ai_location().level_vertex_id();

		monster_squad().get_squad(object)->lock_cover(m_target_node);
	}

	template <typename _Object>
	bool CStateBloodsucker3PredatorLite<_Object>::enemy_see_me()
	{
		return object->EnemyMan.enemy_see_me_now();
	}
}