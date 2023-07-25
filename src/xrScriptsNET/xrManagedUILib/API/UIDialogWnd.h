#pragma once

#include "../xrUICore/UIDialogWnd.h"
#include "../xrUIcore/IGameUI.h"

namespace XRay
{
	public ref class UIDialogWnd abstract
	{
	public:
		UIDialogWnd();
		~UIDialogWnd();

		static ::System::IntPtr GetGameUI() { return (::System::IntPtr)pUIHud; }
		::System::IntPtr GetNative() { return (::System::IntPtr)pNativeLevel; }

	internal:
		static CUIDialogWnd* pNativeLevel;
	};
}
