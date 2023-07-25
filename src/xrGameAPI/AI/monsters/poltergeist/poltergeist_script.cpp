#include "stdafx.h"
#include "poltergeist.h"
#include "poltergeist2.h"

using namespace luabind;

#pragma optimize("s", on)
void CPoltergeist::script_register(lua_State* L)
{
    module(L)[class_<CPoltergeist, CGameObject>("CPoltergeist").def(constructor<>())];
}

#pragma optimize("s", on)
void CPoltergeist2::script_register(lua_State* L)
{
    module(L)[class_<CPoltergeist2, CGameObject>("CPoltergeist2").def(constructor<>())];
}