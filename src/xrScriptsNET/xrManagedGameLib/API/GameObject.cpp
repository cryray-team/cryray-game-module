#include "stdafx.h"
#include "GameObject.h"

#include "../xrGame/GameObject.h"
#include "../xrGame/ai_object_location.h"

using NETGameObject = XRay::GameObject;

NETGameObject::GameObject(IntPtr InNativeObject) : Object(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CGameObject, InNativeObject);
}

u32 NETGameObject::LevelVertexID::get()
{
	return pNativeObject->ai_location().level_vertex_id();
}

u32 NETGameObject::GameVertexID::get()
{
	return pNativeObject->ai_location().game_vertex_id();
}

u32 NETGameObject::AlifeStoryId::get()
{
	return pNativeObject->story_id();
}

bool NETGameObject::IsAIObstacle::get()
{
	return pNativeObject->is_ai_obstacle();
}

u32 NETGameObject::SpawnTimeFrame::get()
{
	return pNativeObject->spawn_time();
}

XRay::GameVertex  NETGameObject::GameVertexPtr::get()
{
	const GameGraph::CVertex* vertex = pNativeObject->ai_location().game_vertex();
	return GameVertex(IntPtr((void*)vertex));
}
