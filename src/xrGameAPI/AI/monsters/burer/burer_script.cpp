#include "stdafx.h"
#include "burer.h"

using namespace luabind;

#pragma optimize("s", on)
void CBurer::script_register(lua_State* L) { module(L)[class_<CBurer, CGameObject>("CBurer").def(constructor<>())]; }

#pragma optimize("s", on)
void CBurer2::script_register(lua_State* L) { module(L)[class_<CBurer2, CGameObject>("CBurer2").def(constructor<>())]; }