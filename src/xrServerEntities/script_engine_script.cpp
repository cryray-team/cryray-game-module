////////////////////////////////////////////////////////////////////////////
//	Module 		: script_engine_script.cpp
//	Created 	: 25.12.2002
//  Modified 	: 10.01.2015
//	Author		: Dmitriy Iassenev
//	Description : ALife Simulator script engine export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_engine.h"
#include "ai_space.h"
#include "script_debugger.h"

using namespace luabind;

void LuaLog1(LPCSTR caMessage)
{
#ifndef MASTER_GOLD
    ai().script_engine().script_log(ScriptStorage::eLuaMessageTypeMessage, "%s", caMessage);
#else
    Log(caMessage);
#endif // #ifndef MASTER_GOLD

#ifdef USE_DEBUGGER
#ifndef USE_LUA_STUDIO
    if (ai().script_engine().debugger())
        ai().script_engine().debugger()->Write(caMessage);
#endif // #ifndef USE_LUA_STUDIO
#endif // #ifdef USE_DEBUGGER
}

void ErrorLog(LPCSTR caMessage)
{
    ai().script_engine().error_log("%s", caMessage);
    // #ifdef PRINT_CALL_STACK
    ai().script_engine().print_stack();
    // #endif //-PRINT_CALL_STACK

#ifdef USE_DEBUGGER
#ifndef USE_LUA_STUDIO
    if (ai().script_engine().debugger())
    {
        ai().script_engine().debugger()->Write(caMessage);
    }
#endif //! USE_LUA_STUDIO
#endif //-USE_DEBUGGER

#ifdef DEBUG
    bool lua_studio_connected = !!ai().script_engine().debugger();
    if (!lua_studio_connected)
#endif // #ifdef DEBUG
        R_ASSERT2(0, caMessage);
}

// AVO:
void PrintStack() { ai().script_engine().print_stack(); }

//-AVO

void FlushLogs()
{
    // #ifdef DEBUG
    FlushLog();
#ifdef LUA_DEBUG_PRINT // DEBUG
    ai().script_engine().flush_log();
#endif // DEBUG
}

void verify_if_thread_is_running()
{
    THROW2(ai().script_engine().current_thread(), "coroutine.yield() is called outside the LUA thread!");
}

bool is_editor()
{
#ifdef XRGAME_EXPORTS
    return (false);
#else
    return (true);
#endif
}

#ifdef XRGAME_EXPORTS
CRenderDevice* get_device() { return (&Device); }
#endif

int bit_and(int i, int j) { return (i & j); }

int bit_or(int i, int j) { return (i | j); }

int bit_xor(int i, int j) { return (i ^ j); }

int bit_not(int i) { return (~i); }

LPCSTR user_name() { return (Core.UserName); }

void prefetch_module(LPCSTR file_name) { ai().script_engine().process_file(file_name); }

struct profile_timer_script
{
    using Clock = std::chrono::high_resolution_clock;
    using Time = Clock::time_point;
    using Duration = Clock::duration;

    Time start_time;
    Duration accumulator;
    u64 count = 0;
    int recurse_mark = 0;

    profile_timer_script() : start_time(), accumulator(), count(0), recurse_mark(0) {}

    bool operator<(const profile_timer_script& profile_timer) const { return accumulator < profile_timer.accumulator; }

    void start()
    {
        if (recurse_mark)
        {
            ++recurse_mark;
            return;
        }

        ++recurse_mark;
        ++count;
        start_time = Clock::now();
    }

    void stop()
    {
        if (!recurse_mark)
            return;

        --recurse_mark;

        if (recurse_mark)
            return;

        const auto finish = Clock::now();
        if (finish > start_time)
            accumulator += finish - start_time;
    }

    float time() const
    {
        using namespace std::chrono;
        return float(duration_cast<milliseconds>(accumulator).count()) * 1000000.f;
    }
};

IC profile_timer_script operator+(const profile_timer_script& portion0, const profile_timer_script& portion1)
{
    profile_timer_script result;
    result.accumulator = portion0.accumulator + portion1.accumulator;
    result.count = portion0.count + portion1.count;
    return result;
}

// IC	std::ostream& operator<<(std::ostream &stream, profile_timer_script &timer)
// {
// 	stream					<< timer.time();
// 	return					(stream);
// }

#ifdef XRGAME_EXPORTS
ICF u32 script_time_global() { return Device.dwTimeGlobal; }
ICF u32 script_time_global_async() { return Device.TimerAsync_MMT(); }
#else
ICF u32 script_time_global() { return 0; }
ICF u32 script_time_global_async() { return 0; }
#endif //-XRGAME_EXPORTS

#pragma optimize("s", on)
void CScriptEngine::script_register(lua_State* L)
{
    module(L)[
        // def("log1", (void(*) (LPCSTR msg)) &Log), // AVO: fixed log func
        def("log", &LuaLog1), def("print_stack", &PrintStack), def("error_log", &ErrorLog), def("flush", &FlushLogs),
        def("prefetch", &prefetch_module), def("verify_if_thread_is_running", &verify_if_thread_is_running),
        def("editor", &is_editor), def("bit_and", &bit_and), def("bit_or", &bit_or), def("bit_xor", &bit_xor),
        def("bit_not", &bit_not), def("user_name", &user_name), def("time_global", &script_time_global),
        def("time_global_async", &script_time_global_async),
#ifdef XRGAME_EXPORTS
        def("device", &get_device),
#endif //-XRGAME_EXPORTS
        class_<profile_timer_script>("profile_timer")
            .def(constructor<>())
            .def(constructor<profile_timer_script&>())
            .def(const_self + profile_timer_script())
            .def(const_self < profile_timer_script())
            .def(tostring(self))
            .def("start", &profile_timer_script::start)
            .def("stop", &profile_timer_script::stop)
            .def("time", &profile_timer_script::time)];

    // function(L, "print_stack", PrintStack);
    // function(L, "log", LuaLog1);
    // function(L, "error_log", ErrorLog);
    // function(L, "flush", FlushLogs);
    // function(L, "prefetch", prefetch_module);
    // function(L, "verify_if_thread_is_running", verify_if_thread_is_running);
    // function(L, "editor", is_editor);
    // function(L, "bit_and", bit_and);
    // function(L, "bit_or", bit_or);
    // function(L, "bit_xor", bit_xor);
    // function(L, "bit_not", bit_not);
    // function(L, "user_name", user_name);
    // function(L, "time_global", script_time_global);
    // function(L, "time_global_async", script_time_global_async);
    // #ifdef XRGAME_EXPORTS
    // function(L, "device", get_device);
    // function(L, "is_enough_address_space_available", is_enough_address_space_available_impl);
    // #endif //-XRGAME_EXPORTS
}
