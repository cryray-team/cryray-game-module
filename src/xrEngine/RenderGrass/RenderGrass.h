#pragma once

#include "perlin.h"
#include "IGame_Persistent.h"

constexpr auto GRASS_BENDERS = 40; //-' ...
constexpr auto MAX_GRASS_DISTANCE = 40.0f; //-' ...

class ENGINE_API RenderGrass
{
public:
    RenderGrass();
    virtual ~RenderGrass();

    enum GrassBenders_Anim
    {
        BENDER_ANIM_EXPLOSION = 0,
        BENDER_ANIM_DEFAULT = 1,
        BENDER_ANIM_WAVY = 2,
        BENDER_ANIM_SUCK = 3,
        BENDER_ANIM_BLOW = 4,
        BENDER_ANIM_PULSE = 5,
    };

    CPerlinNoise1D* PerlinNoise1D;

    void GrassBendersUpdateAnimations();
    void GrassBendersAddExplosion(
        u16 id, Fvector position, Fvector3 dir, float fade, float speed, float intensity, float radius);
    void GrassBendersAddShot(
        u16 id, Fvector position, Fvector3 dir, float fade, float speed, float intensity, float radius);
    void GrassBendersRemoveById(u16 id);
    void GrassBendersRemoveByIndex(u8& idx);
    void GrassBendersUpdate(
        u16 id, u8& data_idx, u32& data_frame, Fvector& position, float radius, float str, bool CheckDistance);
    void GrassBendersReset(u8 idx);
    void GrassBendersSet(u8 idx, u16 id, Fvector position, Fvector3 dir, float fade, float speed, float str,
        float radius, GrassBenders_Anim anim, bool resetTime);
    float GrassBenderToValue(float& current, float go_to, float intensity, bool use_easing);

    struct grass_data
    {
        u8 index;
        s8 anim[GRASS_BENDERS + 1];
        u16 id[GRASS_BENDERS + 1];
        Fvector pos[GRASS_BENDERS + 1];
        Fvector3 dir[GRASS_BENDERS + 1];
        float radius[GRASS_BENDERS + 1];
        float radius_curr[GRASS_BENDERS + 1];
        float str[GRASS_BENDERS + 1];
        float str_target[GRASS_BENDERS + 1];
        float time[GRASS_BENDERS + 1];
        float fade[GRASS_BENDERS + 1];
        float speed[GRASS_BENDERS + 1];
    } grass_shader_data, grass_shader_data_render;

    grass_data GetGrassRenderBenders();

    u32 grass_shader_data_render_frame;
};