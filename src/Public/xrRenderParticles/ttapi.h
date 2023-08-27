#pragma once

#include <windows.h>

/*
    Trivial (and dumb) Threads API
*/

typedef VOID (*PTTAPI_WORKER_FUNC)(LPVOID lpWorkerParameters);
typedef PTTAPI_WORKER_FUNC LPPTTAPI_WORKER_FUNC;

#ifdef XRRENDERPARTICLES_EXPORTS
#define TTAPI __declspec(dllexport)
#else // XRRENDERPARTICLES_EXPORTS
#define TTAPI __declspec(dllimport)
#endif // XRRENDERPARTICLES_EXPORTS

extern "C" 
{
// Initializes subsystem
// Returns zero for error, and number of workers on success
DWORD TTAPI ttapi_Init(ProcessorInfo* ID);

// Destroys subsystem
VOID TTAPI ttapi_Done();

// Return number of workers
DWORD TTAPI ttapi_GetWorkersCount();

// Return number of threads
DWORD TTAPI ttapi_GetThreadsCount();

// Adds new task
// No more than TTAPI_HARDCODED_THREADS should be added
VOID TTAPI ttapi_AddWorker(LPPTTAPI_WORKER_FUNC lpWorkerFunc, LPVOID lpvWorkerFuncParams);

// Runs and wait for all workers to complete job
VOID TTAPI ttapi_RunAllWorkers();
}
