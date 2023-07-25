////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_ALife_Smartcovers.cpp
//	Created 	: 17.12.2008
//  Modified 	:
//	Author		: Alexander Plichko
//	Description : Server objects smartcovers for ALife simulator
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrServer_Objects_ALife_Smartcovers.h"

#include "character_info.h"

CSE_SmartCover::CSE_SmartCover(LPCSTR section) : CSE_ALifeDynamicObject(section)
{
    m_enter_min_enemy_distance = pSettings->r_float(section, "enter_min_enemy_distance");
    m_exit_min_enemy_distance = pSettings->r_float(section, "exit_min_enemy_distance");
    m_is_combat_cover = pSettings->r_bool(section, "is_combat_cover");
    m_can_fire = m_is_combat_cover ? true : pSettings->r_bool(section, "can_fire");
    m_need_to_reparse_loopholes = true;
}

CSE_SmartCover::~CSE_SmartCover()
{
}

ISE_Shape* CSE_SmartCover::shape() { return (this); }

bool CSE_SmartCover::used_ai_locations() const { return (true); }

bool CSE_SmartCover::can_save() const { return (true); }

bool CSE_SmartCover::can_switch_online() const { return (true); }

bool CSE_SmartCover::can_switch_offline() const { return (false); }

bool CSE_SmartCover::interactive() const { return (false); }

LPCSTR CSE_SmartCover::description() const { return (m_description.c_str()); }

void CSE_SmartCover::set_available_loopholes(luabind::object table) { m_available_loopholes = table; }

void CSE_SmartCover::STATE_Read(NET_Packet& tNetPacket, u16 size)
{
    inherited1::STATE_Read(tNetPacket, size);
    cform_read(tNetPacket);
    tNetPacket.r_stringZ(m_description);
    m_hold_position_time = tNetPacket.r_float();
    if (m_wVersion >= 120)
    {
        m_enter_min_enemy_distance = tNetPacket.r_float();
        m_exit_min_enemy_distance = tNetPacket.r_float();
    }

    if (m_wVersion >= 122)
        m_is_combat_cover = tNetPacket.r_u8();

    if (m_wVersion >= 128)
        m_can_fire = tNetPacket.r_u8();
}

void CSE_SmartCover::STATE_Write(NET_Packet& tNetPacket)
{
    inherited1::STATE_Write(tNetPacket);
    cform_write(tNetPacket);
    tNetPacket.w_stringZ(m_description);
    tNetPacket.w_float(m_hold_position_time);
    tNetPacket.w_float(m_enter_min_enemy_distance);
    tNetPacket.w_float(m_exit_min_enemy_distance);
    tNetPacket.w_u8((u8)m_is_combat_cover);
    tNetPacket.w_u8((u8)m_can_fire);
}

void CSE_SmartCover::UPDATE_Read(NET_Packet& tNetPacket) { inherited1::UPDATE_Read(tNetPacket); }

void CSE_SmartCover::UPDATE_Write(NET_Packet& tNetPacket) { inherited1::UPDATE_Write(tNetPacket); }
#ifndef XRGAME_EXPORTS
void CSE_SmartCover::FillProps(LPCSTR pref, PropItemVec& items)
{
}
#endif // #ifndef XRGAME_EXPORTS
