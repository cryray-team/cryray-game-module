#include "stdafx.h"
#include "API/Inventory.h"
#include "../xrGame/InventoryOwner.h"
#include "../xrGame/Inventory.h"

using XRay::Inventory;

Inventory::Inventory(IntPtr InNativeObject) : NativeObject(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CInventoryOwner, InNativeObject);
}

XRay::PDA^ Inventory::PDAObject::get()
{
	CPda* pPDA = pNativeObject->GetPDA();
	if (pPDA)
	{
		return gcnew PDA(IntPtr(pPDA));
	}

	return nullptr;
}

float Inventory::TotalWeight::get()
{
	return pNativeObject->inventory().CalcTotalWeight();
}

u32 Inventory::Money::get()
{
	return pNativeObject->get_money();
}

void Inventory::Money::set(u32 Money)
{
	pNativeObject->set_money(Money, true);
}

System::String^ Inventory::Name::get()
{
	return gcnew String(pNativeObject->Name());
}

void Inventory::Name::set(::System::String^ Name)
{
	array<unsigned char>^ asciiName = ::System::Text::Encoding::ASCII->GetBytes(Name);
	pin_ptr<unsigned char> bufferPtr = &asciiName[0];
    pNativeObject->SetName((LPCSTR)*bufferPtr);
}

bool Inventory::CanTalk::get()
{
	return pNativeObject->IsTalkEnabled();
}

void Inventory::CanTalk::set(bool value)
{
	if (value)
		pNativeObject->EnableTalk();
	else
		pNativeObject->DisableTalk();
}

bool Inventory::CanTrade::get()
{
	return pNativeObject->IsTradeEnabled();
}

void Inventory::CanTrade::set(bool value)
{
	if (value)
		pNativeObject->EnableTrade();
	else
		pNativeObject->DisableTrade();
}