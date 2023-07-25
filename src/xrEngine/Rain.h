// Rain.h: interface for the CRain class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrcdb/xr_collide_defs.h"

//refs
class ENGINE_API IRender_DetailModel;

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/RainRender.h"

#define RAIN_MANAGER_LTX "rain_manager"

struct SRainParams
{
public:
    int max_desired_items;
    float source_offset;
    float max_distance, sink_offset;
    float drop_angle, drop_max_angle;
    float drop_max_wind_vel;
    int max_particles, particles_cache;
    float particles_time;
    u32 dwReferences;

public:
    SRainParams();
};

class ENGINE_API CEffect_Rain
{
    friend class dxRainRender;

private:
    struct RainTimer
    {
        bool			bFirstRainingFrame;
        float			rainTimestamp;
        float			rainDropTimeBasic;
        float			previousFrameTime;
        float			lastRainDuration;
        float			rainDropTime;
        float			rainTimer;

        RainTimer() : bFirstRainingFrame(true), rainTimestamp(0.0f), rainDropTimeBasic(20.0f), previousFrameTime(0.0f),
            lastRainDuration(0.0f), rainDropTime(0.0f), rainTimer(0.0f) {};
    };

    RainTimer* timerWorld;
    RainTimer* timerCurrViewEntity;

public:
    struct RainParams
    {
        float			rainTimer;
    };

private:
    struct Item
    {
        Fvector P;
        Fvector Phit;
        Fvector D;
        float fSpeed;
        u32 dwTime_Life;
        u32 dwTime_Hit;
        u32 uv_set;
        void invalidate() { dwTime_Life = 0; }
    };
    struct Particle
    {
        Particle* next, * prev;
        Fmatrix mXForm;
        Fsphere bounds;
        float time;
    };
    enum States
    {
        stIdle = 0,
        stWorking
    };

private:
    FactoryPtr<IRainRender> m_pRender;

    xr_vector<Item> items;
    States state;

    xr_vector<Particle> particle_pool;
    Particle* particle_active;
    Particle* particle_idle;

public:
    ref_sound snd_Ambient;
    float rain_volume;

private:
    ref_sound snd_Wind;
    bool bWindWorking;

    void p_create();
    void p_destroy();

    void p_remove(Particle* P, Particle*& LST);
    void p_insert(Particle* P, Particle*& LST);
    int p_size(Particle* LST);
    Particle* p_allocate();
    void p_free(Particle* P);

    void Born(Item& dest, float radius);
    void Hit(Fvector& pos);
    BOOL RayPick(const Fvector& s, const Fvector& d, float& range, collide::rq_target tgt);
    void RenewItem(Item& dest, float height, BOOL bHit);

public:
    CEffect_Rain();
    ~CEffect_Rain();

    void Render();
    void OnFrame();

    void StopAmbient();
    void SetInvalidateState();

    void InvalidateState()
    {
        if (state != stIdle)
            snd_Ambient.stop();

        rain_volume = 0.0f;
        state = stIdle;
    }

    float GetRainVolume() { return rain_volume; }

    float GetWorldWetness() const;
    float GetCurrViewEntityWetness() const;
    void UpdateTimer(RainTimer& timer, States state, bool bNeedRayPick);
    void SetTimerNullptr(float rain_timer_value, float last_rain_duration, float rain_drop_time);

};

enum
{
    NO_RAIN,
    IS_RAIN,
};

class ENGINE_API rain_timer_params_new
{
public:
    Fvector		timer;
    bool		not_first_frame;
    float		rain_timestamp;
    float		rain_drop_time_basic;
    float		previous_frame_time;
    float		last_rain_duration;
    float		rain_drop_time;
    float		rain_timer;

    rain_timer_params_new() :not_first_frame(FALSE), rain_timestamp(0), rain_drop_time_basic(20.0), previous_frame_time(0), last_rain_duration(0), rain_drop_time(0), rain_timer(0) { timer.set(0.0, 0.0, 0.0); };
    IC void SetDropTime(float time)
    {
        rain_drop_time_basic = time;
    }
    int Update_new(BOOL state, bool need_raypick);
    BOOL RayPick_new(const Fvector& s, const Fvector& d, float& range, collide::rq_target tgt);
};

ENGINE_API extern rain_timer_params_new* rain_timers_new;
ENGINE_API extern Fvector4* rain_params_new;