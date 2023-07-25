#pragma once

namespace XRay
{
	[ClassRegistratorDecorator()]
	public ref class GameClassRegistrator
	{
	public:
		static void RegisterTypes();
	};
}