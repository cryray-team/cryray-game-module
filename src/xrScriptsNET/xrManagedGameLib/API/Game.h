#pragma once
#include "GameObject.h"
#include "GamePersistent.h"

namespace XRay
{
	public enum class ESingleGameDifficulty : u32
	{
		egdNovice = 0,
		egdStalker = 1,
		egdVeteran = 2,
		egdMaster = 3,
		egdCount,
		egd_force_u32 = u32(-1)
	};

	public enum class eTutorialState
	{
		eStart = 0,
		eStop,
		eCheckOnActiveTutorial // no used yet
	};


	public ref class Game abstract sealed
	{

	public: 
		using CallBack = void(::System::String^);
	public:


		ref struct SGameTime
		{
		private:

			static u32 _years;
			static u32 _months;
			static u32 _weeks;
			static u32 _days;
			static u32 _hours;
			static u32 _minutes;
			static u32 _seconds;
			static u32 _miliseconds;

		public:

			static property u32 Years
			{
				u32 get();
				void set(u32 value);
			}

			static property u32 Months
			{
				u32 get();
				void set(u32 value);
			}
			//////Weeks not used in Original //////
			static property u32 Weeks
			{
				u32 get();
				void set(u32 value);
			}
			
			static property u32 Days
			{
				u32 get();
				void set(u32 value);
			}

			static property u32 Hours
			{
				u32 get();
				void set(u32 value);
			}

			static property u32 Minutes
			{
				u32 get();
				void set(u32 value);
			}

			static property u32 Seconds
			{
				u32 get();
				void set(u32 value);
			}

			static property u32 Miliseconds
			{
				u32 get();
				void set(u32 value);
			}

			//static void ChangeGameTime(float fvalue);
		};

		/// <summary>Returns current game time</summary>
		static property SGameTime^ Time
		{
			SGameTime^ get();
		}

		static property bool IsDeveloperMode
		{
			bool get()
			{
				return GamePersistent().IsDeveloperMode();
			}
		}

		static property u64 StartTime
		{
			u64 get();
		};

		static property float SndVolume
		{
			float get();
			void set(float Value);
		}

		static ::System::String^ TranslateString(::System::String^ str);
		static void GlobalSend(NET_Packet& P);
		static void UserEventGen(NET_Packet& P, u32 _event, u32 _dest);

		static void setTutorialState(::System::String^ name, eTutorialState tutorialState);
		static property bool TutorialState
		{
			bool get();
		}

		/// <summary>Get or set game difficulty</summary>
		static property ESingleGameDifficulty GameDifficulty
		{
			ESingleGameDifficulty get();
			void set(ESingleGameDifficulty dif);
		}

		static Fbox GetBoundingVolume();

		static void SpawnSection(::System::String^ sSection, ::System::Numerics::Vector3 vPosition, u32 LevelVertexID, u16 ParentID) { SpawnSection(sSection, vPosition, LevelVertexID, ParentID, false); };
		static void SpawnSection(::System::String^ sSection, ::System::Numerics::Vector3 vPosition, u32 LevelVertexID, u16 ParentID, bool bReturnItem);
		
		/// <summary> Current target object </summary>
		static GameObject^	GlobalTargetObject();
		/// <summary> Distance to target object </summary>
		static float		GlobalTargetDist();
		/// <summary> Current target element </summary>
		static u32			GlobalTargetElement();
	};
}