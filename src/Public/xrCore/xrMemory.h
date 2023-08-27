#pragma once

#define DUMP_PHASE do {} while (0)

class XRCORE_API xrMemory
{
public:
    struct mdbg
    {
        void* _p;
        size_t _size;
        const char* _name;
        u32 _dummy;
    };
public:
    xrMemory();
    void Initialize(BOOL _debug_mode = FALSE);
    void Destroy();

    struct SProcessMemInfo
    {
        u64 PeakWorkingSetSize;
        u64 WorkingSetSize;
        u64 PagefileUsage;
        u64 PeakPagefileUsage;

        u64 TotalPhysicalMemory;
        s64 FreePhysicalMemory;
        u64 TotalVirtualMemory;
        u32 MemoryLoad;
    };

    void GetProcessMemInfo(SProcessMemInfo& minfo);

    u32 stat_calls;
    s32 stat_counter;

public:
    size_t mem_usage();
    void mem_compact();
    void mem_counter_set(u32 _val) { stat_counter = _val; }
    u32 mem_counter_get() { return stat_counter; }

    void* mem_alloc (size_t size );
    void* mem_realloc (void* p, size_t size );

    void mem_free(void* p);

	void* (WINAPIV* mem_copy)(void*, const void*, size_t) = std::memcpy;
	void* (WINAPIV* mem_fill)(void*, int, size_t) = std::memset;
};

extern XRCORE_API xrMemory Memory;

#undef ZeroMemory
#undef CopyMemory
#undef FillMemory
#define ZeroMemory(a, b) std::memset(a, 0, b)
#define CopyMemory(a, b, c) std::memcpy(a, b, c)
#define FillMemory(a, b, c) std::memset(a, c, b)

#include "xrMemory_subst_msvc.h"

template <class T>
IC T* xr_alloc (u32 count) { return (T*)Memory.mem_alloc(count*sizeof(T)); }
template <class T>
IC void xr_free (T*& P) { if (P) { Memory.mem_free((void*)P); P=NULL; }; }
IC void* xr_malloc (size_t size) { return Memory.mem_alloc(size); }
IC void* xr_realloc (void* P, size_t size) { return Memory.mem_realloc(P,size); }

XRCORE_API char* xr_strdup(const char* string);

//////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4595) //-' В вызове функции отсутствует список аргументов
IC void* operator new (size_t size) { return Memory.mem_alloc(size ? size : 1); }
IC void operator delete (void* p) { xr_free(p); }
IC void* operator new[] (size_t size) { return Memory.mem_alloc(size ? size : 1); }
IC void operator delete[] (void* p) { xr_free(p); }
//////////////////////////////////////////////////////////////////////////////////////////

XRCORE_API void vminfo(size_t* _free, size_t* reserved, size_t* committed);
XRCORE_API void log_vminfo();