#pragma once

namespace BLOODSUCKER_VAMPIRE_APPROACH
{
    template <typename _Object>
    CStateBloodsuckerVampireApproach<_Object>::CStateBloodsuckerVampireApproach(_Object* obj) : inherited(obj)
    {
    }

    template <typename _Object>
    CStateBloodsuckerVampireApproach<_Object>::~CStateBloodsuckerVampireApproach()
    {
    }

    template <typename _Object>
    void CStateBloodsuckerVampireApproach<_Object>::initialize()
    {
        inherited::initialize();
        object->path().prepare_builder();
    }

    template <typename _Object>
    void CStateBloodsuckerVampireApproach<_Object>::execute()
    {
        // установка параметров функциональных блоков
        object->set_action(ACT_RUN);
        object->anim().accel_activate(eAT_Aggressive);
        object->anim().accel_set_braking(false);

        u32 const target_vertex = object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
        Fvector const target_pos = ai().level_graph().vertex_position(target_vertex);

        object->path().set_target_point(target_pos, target_vertex);
        object->path().set_rebuild_time(object->get_attack_rebuild_time());
        object->path().set_use_covers(false);
        object->path().set_distance_to_end(0.1f);
        object->set_state_sound(MonsterSound::eMonsterSoundAggressive);
    }
}

namespace BLOODSUCKER3_VAMPIRE_APPROACH
{
	template <typename _Object>
	CStateBloodsucker3VampireApproach<_Object>::CStateBloodsucker3VampireApproach(_Object* obj) : inherited(obj)
	{
	}
	template <typename _Object>
	CStateBloodsucker3VampireApproach<_Object>::~CStateBloodsucker3VampireApproach()
	{
	}
	template <typename _Object>
	void CStateBloodsucker3VampireApproach<_Object>::initialize()
	{
		inherited::initialize();
		object->path().prepare_builder();
	}

	template <typename _Object>
	void CStateBloodsucker3VampireApproach<_Object>::execute()
	{
		// установка параметров функциональных блоков
		object->set_action(ACT_RUN);
		object->anim().accel_activate(eAT_Aggressive);
		object->anim().accel_set_braking(false);

		u32 const target_vertex = object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
		Fvector const target_pos = ai().level_graph().vertex_position(target_vertex);

		object->path().set_target_point(target_pos, target_vertex);
		object->path().set_rebuild_time(object->get_attack_rebuild_time());
		object->path().set_use_covers(false);
		object->path().set_distance_to_end(0.1f);
		object->set_state_sound(MonsterSound::eMonsterSoundAggressive);
	}
}