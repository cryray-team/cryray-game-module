#pragma once
#include "PhysicsWorldScripted.h"
#include "API/WeatherFX.h"
#include "Game.h"

class CLevel;
class CLevelGraph;
class CPatrolPath;

namespace XRay
{

	ref class LevelGraph abstract
	{
	public:
		/// <summaru> Return Level ID</summaru>
		property ::System::UInt32 LevelID
		{
			::System::UInt32 get();
		}

		/// <summaru> Return Vertex count</summaru>
		property ::System::UInt32 VertexCount
		{
			::System::UInt32 get();
		}
		static ::System::UInt32 GetVertexId(::System::Numerics::Vector3 vPosition);

		static void PatrolPathAdd(::System::String^ patrol_path, CPatrolPath* path);
		static void PatrolPathRemove(::System::String^ patrol_path);
	};

	public ref class Level abstract
	{
	internal:
		CLevel* pNativeLevel;
	public:
		using CallBack = void(::System::String^);

	public:

		/// <summary>Get Name</summary>
		property ::System::String^ Name
		{
			::System::String^ get();
		}

		/// <summary>Return WfxTime</summary>
		static property float WfxTime
		{
			float get();
		}
		/// <summary>Return rain factor</summary>
		static property float RainFactor
		{
			float get();
		}
		/// <summary>Return level name from a text files</summary>
		static property ::System::String^ LevelName
		{
			::System::String^ get();
		}
		/// <summary>Return or set weather name</summary>
		static property ::System::String^ Weather
		{
			::System::String^ get();
			void set(::System::String^ str);
		}
		/// <summary>Get or set time factor</summary>
		static property float TimeFactor
		{
			float get();
			void set(float fFactor);
		}

		/// <summary>Is level inited</summary>
		static property bool LevelPresent
		{
			bool get();
		}

		static property GameObject^ ViewEntity
		{
			GameObject^ get();
			void set(GameObject^);
		}

		static float HighCoverInDirection(u32 level_vertex_id, ::System::Numerics::Vector3 vDirection);
		static float LowCoverInDirection(u32 level_vertex_id, ::System::Numerics::Vector3 vDirection);
		static ::System::Numerics::Vector3^ VertexPosition(u32 level_vertex_id);

		/// <summary>Check: Current level vertex be at level</summary>
		static bool ValidVertex(u32 level_vertex_id);

		static ::System::UInt32 VertexInDirection(u32 level_vertex_id, ::System::Numerics::Vector3 vDirection, float max_distance);

		static bool PatrolPathExists(::System::String^ PatrolPath);
		static XRay::PhysicsWorldScripted^ physicsWorldScripted();
#if 0
		static void AddDialogToRender(XRay::UIDialogWnd^ pDialog);
		static void RemoveDialogFromRender(XRay::UIDialogWnd^ pDialog);
#endif
		//static XRay::MEnvironment^ pEnvironment();
		static XRay::EnvDescriptor^ CurrentEnvironment(XRay::MEnvironment^ self);
		static void SpawnPhantom(::System::Numerics::Vector3 vPosition);
	};
}