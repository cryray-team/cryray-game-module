#pragma once
#include "API/NativeObject.h"

class IGame_Persistent;
using namespace System;

namespace XRay
{
	public ref class CGamePersistence : NativeObject
	{
	internal:
		IGame_Persistent* pNativeObject;

	public:
		CGamePersistence();
		CGamePersistence(IntPtr InNativeObject);

		virtual void shedule_update(u32 interval);
	};
}