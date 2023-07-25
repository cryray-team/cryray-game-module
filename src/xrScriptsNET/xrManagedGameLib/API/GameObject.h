#pragma once
#include "API/GameVertex.h"

class CGameObject;
namespace XRay
{
	public ref class GameObject : public Object
	{
		CGameObject* pNativeObject;
	public:

		GameObject(IntPtr InNativeObject);

		property UInt32 AlifeStoryId
		{
			UInt32 get();
		}

		property UInt32 LevelVertexID
		{
			UInt32 get();
		}

		property UInt32 GameVertexID
		{
			UInt32 get();
		}

		property GameVertex GameVertexPtr
		{
			GameVertex get();
		}

		// Spawn time in Device.dwFrame
		property UInt32 SpawnTimeFrame
		{
			UInt32 get();
		}

		property bool IsAIObstacle
		{
			bool get();
		}

		BEGIN_DEFINE_EVENT(OnTradeStart)
		END_DEFINE_EVENT(OnTradeStart)

		BEGIN_DEFINE_EVENT(OnTradeStop)
		END_DEFINE_EVENT(OnTradeStop)

		BEGIN_DEFINE_EVENT(OnTradeSellBuyItem, GameObject^ Obj, bool bBuying, u32 MoneyTransfered)
		END_DEFINE_EVENT(OnTradeSellBuyItem, Obj, bBuying, MoneyTransfered)

		BEGIN_DEFINE_EVENT(OnTradePerformTradeOperation, u32 MoneyGet, u32 MoneyPut)
		END_DEFINE_EVENT(OnTradePerformTradeOperation, MoneyGet, MoneyPut)
		
		BEGIN_DEFINE_EVENT(OnZoneEnter, GameObject^ EnteredObject)
		END_DEFINE_EVENT(OnZoneEnter, EnteredObject)

		BEGIN_DEFINE_EVENT(OnZoneExit, GameObject^ EnteredObject)
		END_DEFINE_EVENT(OnZoneExit, EnteredObject)

		BEGIN_DEFINE_EVENT(OnLevelBorderEnter)
		END_DEFINE_EVENT(OnLevelBorderEnter)

		BEGIN_DEFINE_EVENT(OnLevelBorderExit)
		END_DEFINE_EVENT(OnLevelBorderExit)
	};
}