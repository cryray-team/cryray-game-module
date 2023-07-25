#include "stdafx.h"
#include "boar.h"

using namespace luabind;

#pragma optimize("s", on)
void CAI_Boar::script_register(lua_State* L)
{
    module(L)[class_<CAI_Boar, CGameObject>("CAI_Boar").def(constructor<>())];
}

#pragma optimize("s", on)
void CAI_CBoar2::script_register(lua_State* L)
{
    module(L)[class_<CAI_CBoar2, CGameObject>("CAI_CBoar2").def(constructor<>())];
}