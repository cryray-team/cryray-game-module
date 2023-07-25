#pragma once
#include <windows.h>

class Lock
{
private:
    friend class AutoLock;
    CRITICAL_SECTION cs;

public:
    Lock() { InitializeCriticalSection(&cs); }

    ~Lock() { DeleteCriticalSection(&cs); }

    Lock(const Lock&) = delete;
    Lock& operator= (const Lock&) = delete;

    void Enter() { EnterCriticalSection(&cs); }

    bool TryEnter() { return !!TryEnterCriticalSection(&cs); }

    void Leave() { LeaveCriticalSection(&cs); }

    bool IsLocked() const { return cs.RecursionCount > 0 && (DWORD_PTR)cs.OwningThread == GetCurrentThreadId(); }
};
