#pragma once
#include "UILines.h"

class CUIStatic;

namespace XRay
{
	public ref class UIStatic
	{
	internal:
		CUIStatic* pNativeStatic;

	public:
		UIStatic();
		UIStatic(::System::IntPtr pObject);

		~UIStatic();
		
		UILines^ TextItemControl();
		void	 InitTexture(::System::String^ Name);

		property bool StretchTexture
		{
			void set(bool bUse);
		}
	};
}