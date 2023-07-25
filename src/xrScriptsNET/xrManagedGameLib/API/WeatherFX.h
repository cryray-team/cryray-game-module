#pragma once

namespace XRay
{
	public ref class WeatherFX abstract
	{
	public:
		static void Create(::System::String^ Name);
		static void Stop();

		static property bool Enable
		{
			bool get();
		}
	};
}