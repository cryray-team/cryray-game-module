//-' OldSerpskiStalker
//-' Класс с выделением памяти для передачи данных из движка в решейд

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define USE_XRCORE 0

#if USE_XRCORE
#define XR_MODULE "[xrReshadeDecl]"
#include "xrCore/xrCore.h"
#endif

#ifdef DECL_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#define CRYRAY_ENGINE_X64

#define VERSION_FULL 5.6.0.0
#define VERSION_MAJOR 5
#define VERSION_MINOR 6
#define VERSION_REVISION 0
#define VERSION_BUILD 0

#define VERSION_STRING_FILE "5.6.0.0"
#define VERSION_STRING_PRODUCT "5.6.0 UNOFFICIAL for CryRay Engine x64"

class DLL_API ReShadeAPI
{
    bool active;

public:
    ReShadeAPI() { active = false; }
    virtual ~ReShadeAPI()
    {
#if USE_XRCORE
        Msg("~ ReShadeAPI::~ReShadeAPI()\n");
#endif
    }

    void* operator new(size_t size)
    {
        void* p = malloc(size);
#if USE_XRCORE
        Msg("~ Allocated %zu bytes @ %p\n", size, p);
#endif
        return p;
    }

    void operator delete(void* ptr)
    {
#if USE_XRCORE
        Msg("~ Freeing %p\n", ptr);
#endif
        free(ptr);
    }

    inline void SetActive(bool set) { this->active = set; };
    inline bool IsActive() { return this->active == true; };
};

extern DLL_API void InitReshadeAPI();
extern DLL_API void FreeReshadeAPI();

extern DLL_API ReShadeAPI* pReShadeAPI;