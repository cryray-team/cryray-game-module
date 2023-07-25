#pragma once
#include "eatable_item_object.h"

class CBandage : public CEatableItemObject
{
public:
    CBandage();
    virtual ~CBandage();

#ifdef ANOMALY
    DECLARE_SCRIPT_REGISTER_FUNCTION
#endif
};

#ifdef ANOMALY
add_to_type_list(CBandage)
#undef script_type_list
#define script_type_list save_type_list(CBandage)
#endif