#pragma once
class CTorch;

namespace XRay
{
	public ref class Torch
	{
	internal:
	CTorch* pNativeObject;
	
	public:
		Torch(::System::IntPtr InNativeObject);

		property bool IsEnabled
		{
			bool get();
			void set(bool value);
		}
	};
}