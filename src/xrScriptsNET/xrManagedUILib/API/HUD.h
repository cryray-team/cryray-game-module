#pragma once

using namespace System;
using namespace System::Numerics;

namespace XRay
{
	public ref class HUD sealed
	{
	public:
		static void Text(String^ text, Vector2 pos);
	};
}