#include "stdafx.h"
#include "bloodsucker.h"
#include "bloodsucker2.h"
#include "bloodsucker3.h"

using namespace luabind;

#pragma optimize("s", on)
void CAI_Bloodsucker::script_register(lua_State* L)
{
    module(L)[class_<CAI_Bloodsucker, CGameObject>("CAI_Bloodsucker")
                  .def(constructor<>())
                  .def("force_visibility_state", &CAI_Bloodsucker::force_visibility_state)];
}

#pragma optimize("s", on)
void CAI_Bloodsucker2::script_register(lua_State* L)
{
    module(L)[class_<CAI_Bloodsucker2, CGameObject>("CAI_Bloodsucker2").def(constructor<>())];
}

#pragma optimize("s", on)
void CAI_Bloodsucker3::script_register(lua_State* L)
{
    module(L)[class_<CAI_Bloodsucker3, CGameObject>("CAI_Bloodsucker3").def(constructor<>())];
}
