#pragma once
#include <mutex>
#include <Windows.h>

class Event
{
    void* handle;

public:
    Event() noexcept { handle = (void*)CreateEvent(nullptr, FALSE, FALSE, nullptr); }

    ~Event() noexcept { CloseHandle(handle); }

    Event(std::nullptr_t) noexcept { handle = nullptr; }

    Event(void* event) noexcept { handle = event; }

    // Reset the event to the unsignalled state.
    void Reset() noexcept { ResetEvent(handle); }

    // Set the event to the signalled state.
    void Set() noexcept { SetEvent(handle); }

    // Wait indefinitely for the object to become signalled.
    void Wait() noexcept { WaitForSingleObject(handle, INFINITE); }

    // Wait, with a time limit, for the object to become signalled.
    bool Wait(unsigned int millisecondsTimeout) noexcept
    {
        return WaitForSingleObject(handle, millisecondsTimeout) != WAIT_TIMEOUT;
    }

    // Expected thread can issue an exception. But it requires processing one message from HWND message queue,
    // otherwise, the thread can't show an error message.
    void WaitEx(unsigned int millisecondsTimeout) noexcept
    {
        DWORD WaitResult = WAIT_TIMEOUT;
        do
        {
            WaitResult = WaitForSingleObject(handle, millisecondsTimeout);
            if (WaitResult == WAIT_TIMEOUT)
            {
                MSG msg;
                if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        } while (WaitResult == WAIT_TIMEOUT);
    }

    void* GetHandle() noexcept { return handle; }

    bool Valid() const noexcept { return handle != nullptr; }
};

#ifndef SPECTRE
typedef std::mutex Mutex;
typedef std::recursive_mutex RMutex;
#endif