#include "stdafx.h"
#include "f1.h"
#include "WeaponAmmo.h"
#include "Medkit.h"
#include "Antirad.h"
#include "FoodItem.h"
#include "BottleItem.h"
#include "ExplosiveItem.h"
#include "InventoryBox.h"

CF1::CF1(void) {}
CF1::~CF1(void) {}

using namespace luabind;

#pragma optimize("s", on)
void CF1::script_register(lua_State* L)
{
    module(L)[class_<CF1, CGameObject>("CF1").def(constructor<>()),
        class_<CExplosiveItem, CGameObject>("CExplosiveItem").def(constructor<>())];
}
