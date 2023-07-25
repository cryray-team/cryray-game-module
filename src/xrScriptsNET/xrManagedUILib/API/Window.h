#pragma once

using namespace System;
using namespace System::Numerics;

namespace XRay
{
	[::System::FlagsAttribute()]
	public enum class WindowAligment : int
	{
		None	= 0,
		Left	= 1,
		Right	= 2,
		Top		= 4,
		Bottom	= 8,
		Center	= 16
	};

	public ref class Window
	{
	internal:
		class CUIWindow* pNativeObject;

	public:

		Window(IntPtr InNativeObject);

		property Vector2 Position
		{
			Vector2 get();
			void set(Vector2 InPos);
		}

		property Vector2 Size
		{
			Vector2 get();
			void set(Vector2 InSize);
		}

		property float Width
		{
			float get();
			void set(float InWidth);
		}

		property float Height
		{
			float get();
			void set(float InHeight);
		}

		property WindowAligment Aligment
		{
			WindowAligment get();
			void set(WindowAligment value);
		}

		IntPtr EngineWNDClass()
		{
			return (IntPtr)pNativeObject;
		}
	};
}