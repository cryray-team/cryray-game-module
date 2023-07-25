#pragma once
class CUILines;

namespace XRay
{
	public ref class UILines
	{
	internal:
		CUILines* pNativeLines;

	public:
		UILines();
		UILines(CUILines* pLines);

		~UILines();
	};
}