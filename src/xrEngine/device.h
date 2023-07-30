#pragma once

#include "pure.h"
#include "../xrCore/ftimer.h"
#include "../xrThreads/Event.hpp"
#include "stats.h"

#include <psapi.h>
#include <mmsystem.h>

#define VIEWPORT_NEAR 0.2f		//Main viewport near
#define R_VIEWPORT_NEAR 0.005f	//Second viewport near (to avoid z-fighting)

#define DEVICE_RESET_PRECACHE_FRAME_COUNT 10

#pragma warning(disable:4172) //-' Возвращение адреса локальной или временной переменной

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/RenderDeviceRender.h"
#ifndef SPECTRE
#include "task_group.h"
#endif
#include "../xrCryRayAPI/Semki.h"

class engine_impl;

#pragma pack(push,4)

class IRenderDevice
{
public:
    virtual CStatsPhysics* 	 StatPhysics() = 0;
    virtual void 	 AddSeqFrame(pureFrame* f, bool mt) = 0;
    virtual void 	 RemoveSeqFrame(pureFrame* f) = 0;
};

class ENGINE_API CRenderDeviceData
{
public:
    u32 dwWidth;
    u32 dwHeight;

    u32 dwPrecacheFrame;
    BOOL b_is_Ready;
    BOOL b_is_Active;
    BOOL b_hide_cursor;

public:
    // Engine flow-control
    u32 dwFrame;

    float fTimeDelta;
    float fTimeGlobal;
    u32 dwTimeDelta;
    u32 dwTimeGlobal;
    u32 dwTimeContinual;

    Fvector vCameraPosition;
    Fvector vCameraDirection;
    Fvector vCameraTop;
    Fvector vCameraRight;

    Fmatrix mView;
    Fmatrix mProject;
    Fmatrix mFullTransform;

    // Copies of corresponding members. Used for synchronization.
    Fvector vCameraPosition_saved;

    Fmatrix mView_saved;
    Fmatrix mProject_saved;
    Fmatrix mFullTransform_saved;

    float fFOV;
    float fASPECT;

protected:
    u32 Timer_MM_Delta;
    CTimer_paused Timer;
    CTimer_paused TimerGlobal;

    //AVO: 
    CTimer frame_timer;   //TODO: проверить, не дублируется-ли схожий таймер (alpet)
    //-AVO

public:
    // Registrators
    CRegistrator <pureRender > seqRender;
    CRegistrator <pureAppActivate > seqAppActivate;
    CRegistrator <pureAppDeactivate > seqAppDeactivate;
    CRegistrator <pureAppStart > seqAppStart;
    CRegistrator <pureAppEnd > seqAppEnd;
    CRegistrator <pureFrame > seqFrame;
    CRegistrator <pureScreenResolutionChanged> seqResolutionChanged;

    HWND m_hWnd;
};

class ENGINE_API CRenderDeviceBase :
    public IRenderDevice,
    public CRenderDeviceData
{
public:
};

class ENGINE_API CSecondVPParams //--#SM+#-- +SecondVP+
{
    bool isActive; // Флаг активации рендера во второй вьюпорт
    u8 frameDelay; // На каком кадре с момента прошлого рендера во второй вьюпорт мы начнём новый
                   //(не может быть меньше 2 - каждый второй кадр, чем больше тем более низкий FPS во втором вьюпорте)

public:
    bool isCamReady; // Флаг готовности камеры (FOV, позиция, и т.п) к рендеру второго вьюпорта

    bool isR1;

    IC bool IsSVPActive() { return isActive; }
    IC void SetSVPActive(bool bState);
    bool IsSVPFrame();

    IC u8 GetSVPFrameDelay() { return frameDelay; }
    void SetSVPFrameDelay(u8 iDelay)
    {
        frameDelay = iDelay;
        clamp<u8>(frameDelay, 1, u8(-1));
    }
};

#pragma pack(pop)
// refs
class ENGINE_API CRenderDevice : public CRenderDeviceBase
{
private:
    // Main objects used for creating and rendering the 3D scene
    u64 m_dwWindowStyle;
    RECT m_rcWindowBounds;
    RECT m_rcWindowClient;
    CTimer TimerMM;

    void _Create(LPCSTR shName);
    void _Destroy(BOOL bKeepTextures);
    void _SetupStates();

public:
    u32 dwPrecacheTotal;
    float fWidth_2, fHeight_2;
    void OnWM_Activate(WPARAM wParam, LPARAM lParam);

public:
    IRenderDeviceRender* m_pRender;

    BOOL m_bNearer;
    void SetNearer(BOOL enabled)
    {
        if (enabled&&!m_bNearer)
        {
            m_bNearer = TRUE;
            mProject._43 -= EPS_L;
        }
        else if (!enabled&&m_bNearer)
        {
            m_bNearer = FALSE;
            mProject._43 += EPS_L;
        }
        m_pRender->SetCacheXform(mView, mProject);
    }

    void DumpResourcesMemoryUsage() { m_pRender->ResourcesDumpMemoryUsage(); }
public:
    // TBB
#ifndef SPECTRE
    tbb::task_group m_CalcAsync;
#endif
    // Registrators
    CRegistrator <pureFrame > seqFrameMT;
    CRegistrator <pureDeviceReset > seqDeviceReset;
	CSecondVPParams m_SecondViewport;	//--#SM+#-- +SecondVP+

	xr_vector <fastdelegate::FastDelegate<void()>> seqParallel;
	
    CStats* Statistic;

    Fmatrix mInvFullTransform;
    // Saved main viewport params
    Fvector mainVPCamPosSaved;
    Fmatrix mainVPFullTrans;
    Fmatrix mainVPViewSaved;
    Fmatrix mainVPProjectSaved;

    CRenderDevice();
    ~CRenderDevice();

    void Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason);
    bool Paused();

    // Scene control
    void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input);
    BOOL Begin();
    void Clear();
    void End();
    void FrameMove();

    void overdrawBegin();
    void overdrawEnd();

    // Console Screenshot
    void Screenshot();

    // Mode control
    IC CTimer_paused* GetTimerGlobal() { return &TimerGlobal; }
    u64 TimerAsync() { return TimerGlobal.GetElapsed_ms(); }
    u64 TimerAsync_MMT() { return TimerMM.GetElapsed_ms() + Timer_MM_Delta; }

    // Creation & Destroying
    void ConnectToRender();
    void Create(void);
    void Run(void);
    void Destroy(void);
    void Reset(bool precache = true);

    void Initialize(void);
    bool IsMainMenuActive();
	
public:
    void time_factor(const float& time_factor); //--#SM+#--

    IC const float& time_factor() const
    {
        VERIFY(Timer.time_factor() == TimerGlobal.time_factor());
        return (Timer.time_factor());
    }

    // Multi-threading
    Lock mt_csEnter;
    Lock mt_csLeave;
    volatile BOOL mt_bMustExit;
    
    ICF void remove_from_seq_parallel(const fastdelegate::FastDelegate<void()> &delegate)
    {
        seqParallel.erase( std::remove( seqParallel.begin(), seqParallel.end(), delegate ), seqParallel.end() );
    }

    //AVO: elapsed famed counter (by alpet)
    IC u64 frame_elapsed()
    {
        return frame_timer.GetElapsed_ms();
    }

public:
    void on_idle();
    bool on_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
    void message_loop();
    virtual void 	 AddSeqFrame(pureFrame* f, bool mt);
    virtual void 	 RemoveSeqFrame(pureFrame* f);
    virtual CStatsPhysics* 	 StatPhysics() { return Statistic; }
};

extern ENGINE_API CRenderDevice Device;
extern ENGINE_API xr_list<fastdelegate::FastDelegate<bool()>> g_loading_events;

class ENGINE_API CLoadScreenRenderer :public pureRender
{
public:
    CLoadScreenRenderer();
    void start(bool b_user_input);
    void stop();
    virtual void OnRender();

    bool b_registered;
    bool b_need_user_input;
};

extern ENGINE_API Flags32 p_engine_flags32;

extern ENGINE_API u32 g_crosshair_color;
extern ENGINE_API Ivector4 g_crosshair_color_temp;

#define C_DEFAULT D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0x80)

extern ENGINE_API BOOL g_bRendering;
extern ENGINE_API float g_AimLookFactor;
extern ENGINE_API bool renderer_allow_override;
extern ENGINE_API CLoadScreenRenderer load_screen_renderer;
