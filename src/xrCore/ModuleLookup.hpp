#pragma once
#include "xr_smart_pointers.h"

namespace CryRayLoadLib
{
    class XRCORE_API ModuleHandle
    {
        void* handle;
        bool dontUnload;

    public:
        ModuleHandle(const bool dontUnload = false);
        ModuleHandle(const char* moduleName, bool dontUnload = false);
        ~ModuleHandle();

        void* Open(const char* moduleName);
        void Close();

        bool IsLoaded() const;

        void* operator()() const;

        void* GetProcAddress(const char* procName) const;
    };

    using Module = xr_unique_ptr<ModuleHandle>;

    inline auto LoadModule(bool dontUnload = false)
    {
        return xr_make_unique<ModuleHandle>(dontUnload);
    }

    inline auto LoadModule(const char* moduleName, bool dontUnload = false)
    {
        return xr_make_unique<ModuleHandle>(moduleName, dontUnload);
    }
}