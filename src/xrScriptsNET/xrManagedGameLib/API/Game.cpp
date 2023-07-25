#include "stdafx.h"
#include "Game.h"
#include "../xrGame/date_time.h"
#include "../xrGame/Level.h"
#include "../xrGame/Actor.h"
#include "../xrGame/HUDManager.h"
#include "../xrGame/game_cl_single.h" 
#include "../xrGame/GamePersistent.h"

XRay::Game::SGameTime^ XRay::Game::Time::get()
{
	u32 year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
	SGameTime^ CurTime = gcnew SGameTime();

	if (g_pGameLevel != nullptr && ::Level().game != nullptr)
	{
		u64 packedTime = ::Level().GetGameTime();
		split_time(packedTime, year, month, day, hours, mins, secs, milisecs);
		CurTime->Years = year;
		CurTime->Months = month;
		//CurTime->Weeks = week;
		CurTime->Days = day;
		CurTime->Hours = hours;
		CurTime->Minutes = mins;
		CurTime->Seconds = secs;
		//	CurTime->Miliseconds = milisecs;
	}

	return CurTime;
}

void XRay::Game::SGameTime::Years::set(::System::UInt32 value)
{
	_years = value;
}
void XRay::Game::SGameTime::Months::set(::System::UInt32 value)
{
	_months = value;
}
void XRay::Game::SGameTime::Weeks::set(::System::UInt32 value)
{
	_weeks = value;
}
void XRay::Game::SGameTime::Days::set(::System::UInt32 value)
{
	_days = value;
}
void XRay::Game::SGameTime::Hours::set(::System::UInt32 value)
{
	_hours = value;
}
void XRay::Game::SGameTime::Minutes::set(::System::UInt32 value)
{
	_minutes = value;
}
void XRay::Game::SGameTime::Seconds::set(::System::UInt32 value)
{
	_seconds = value;
}
void XRay::Game::SGameTime::Miliseconds::set(::System::UInt32 value)
{
	_miliseconds = value;
}
::System::UInt32 XRay::Game::SGameTime::Years::get()
{
	return _years;
}
::System::UInt32 XRay::Game::SGameTime::Months::get()
{
	return _months;
}
::System::UInt32 XRay::Game::SGameTime::Weeks::get()
{
	return _weeks;
}
::System::UInt32 XRay::Game::SGameTime::Days::get()
{
	return _days;
}
::System::UInt32 XRay::Game::SGameTime::Hours::get()
{
	return _hours;
}
::System::UInt32 XRay::Game::SGameTime::Minutes::get()
{
	return _minutes;
}
::System::UInt32 XRay::Game::SGameTime::Seconds::get()
{
	return _seconds;
}
::System::UInt32 XRay::Game::SGameTime::Miliseconds::get()
{
	return _miliseconds;
}

void XRay::Game::GameDifficulty::set(ESingleGameDifficulty dif)
{
	g_SingleGameDifficulty = (::ESingleGameDifficulty)u32(dif);
	::Actor()->OnDifficultyChanged();
}

XRay::ESingleGameDifficulty XRay::Game::GameDifficulty::get()
{
	return (ESingleGameDifficulty)u32(g_SingleGameDifficulty);
}

u64 XRay::Game::StartTime::get()
{
	return ::Level().GetStartGameTime();
}

float XRay::Game::SndVolume::get()
{
	return psSoundVFactor;
}

void XRay::Game::SndVolume::set(float v)
{
	psSoundVFactor = v;
	clamp(psSoundVFactor, 0.0f, 1.0f);
}

extern GAME_API CUISequencer* g_tutorial;
extern GAME_API CUISequencer* g_tutorial2;

void XRay::Game::setTutorialState(::System::String^ name, eTutorialState tutorialState)
{
	TO_STRING64(NativeString, name);
	
	bool bStart = false;
	switch (tutorialState)
	{
	case XRay::eTutorialState::eStart:
		bStart = true;
		break;
	default:
		break;
	}

	GamePersistent().SetTutorialState(NativeString, bStart);
}
bool XRay::Game::TutorialState::get()
{
	return (g_tutorial != nullptr);
}

void XRay::Game::GlobalSend(NET_Packet& P)
{
	::Level().Send(P);
}

void XRay::Game::UserEventGen(NET_Packet& P, u32 _event, u32 _dest)
{
	CGameObject::u_EventGen(P, _event, _dest);
}

::System::String^ XRay::Game::TranslateString(::System::String^ str)
{
	TO_STRING64(NativeString, str);
	return gcnew ::System::String(NativeString);
}

Fbox XRay::Game::GetBoundingVolume()
{
	return ::Level().ObjectSpace.GetBoundingVolume();
}

XRay::GameObject^ XRay::Game::GlobalTargetObject()
{
	collide::rq_result& RQ = HUD().GetCurrentRayQuery();
	if (RQ.O)
	{
		CGameObject	*game_object = static_cast<CGameObject*>(RQ.O);
		if (game_object)
			return gcnew GameObject(::System::IntPtr(game_object));
	}
	return (nullptr);
}

float XRay::Game::GlobalTargetDist()
{
	collide::rq_result& RQ = HUD().GetCurrentRayQuery();
	if (RQ.range)
		return RQ.range;
	return (0);
}

void XRay::Game::SpawnSection(::System::String^ sSection, ::System::Numerics::Vector3 vPosition, u32 LevelVertexID, u16 ParentID, bool bReturnItem)
{
	TO_STRING64(NativeString, sSection);
	TO_FVECTOR(NativeVect, vPosition);
	
	::Level().spawn_item(NativeString, NativeVect, LevelVertexID, ParentID, bReturnItem);
}

::System::UInt32 XRay::Game::GlobalTargetElement()
{
	collide::rq_result& RQ = HUD().GetCurrentRayQuery();
	if (RQ.element)
	{
		return RQ.element;
	}
	return (0);
}