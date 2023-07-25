#pragma once
class CUIView;

namespace XRay {

	public ref class View
	{
	internal:
		CUIView* pNativeView;

	public:
		View();
		virtual ~View();

		virtual void Draw();
	};
}

