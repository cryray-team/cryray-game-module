#include "stdafx.h"
#include "GameClassRegistrator.h"
#include "API/Actor.h"
#include "API/GameClassIDs.h"

void XRay::GameClassRegistrator::RegisterTypes()
{
    return ClassRegistrator::Register(XRay::Actor::typeid, (unsigned long long)GameClassIDs::Actor2);
}
