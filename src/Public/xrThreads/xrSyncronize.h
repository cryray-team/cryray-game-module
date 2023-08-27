#pragma once
#include <mutex>
#include "../xrThreads/main.h"

namespace ThreadsSyncronize
{
    // Desc: Simple wrapper for critical section
    class THREADS_API xrCriticalSection
    {
    private:
        void* pmutex;

    public:
        xrCriticalSection();
        ~xrCriticalSection();

        void Enter();
        void Leave();
        BOOL TryEnter();
    };

    class THREADS_API xrRecursiveMutex
    {
    private:
        std::recursive_mutex* pmutex;

    public:
        xrRecursiveMutex();

        ~xrRecursiveMutex();

        void Enter();
        void Leave();
        BOOL TryEnter();
    };

    // very slow due mem allocation
    class THREADS_API xrSharedCriticalSectionGuard
    {
    public:
        explicit xrSharedCriticalSectionGuard(xrCriticalSection& InSection);
        xrSharedCriticalSectionGuard(const xrSharedCriticalSectionGuard& InSharedSection);
        ~xrSharedCriticalSectionGuard();

        xrSharedCriticalSectionGuard& operator=(const xrSharedCriticalSectionGuard& Other);

    private:
        void ConditionalFreeLock();

        std::atomic<u32>* pReference;
        xrCriticalSection* pSection;
    };

    class THREADS_API xrCriticalSectionGuard
    {
    public:
        explicit xrCriticalSectionGuard(xrCriticalSection& InSection);
        ~xrCriticalSectionGuard();

        // do not allow sharing
        xrCriticalSectionGuard(const xrCriticalSectionGuard& Other) = delete;
        xrCriticalSectionGuard& operator=(const xrCriticalSectionGuard& Other) = delete;

    private:
        xrCriticalSection& Section;
    };

    class THREADS_API xrRecursiveMutexGuard
    {
    public:
        explicit xrRecursiveMutexGuard(xrRecursiveMutex& InMutex);
        ~xrRecursiveMutexGuard();

        // do not allow sharing
        xrRecursiveMutexGuard(const xrRecursiveMutexGuard& Other) = delete;
        xrRecursiveMutexGuard& operator=(const xrRecursiveMutexGuard& Other) = delete;

    private:
        xrRecursiveMutex& Mutex;
    };

    typedef xrCriticalSection CriticalSection;
    typedef xrRecursiveMutex RecursiveMutex;
    typedef xrSharedCriticalSectionGuard SharedCriticalSectionGuard;
    typedef xrCriticalSectionGuard CriticalSectionGuard;
    typedef xrRecursiveMutexGuard RecursiveMutexGuard;
}