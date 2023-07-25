#include "stdafx.h"
#include "pseudo_gigant.h"

using namespace luabind;

#pragma optimize("s", on)
void CPseudoGigant::script_register(lua_State* L)
{
    module(L)[class_<CPseudoGigant, CGameObject>("CPseudoGigant").def(constructor<>())];
}

#pragma optimize("s", on)
void CPseudoGigantJumper::script_register(lua_State* L)
{
    module(L)[class_<CPseudoGigantJumper, CGameObject>("CPseudoGigantJumper").def(constructor<>())];
}