////////////////////////////////////////////////////////////////////////////
//	Module 		: saved_game_wrapper.h
//	Created 	: 21.02.2006
//  Modified 	: 21.02.2006
//	Author		: Dmitriy Iassenev
//	Description : saved game wrapper class
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrGameAPI\alife_space.h"
#include "../xrServerEntities/game_graph_space.h"
#include "script_export_space.h"

class CSavedGameWrapper
{
public:
    typedef ALife::_TIME_ID _TIME_ID;
    typedef GameGraph::_LEVEL_ID _LEVEL_ID;

private:
    _TIME_ID m_game_time;
    _LEVEL_ID m_level_id;
    shared_str m_level_name;
    float m_actor_health;

public:
    CSavedGameWrapper(LPCSTR saved_game_name);
    static LPCSTR saved_game_full_name(LPCSTR saved_game_name, string_path& result);
    static bool saved_game_exist(LPCSTR saved_game_name);
    static bool valid_saved_game(IReader& stream);
    static bool valid_saved_game(LPCSTR saved_game_name);
    IC const _TIME_ID& game_time() const;
    IC const _LEVEL_ID& level_id() const;
    IC LPCSTR level_name() const;
    IC const float& actor_health() const;
    DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CSavedGameWrapper)
#undef script_type_list
#define script_type_list save_type_list(CSavedGameWrapper)

#include "saved_game_wrapper_inline.h"
