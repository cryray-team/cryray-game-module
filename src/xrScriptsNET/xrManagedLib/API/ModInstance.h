#pragma once

/// <summary>
/// Main namespace for all script classes of Spectre
/// 
/// ---
/// Spectre Engine, powered by Oxygen Team
/// X-Ray Oxygen 1.7f
/// </summary>
namespace XRay
{
	public ref class ModInstance abstract
	{
	public:
		virtual void OnLoad() = 0;
		virtual void OnShutdown() = 0;
	};
}