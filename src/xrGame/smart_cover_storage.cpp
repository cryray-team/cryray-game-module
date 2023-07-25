////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover_storage.cpp
//	Created 	: 16.08.2007
//	Author		: Alexander Dudin
//	Description : Smart cover storage class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "smart_cover_storage.h"
#include "smart_cover_description.h"
#include "../xrGameAPI\object_broker.h"

using smart_cover::cover;
using smart_cover::description;
using smart_cover::storage;

typedef storage::DescriptionPtr DescriptionPtr;

struct id_predicate_storage
{
    shared_str m_id;

public:
    IC id_predicate_storage(shared_str const& id) : m_id(id) {}

    IC bool operator()(::description* const& ptr) const { return (m_id._get() == ptr->table_id()._get()); }
};

DescriptionPtr storage::description(shared_str const& table_id)
{
    collect_garbage();

    Descriptions::iterator found =
        std::find_if(m_descriptions.begin(), m_descriptions.end(), id_predicate_storage(table_id));

    if (found != m_descriptions.end())
        return (*found);

    ::description* description = xr_new<::description>(table_id);
    m_descriptions.push_back(description);
    return (description);
}

storage::~storage()
{
#ifdef DEBUG
    Descriptions::const_iterator I = m_descriptions.begin();
    Descriptions::const_iterator E = m_descriptions.end();
    for (; I != E; ++I)
        VERIFY(!(*I)->m_ref_count);
#endif // DEBUG
    delete_data(m_descriptions);
}

void storage::collect_garbage()
{
    struct garbage
    {
        static IC bool predicate(::description* const& object)
        {
            if (object->m_ref_count)
                return (false);

            if (Device.dwTimeGlobal < object->m_last_time_dec + CryRayStaticValues::time_to_delete_smart_cover)
                return (false);

            ::description* temp = object;
            xr_delete(temp);
            return (true);
        }
    };

    m_descriptions.erase(
        std::remove_if(m_descriptions.begin(), m_descriptions.end(), &garbage::predicate), m_descriptions.end());
}
