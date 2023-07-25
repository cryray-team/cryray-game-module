#include "stdafx.h"
#include "GameVertex.h"
#include "../xrServerEntities/game_graph_space.h"
using XRay::GameVertex;

GameVertex::GameVertex(IntPtr InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(GameGraph::CVertex, InNativeObject);
}

System::UInt32 GameVertex::LevelVertexID::get()
{
	return pNativeObject->level_vertex_id();
}

System::Byte GameVertex::LevelID::get()
{
	return pNativeObject->level_id();
}

System::Byte GameVertex::EdgeCount::get()
{
	return pNativeObject->edge_count();
}

System::UInt32 GameVertex::EdgeOffset::get()
{
	return pNativeObject->edge_offset();
}

System::Numerics::Vector3 GameVertex::GamePoint::get()
{
	return Utils::FromFvector(IntPtr((void*)& pNativeObject->game_point()));
}

System::Numerics::Vector3 GameVertex::LevelPoint::get()
{
	return Utils::FromFvector(IntPtr((void*)& pNativeObject->level_point()));
}

array<::System::Byte>^ GameVertex::Mask::get()
{
	array<::System::Byte>^ Result = gcnew array<::System::Byte>(4);

	const u8* pMask = pNativeObject->vertex_type();
	Result[0] = pMask[0];
	Result[1] = pMask[1];
	Result[2] = pMask[2];
	Result[3] = pMask[3];

	return Result;
}