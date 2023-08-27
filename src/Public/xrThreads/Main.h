#pragma once

#ifdef XRTHREADS_EXPORTS
#define THREADS_API __declspec(dllexport)
#else
#define THREADS_API __declspec(dllimport)
#endif

class THREADS_API Threads
{
public:
    static void CheckThreadsDLL();
};