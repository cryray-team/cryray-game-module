#include "stdafx.h"
#include "chimera.h"

using namespace luabind;

#pragma optimize("s", on)
void CChimera::script_register(lua_State* L)
{
    module(L)[class_<CChimera, CGameObject>("CChimera").def(constructor<>())];
}

#pragma optimize("s", on)
void CChimera2::script_register(lua_State* L)
{
    module(L)[class_<CChimera2, CGameObject>("CChimera2").def(constructor<>())];
}