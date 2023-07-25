#include "stdafx.h"
#include "GameInfo.h"
#include "Actor.h"
#include "..\xrGame\Actor.h"
#include "..\xrGame\InventoryOwner.h"

using namespace XRay;

bool GameInfo::HasInfo(::System::String^ InfoName)
{
    CInventoryOwner* pInventoryOwner = dynamic_cast<CInventoryOwner*>(g_actor);
	if (!pInventoryOwner) return false;

	string512 AsciiText = { 0 };
	ConvertDotNetStringToAscii(InfoName, AsciiText);

	return pInventoryOwner ? pInventoryOwner->HasInfo(AsciiText) : false;
}

bool GameInfo::DontHasInfo(::System::String^ InfoName)
{
	return !HasInfo(InfoName);
}

void GameInfo::AddInfo(::System::String^ InfoName)
{
    CInventoryOwner* pInventoryOwner = dynamic_cast<CInventoryOwner*>(g_actor);
	if (pInventoryOwner)
	{
		string512 AsciiText = { 0 };
		ConvertDotNetStringToAscii(InfoName, AsciiText);

		pInventoryOwner->TransferInfo(AsciiText, true);
	}
}

void GameInfo::RemoveInfo(::System::String^ InfoName)
{
    CInventoryOwner* pInventoryOwner = dynamic_cast<CInventoryOwner*>(g_actor);
	if (pInventoryOwner)
	{
		string512 AsciiText = { 0 };
		ConvertDotNetStringToAscii(InfoName, AsciiText);

		pInventoryOwner->TransferInfo(AsciiText, false);
	}
}