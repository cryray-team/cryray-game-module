///////////////////////////////////////////////////////////////////////////
//	File 		: CryRayTimerID.h
//	Author		: OldSerpskiStalker
//	Description : Timers, analogous to LUA AMK-Scripts modules
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <functional>

namespace TimerID
{
    struct Timer
    {
        std::shared_ptr<unsigned> idPtr;
        std::function<void()> callback;
        unsigned int start;
        unsigned int end;

        Timer(std::shared_ptr<unsigned> idPtr, std::function<void()> callback, unsigned int start, unsigned int end) 
            : idPtr(std::move(idPtr)), callback(callback), start(start), end(end) {}
    };

    extern void StartTimer(
        std::shared_ptr<unsigned> idPtr, std::function<void()> callback, unsigned int start, unsigned int end);
    }