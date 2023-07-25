////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_stalker_impl.h
//	Created 	: 25.02.2003
//  Modified 	: 25.02.2003
//	Author		: Dmitriy Iassenev
//	Description : AI Behaviour for monster "Stalker" (IC functions implementation)
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../../level.h"
#include "../../seniority_hierarchy_holder.h"
#include "../../team_hierarchy_holder.h"
#include "../../squad_hierarchy_holder.h"
#include "../../group_hierarchy_holder.h"
#include "../../effectorshot.h"
#include "stalker_movement_manager_smart_cover.h"
#include "smart_cover_animation_selector.h"
#include "smart_cover_animation_planner.h"

IC Fvector CAI_Stalker::weapon_shot_effector_direction(const Fvector& current) const
{
    return current;
}
