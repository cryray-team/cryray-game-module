#include "stdafx.h"
#include "Entity.h"
#include "../xrGame/Entity.h"

using XRay::Entity;

Entity::Entity(IntPtr InNativeObject) : PhysicalGameObject(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CEntity, InNativeObject);
}

void Entity::ChangeTeam(int team, int squad, int group)
{
	pNativeObject->ChangeTeam(team, squad, group);
}

float Entity::Health::get()
{
	return pNativeObject->GetfHealth();
}

void Entity::Health::set(float value)
{
	pNativeObject->SetfHealth(value);
}

float Entity::Morale::get()
{
	return pNativeObject->m_fMorale;
}

void Entity::Morale::set(float value)
{
	pNativeObject->m_fMorale = value;
}

float Entity::MaxHealth::get()
{
	return pNativeObject->GetMaxHealth();
}

void Entity::MaxHealth::set(float value)
{
	pNativeObject->SetMaxHealth(value);
}

bool Entity::IsAlive::get()
{
	return pNativeObject->g_Alive();
}

bool Entity::IsJumping::get()
{
	CEntity::SEntityState EntityState;
	if (pNativeObject->g_State(EntityState))
	{
		return EntityState.bJump;
	}
	return false;
}

bool Entity::IsSprinting::get()
{
	CEntity::SEntityState EntityState;
	if (pNativeObject->g_State(EntityState))
	{
		return EntityState.bSprint;
	}
	return false;
}

bool Entity::IsCrouching::get()
{
	CEntity::SEntityState EntityState;
	if (pNativeObject->g_State(EntityState))
	{
		return EntityState.bCrouch;
	}
	return false;
}

bool Entity::IsFalling::get()
{
	CEntity::SEntityState EntityState;
	if (pNativeObject->g_State(EntityState))
	{
		return EntityState.bFall;
	}
	return false;
}


int Entity::Team::get()
{
	return pNativeObject->GetfHealth();
}

void Entity::Team::set(int value)
{
	ChangeTeam(value, pNativeObject->g_Squad(), pNativeObject->g_Group());
}

int Entity::Squad::get()
{
	return pNativeObject->g_Squad();
}

void Entity::Squad::set(int value)
{
	ChangeTeam(pNativeObject->g_Team(), value, pNativeObject->g_Group());
}

int Entity::Group::get()
{
	return pNativeObject->g_Group();
}

void Entity::Group::set(int value)
{
	ChangeTeam(pNativeObject->g_Team(), pNativeObject->g_Squad(), value);
}