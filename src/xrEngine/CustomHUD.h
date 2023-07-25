#pragma once

ENGINE_API extern Flags32 psHUD_Flags;

class ENGINE_API IRender_Visual;
class CUI;

class ENGINE_API CCustomHUD :
    public DLL_Pure,
    public IEventReceiver,
    public pureScreenResolutionChanged
{
public:
    CCustomHUD();
    virtual ~CCustomHUD();

    BENCH_SEC_SCRAMBLEVTBL2

    virtual void Render_First() { ; }
    virtual void Render_Last() { ; }
    virtual void Render_Actor_Shadow(){;} //Swartz: actor shadow
    BENCH_SEC_SCRAMBLEVTBL1

    virtual void OnFrame() { ; }
    virtual void OnEvent(EVENT E, u64 P1, u64 P2) { ; }

    virtual void Load() { ; }
    virtual void OnDisconnected() = 0;
    virtual void OnConnected() = 0;
    virtual void RenderActiveItemUI() = 0;
    virtual bool RenderActiveItemUIQuery() = 0;
    virtual void net_Relcase(CObject* object) = 0;
};

extern ENGINE_API CCustomHUD* g_hud;