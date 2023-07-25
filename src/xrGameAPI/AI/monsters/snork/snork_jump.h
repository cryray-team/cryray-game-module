#pragma once

class CSnork;

class CSnorkJump
{
    CSnork* m_object;

    float m_cur_dist;
    bool m_specific_jump;

    CObject* m_target_object;
    u32 m_velocity_mask;
};
