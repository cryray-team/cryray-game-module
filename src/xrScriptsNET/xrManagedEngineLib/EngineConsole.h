#pragma once
#include "API\NativeObject.h"

namespace XRay
{
	ref class EngineConsole abstract
	{
		static bool Check(::System::String^ Command);
		static void Execute(::System::String^ Command);
		static void Execute(::System::String^ Command, bool bEnable);
	};
}