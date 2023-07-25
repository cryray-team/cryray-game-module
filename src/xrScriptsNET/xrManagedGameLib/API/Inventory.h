#pragma once
#include "API/PDA.h"

namespace XRay
{
	public ref class Inventory : public NativeObject
	{
		CInventoryOwner* pNativeObject;

	public:
		Inventory(IntPtr InNativeObject);

		property float TotalWeight
		{
			float get();
		}

		// Can return null, be careful
		property PDA^ PDAObject
		{
			PDA^ get();
		}

		property UInt32 Money
		{
			UInt32 get();
			void set(UInt32 value);
		}

		property String^ Name
		{
			String^ get();
			void set(String^ value);
		}

		property bool CanTalk
		{
			bool get();
			void set(bool value);
		}

		property bool CanTrade
		{
			bool get();
			void set(bool value);
		}
	};
}