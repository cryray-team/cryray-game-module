#pragma once

namespace XRay 
{
	public ref class GameInfo abstract sealed
	{
	public:
		/// <summary> Does the Actor have infoportion </summary>
		static bool HasInfo(::System::String^ InfoName);
		/// <summary> Does the Actor have no infoportion </summary>
		static bool DontHasInfo(::System::String^ InfoName);

		/// <summary> Give infoportion to Actor </summary>
		static void AddInfo(::System::String^ InfoName);
		/// <summary> Strip the actor infoportion </summary>
		static void RemoveInfo(::System::String^ InfoName);
	};
}