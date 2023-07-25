////////////////////////////////////////////////////////////////////////////
//	Module 		: game_level_cross_table_inline.h
//	Created 	: 20.02.2003
//  Modified 	: 13.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Cross table between game and level graphs IC functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC CGameLevelCrossTable::CGameLevelCrossTable(const void* buffer, const u32& buffer_size)
{
    Memory.mem_copy(&m_tCrossTableHeader, buffer, sizeof(m_tCrossTableHeader));
    buffer = (const u8*)buffer + sizeof(m_tCrossTableHeader);

    R_ASSERT2(m_tCrossTableHeader.version() == XRAI_CURRENT_VERSION, "Cross table version mismatch!");

    m_tpaCrossTable = (CCell*)buffer;
}

IC CGameLevelCrossTable::~CGameLevelCrossTable(){};

IC const CGameLevelCrossTable::CCell& CGameLevelCrossTable::vertex(u32 level_vertex_id) const
{
    VERIFY(level_vertex_id < header().level_vertex_count());
    return (m_tpaCrossTable[level_vertex_id]);
}

IC u32 CGameLevelCrossTable::CHeader::version() const { return (dwVersion); }

IC u32 CGameLevelCrossTable::CHeader::level_vertex_count() const { return (dwNodeCount); }

IC u32 CGameLevelCrossTable::CHeader::game_vertex_count() const { return (dwGraphPointCount); }

IC const xrGUID& CGameLevelCrossTable::CHeader::level_guid() const { return (m_level_guid); }

IC const xrGUID& CGameLevelCrossTable::CHeader::game_guid() const { return (m_game_guid); }

IC GameGraph::_GRAPH_ID CGameLevelCrossTable::CCell::game_vertex_id() const { return (tGraphIndex); }

IC float CGameLevelCrossTable::CCell::distance() const { return (fDistance); }

IC const CGameLevelCrossTable::CHeader& CGameLevelCrossTable::header() const { return (m_tCrossTableHeader); }
