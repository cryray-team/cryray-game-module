#pragma once
#include "Include/KinematicsAnimated.h"
#include "../xrEngine/CameraBase.h"
#include "actor.h"
#include "HudManager.h"
#include "inventory.h"
#include "CustomOutfit.h"

namespace BLOODSUCKER_EXECUTE
{
    #define VAMPIRE_TIME_HOLD 4000
    #define VAMPIRE_HIT_IMPULSE 40.f
    #define VAMPIRE_MIN_DIST 0.5f
    #define VAMPIRE_MAX_DIST 1.f

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::initialize()
    {
        inherited::initialize();

        object->CControlledActor::install();

        look_head();

        m_action = eActionPrepare;
        time_vampire_started = 0;

        object->set_visibility_state(CAI_Bloodsucker::full_visibility);

        object->m_hits_before_vampire = 0;
        object->m_sufficient_hits_before_vampire_random = -1 + (rand() % 3);

        HUD().SetRenderable(false);
        NET_Packet P;
        Actor()->u_EventGen(P, GEG_PLAYER_WEAPON_HIDE_STATE, Actor()->ID());
        P.w_u16(INV_STATE_BLOCK_ALL);
        P.w_u8(u8(true));
        Actor()->u_EventSend(P);

        Actor()->set_inventory_disabled(true);

        m_effector_activated = false;
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::execute()
    {
        if (!object->CControlledActor::is_turning() && !m_effector_activated)
        {
            object->ActivateVampireEffector();
            m_effector_activated = true;
        }

        look_head();

        switch (m_action)
        {
        case eActionPrepare:
            execute_vampire_prepare();
            m_action = eActionContinue;
            break;

        case eActionContinue: execute_vampire_continue(); break;

        case eActionFire:
            execute_vampire_hit();
            m_action = eActionWaitTripleEnd;
            break;

        case eActionWaitTripleEnd:
            if (!object->com_man().ta_is_active())
            {
                m_action = eActionCompleted;
            }

        case eActionCompleted: break;
        }

        object->dir().face_target(object->EnemyMan.get_enemy());

        Fvector const enemy_to_self = object->EnemyMan.get_enemy()->Position() - object->Position();
        float const dist_to_enemy = magnitude(enemy_to_self);
        float const vampire_dist = object->get_vampire_distance();

        if (angle_between_vectors(object->Direction(), enemy_to_self) < deg2rad(20.f) && dist_to_enemy > vampire_dist)
        {
            object->set_action(ACT_RUN);
            object->anim().accel_activate(eAT_Aggressive);
            object->anim().accel_set_braking(false);

            u32 const target_vertex = object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
            Fvector const target_pos = ai().level_graph().vertex_position(target_vertex);

            object->path().set_target_point(target_pos, target_vertex);
            object->path().set_rebuild_time(100);
            object->path().set_use_covers(false);
            object->path().set_distance_to_end(vampire_dist);
        }
        else
        {
            object->set_action(ACT_STAND_IDLE);
        }
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::show_hud()
    {
        HUD().SetRenderable(true);
        NET_Packet P;

        Actor()->u_EventGen(P, GEG_PLAYER_WEAPON_HIDE_STATE, Actor()->ID());
        P.w_u16(INV_STATE_BLOCK_ALL);
        P.w_u8(u8(false));
        Actor()->u_EventSend(P);
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::cleanup()
    {
        Actor()->set_inventory_disabled(false);

        if (object->com_man().ta_is_active())
            object->com_man().ta_deactivate();

        if (object->CControlledActor::is_controlling())
            object->CControlledActor::release();

        show_hud();
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::finalize()
    {
        inherited::finalize();
        cleanup();
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::critical_finalize()
    {
        inherited::critical_finalize();
        cleanup();
    }

    template <typename _Object>
    bool CStateBloodsuckerVampireExecute<_Object>::check_start_conditions()
    {
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();

        // проверить дистанцию
        // 	float dist		= object->MeleeChecker.distance_to_enemy	(enemy);
        // 	if ((dist > VAMPIRE_MAX_DIST) || (dist < VAMPIRE_MIN_DIST))	return false;

        if (!object->done_enough_hits_before_vampire())
            return false;

        u32 const vertex_id = ai().level_graph().check_position_in_direction(
            object->ai_location().level_vertex_id(), object->Position(), enemy->Position());
        if (!ai().level_graph().valid_vertex_id(vertex_id))
            return false;

        if (!object->MeleeChecker.can_start_melee(enemy))
            return false;

        // проверить направление на врага
        if (!object->control().direction().is_face_target(enemy, PI_DIV_2))
            return false;

        if (!object->WantVampire())
            return false;

        // является ли враг актером
        if (!smart_cast<CActor const*>(enemy))
            return false;

        if (object->CControlledActor::is_controlling())
            return false;

        const CActor* actor = smart_cast<const CActor*>(enemy);

        VERIFY(actor);

        if (actor->input_external_handler_installed())
            return false;

        PIItem item_from_oslot = actor->inventory().ItemFromSlot(OUTFIT_SLOT);
        CCustomOutfit* outfit = smart_cast<CCustomOutfit*>(item_from_oslot);
        const bool outfitBlocksVampire = outfit && outfit->block_bloodsucker;

        if (outfitBlocksVampire)
            return false;

        return true;
    }

    template <typename _Object>
    bool CStateBloodsuckerVampireExecute<_Object>::check_completion()
    {
        return (m_action == eActionCompleted);
    }

    //////////////////////////////////////////////////////////////////////////

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::execute_vampire_prepare()
    {
        object->com_man().ta_activate(object->anim_triple_vampire);
        time_vampire_started = Device.dwTimeGlobal;

        object->sound().play(CAI_Bloodsucker::eVampireGrasp);
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::execute_vampire_continue()
    {
        const CEntityAlive* enemy = object->EnemyMan.get_enemy();

        // if (object->Position().distance_to(Actor()->Position()) > 2.f) {
        if (!object->MeleeChecker.can_start_melee(enemy))
        {
            object->com_man().ta_deactivate();
            m_action = eActionCompleted;
            return;
        }

        object->sound().play(CAI_Bloodsucker::eVampireSucking);

        // проверить на грави удар
        if (time_vampire_started + VAMPIRE_TIME_HOLD < Device.dwTimeGlobal)
        {
            m_action = eActionFire;
        }
    }

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::execute_vampire_hit()
    {
        object->com_man().ta_pointbreak();
        object->sound().play(CAI_Bloodsucker::eVampireHit);
        object->SatisfyVampire();
    }

    //////////////////////////////////////////////////////////////////////////

    template <typename _Object>
    void CStateBloodsuckerVampireExecute<_Object>::look_head()
    {
        IKinematics* pK = smart_cast<IKinematics*>(object->Visual());
        Fmatrix bone_transform;
        bone_transform = pK->LL_GetTransform(pK->LL_BoneID("bip01_head"));

        Fmatrix global_transform;
        global_transform.mul_43(object->XFORM(), bone_transform);

        object->CControlledActor::look_point(global_transform.c);
    }
}

namespace BLOODSUCKER3_EXECUTE
{
	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::initialize()
	{
		inherited::initialize();

		this->object->CControlledActor::install();

		look_head();

		m_action = eActionPrepare;
		time_vampire_started = 0;

		this->object->m_hits_before_vampire = 0;
		this->object->m_sufficient_hits_before_vampire_random = -1 + (rand() % 3);

		HUD().SetRenderable(false);
		NET_Packet P;
		Actor()->u_EventGen(P, GEG_PLAYER_WEAPON_HIDE_STATE, Actor()->ID());
		P.w_u16(INV_STATE_BLOCK_ALL);
		P.w_u8(u8(true));
		Actor()->u_EventSend(P);

		Actor()->set_inventory_disabled(true);

		m_effector_activated = false;
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::execute()
	{
		if (!this->object->CControlledActor::is_turning() && !m_effector_activated)
		{
			this->object->ActivateVampireEffector();
			m_effector_activated = true;
		}

		look_head();

		switch (m_action)
		{
		case eActionPrepare:
			execute_vampire_prepare();
			m_action = eActionContinue;
			break;

		case eActionContinue: execute_vampire_continue(); break;

		case eActionFire:
			execute_vampire_hit();
			m_action = eActionWaitTripleEnd;
			break;

		case eActionWaitTripleEnd:
			if (!this->object->com_man().ta_is_active())
			{
				m_action = eActionCompleted;
			}

		case eActionCompleted: break;
		}

		this->object->dir().face_target(this->object->EnemyMan.get_enemy());

		Fvector const enemy_to_self = this->object->EnemyMan.get_enemy()->Position() - this->object->Position();
		float const dist_to_enemy = magnitude(enemy_to_self);
		float const vampire_dist = this->object->get_vampire_distance();

		if (angle_between_vectors(this->object->Direction(), enemy_to_self) < deg2rad(20.f) && dist_to_enemy > vampire_dist)
		{
			this->object->set_action(ACT_RUN);
			this->object->anim().accel_activate(eAT_Aggressive);
			this->object->anim().accel_set_braking(false);

			u32 const target_vertex = this->object->EnemyMan.get_enemy()->ai_location().level_vertex_id();
			Fvector const target_pos = ai().level_graph().vertex_position(target_vertex);

			this->object->path().set_target_point(target_pos, target_vertex);
			this->object->path().set_rebuild_time(100);
			this->object->path().set_use_covers(false);
			this->object->path().set_distance_to_end(vampire_dist);
		}
		else
		{
			this->object->set_action(ACT_STAND_IDLE);
		}
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::show_hud()
	{
		HUD().SetRenderable(true);
		NET_Packet P;

		Actor()->u_EventGen(P, GEG_PLAYER_WEAPON_HIDE_STATE, Actor()->ID());
		P.w_u16(INV_STATE_BLOCK_ALL);
		P.w_u8(u8(false));
		Actor()->u_EventSend(P);
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::cleanup()
	{
		Actor()->set_inventory_disabled(false);

		if (this->object->com_man().ta_is_active())
			this->object->com_man().ta_deactivate();

		if (this->object->CControlledActor::is_controlling())
			this->object->CControlledActor::release();

		show_hud();
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::finalize()
	{
		inherited::finalize();
		cleanup();
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::critical_finalize()
	{
		inherited::critical_finalize();
		cleanup();
	}

	template <typename _Object>
	bool CStateBloodsucker3VampireExecute<_Object>::check_start_conditions()
	{
		const CEntityAlive* enemy = this->object->EnemyMan.get_enemy();

		// проверить дистанцию
		// 	float dist		= object->MeleeChecker.distance_to_enemy	(enemy);
		// 	if ((dist > VAMPIRE_MAX_DIST) || (dist < VAMPIRE_MIN_DIST))	return false;

		if (!this->object->done_enough_hits_before_vampire())
			return false;

		u32 const vertex_id = ai().level_graph().check_position_in_direction(
			this->object->ai_location().level_vertex_id(), this->object->Position(), enemy->Position());
		if (!ai().level_graph().valid_vertex_id(vertex_id))
			return false;

		if (!this->object->MeleeChecker.can_start_melee(enemy))
			return false;

		// проверить направление на врага
		if (!this->object->control().direction().is_face_target(enemy, PI_DIV_2))
			return false;

		if (!this->object->WantVampire())
			return false;

		// является ли враг актером
		if (!smart_cast<CActor const*>(enemy))
			return false;

		if (this->object->CControlledActor::is_controlling())
			return false;

		const CActor* actor = smart_cast<const CActor*>(enemy);

		VERIFY(actor);

		if (actor->input_external_handler_installed())
			return false;

		return true;
	}

	template <typename _Object>
	bool CStateBloodsucker3VampireExecute<_Object>::check_completion()
	{
		return (m_action == eActionCompleted);
	}
	//////////////////////////////////////////////////////////////////////////

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::execute_vampire_prepare()
	{
		this->object->com_man().ta_activate(this->object->anim_triple_vampire);
		time_vampire_started = Device.dwTimeGlobal;

		this->object->sound().play(CAI_Bloodsucker3::eVampireGrasp);
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::execute_vampire_continue()
	{
		const CEntityAlive* enemy = this->object->EnemyMan.get_enemy();

		if (!this->object->MeleeChecker.can_start_melee(enemy))
		{
			this->object->com_man().ta_deactivate();
			m_action = eActionCompleted;
			return;
		}

		this->object->sound().play(CAI_Bloodsucker3::eVampireSucking);

		// проверить на грави удар
		if (time_vampire_started + VAMPIRE_TIME_HOLD < Device.dwTimeGlobal)
		{
			m_action = eActionFire;
		}
	}

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::execute_vampire_hit()
	{
		this->object->com_man().ta_pointbreak();
		this->object->sound().play(CAI_Bloodsucker3::eVampireHit);
		this->object->SatisfyVampire();
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename _Object>
	void CStateBloodsucker3VampireExecute<_Object>::look_head()
	{
		IKinematics* pK = smart_cast<IKinematics*>(this->object->Visual());
		Fmatrix bone_transform = pK->LL_GetTransform(pK->LL_BoneID("bip01_head"));

		Fmatrix global_transform;
		global_transform.mul_43(this->object->XFORM(), bone_transform);

		this->object->CControlledActor::look_point(global_transform.c);
	}
}