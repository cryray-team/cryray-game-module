#pragma once
#include "UILines.h"

class CUIProgressBar;

namespace XRay
{
	public ref class UIProgressBar
	{
	internal:
		CUIProgressBar* pNativeStatic;

	public:
		UIProgressBar();
		UIProgressBar(::System::IntPtr pObject);

		~UIProgressBar();
	};
}