#include "stdafx.h"
#include "UIOptionsItem.h"
#include "UIOptionsManagerScript.h"

using namespace luabind;

void CUIOptionsManagerScript::SetCurrentValues(const char* group)
{
    CUIOptionsItem::GetOptionsManager()->SetCurrentValues(group);
}

void CUIOptionsManagerScript::SaveBackupValues(const char* group)
{
    CUIOptionsItem::GetOptionsManager()->SaveBackupValues(group);
}

void CUIOptionsManagerScript::SaveValues(const char* group) { CUIOptionsItem::GetOptionsManager()->SaveValues(group); }

void CUIOptionsManagerScript::UndoGroup(const char* group) { CUIOptionsItem::GetOptionsManager()->UndoGroup(group); }

void CUIOptionsManagerScript::OptionsPostAccept() { CUIOptionsItem::GetOptionsManager()->OptionsPostAccept(); }

void CUIOptionsManagerScript::SendMessage2Group(const char* group, const char* message) { CUIOptionsItem::GetOptionsManager()->SendMessage2Group(group, message); }

void CUIOptionsManagerScript::DoLevelRestart() { CUIOptionsItem::GetOptionsManager()->DoLevelRestart(); }

bool CUIOptionsManagerScript::NeedSystemRestart() { return CUIOptionsItem::GetOptionsManager()->NeedSystemRestart(); }

bool CUIOptionsManagerScript::NeedVidRestart() { return CUIOptionsItem::GetOptionsManager()->NeedVidRestart(); }

bool CUIOptionsManagerScript::NeedLevelRestart() { return CUIOptionsItem::GetOptionsManager()->NeedLevelRestart(); }

LPCSTR CUIOptionsManagerScript::GetRendererName(int index)
{
    for (int i = 0; xrAPI.vid_quality_token[i].name; i++)
    {
        if (i == index)
            return xrAPI.vid_quality_token[i].name;
    }
    return "";
}

#pragma optimize("s", on)
void CUIOptionsManagerScript::script_register(lua_State* L)
{
    module(L)[class_<CUIOptionsManagerScript>("COptionsManager")
                  .def(constructor<>())
                  .def("SaveBackupValues", &CUIOptionsManagerScript::SaveBackupValues)
                  .def("SetCurrentValues", &CUIOptionsManagerScript::SetCurrentValues)
                  .def("SaveValues", &CUIOptionsManagerScript::SaveValues)
                  .def("UndoGroup", &CUIOptionsManagerScript::UndoGroup)
                  .def("OptionsPostAccept", &CUIOptionsManagerScript::OptionsPostAccept)
                  .def("SendMessage2Group", &CUIOptionsManagerScript::SendMessage2Group)
                  .def("NeedSystemRestart", &CUIOptionsManagerScript::NeedSystemRestart)
                  .def("NeedVidRestart", &CUIOptionsManagerScript::NeedVidRestart)
                  .def("NeedLevelRestart", &CUIOptionsManagerScript::NeedLevelRestart)
                  .def("DoLevelRestart", &CUIOptionsManagerScript::DoLevelRestart)
                  .def("GetRendererName", &CUIOptionsManagerScript::GetRendererName)];
}
