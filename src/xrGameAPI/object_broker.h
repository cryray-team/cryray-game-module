////////////////////////////////////////////////////////////////////////////
//	Module 		: object_broker.h
//	Created 	: 21.01.2003
//  Modified 	: 12.05.2004
//	Author		: Dmitriy Iassenev
//	Description : Object broker
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <queue>

#include "object_interfaces.h"
#include "object_type_traits.h"
#include "object_comparer.h"
#include "object_cloner.h"
#include "object_destroyer.h"
#include "object_loader.h"
#include "object_saver.h"

namespace /* For Unity build */
{
	static IC bool check(const u8& mask, const u8& test)
	{
		return							(!!(mask & test));
	}
};