#pragma once

#include "engineAPI.h"
#include "eventAPI.h"
#include "../xrRenderParticles/xrRenderParticles.h"
#include "xrSheduler.h"

class ENGINE_API CEngine
{
    HMODULE hPSGP;

public:
    BENCH_SEC_SCRAMBLEMEMBER1
    // DLL api stuff
    CEngineAPI External;
    CEventAPI Event;
    CSheduler Sheduler;

    void Initialize();
    void Destroy();

    CEngine();
    ~CEngine();
};

ENGINE_API extern CEngine Engine;
ENGINE_API extern xrDispatchTable PSGP;