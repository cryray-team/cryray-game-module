#include "stdafx.h"
#include "FoodItem.h"

using namespace luabind;

#pragma optimize("s", on)
void CFoodItem::script_register(lua_State* L)
{
    module(L)[class_<CFoodItem, CGameObject>("CFoodItem").def(constructor<>())];
}
