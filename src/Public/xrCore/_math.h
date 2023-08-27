#pragma once

struct ProcessorInfo;

namespace CPU
{
    XRCORE_API extern u64 clk_per_second;
    XRCORE_API extern u64 clk_per_milisec;
    XRCORE_API extern u64 clk_per_microsec;
    XRCORE_API extern u64 clk_overhead;
    XRCORE_API extern float clk_to_seconds;
    XRCORE_API extern float clk_to_milisec;
    XRCORE_API extern float clk_to_microsec;

    XRCORE_API extern u64 qpc_freq;
    XRCORE_API extern u64 qpc_overhead;
    XRCORE_API extern u32 qpc_counter;

    XRCORE_API extern ProcessorInfo ID;
    XRCORE_API extern u64 QPC();
    XRCORE_API extern u64 QPC_Freq();

    IC u64 GetCLK(void)
    {
        return __rdtsc();
    }
};

extern XRCORE_API void _initialize_cpu();
extern XRCORE_API void _initialize_cpu_thread();

// threading
typedef void thread_t(void*);
extern XRCORE_API void thread_name(const char* name);
extern XRCORE_API void thread_spawn(thread_t* entry, const char* name, unsigned stack, void* arglist);
