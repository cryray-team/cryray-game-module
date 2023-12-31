#pragma once
#include "ui\uistatic.h"
#include "HudSound.h"

class CObject;

enum
{
    flVisObjNotValid = (1 << 0),
    flTargetLocked = (1 << 1),
};

struct SBinocVisibleObj
{
    SBinocVisibleObj(){};
    CObject* m_object;
    CUIStatic m_lt;
    CUIStatic m_lb;
    CUIStatic m_rt;
    CUIStatic m_rb;
    Frect cur_rect;

    u32 m_visible_time;
    Fcolor m_colors[3];
    float m_upd_speed;
    Flags8 m_flags;
    void create_default(u32 color);
    void Draw();
    void Update();

    bool operator<(const SBinocVisibleObj& other) const
    {
        return m_flags.test(flVisObjNotValid) < other.m_flags.test(flVisObjNotValid);
    } // move non-actual to tail
};

class CBinocularsVision
{
    typedef xr_vector<SBinocVisibleObj*> VIS_OBJECTS;
    typedef VIS_OBJECTS::iterator VIS_OBJECTS_IT;
    VIS_OBJECTS m_active_objects;

public:
    CBinocularsVision(const shared_str& sect);
    ~CBinocularsVision();
    void Update();
    void Draw();
    void remove_links(CObject* object);

protected:
    Fcolor m_frame_color;
    Fcolor m_colors[3];
    float m_rotating_speed;
    void Load(const shared_str& section);
    HUD_SOUND_COLLECTION_LAYERED m_sounds;

    u32 m_min_visible_time;
    float m_transparency_threshold;
};
