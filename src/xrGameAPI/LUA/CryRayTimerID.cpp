///////////////////////////////////////////////////////////////////////////
//	File 		: CryRayTimerID.cpp
//	Author		: OldSerpskiStalker
//	Description : Timers, analogous to LUA AMK-Scripts modules
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CryRayTimerID.h"

namespace TimerID
{
    void StartTimer(std::shared_ptr<unsigned> idPtr, std::function<void()> callback, unsigned int start, unsigned int end)
    {
        Timer timer(std::move(idPtr), callback, start, end);

        for (auto i = timer.start; i <= timer.end; ++i)
        {
            Msg("Timer: %i", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        timer.callback();
    }
}