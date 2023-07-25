#include "StdAfx.h"
#include "UISpinText.h"
#include "UILines.h"
#include "../string_table.h"

UISpinText::CUISpinText::CUISpinText() : m_curItem(-1) {}

void UISpinText::CUISpinText::AddItem_(const char* item, int id)
{
    SInfo _info;
    _info._orig = item;
    _info._transl = CStringTable().translate(item);
    _info._id = id;

    m_list.push_back(_info);
    if (-1 == m_curItem)
    {
        m_curItem = 0;
        SetItem(m_curItem);
    }
}

void UISpinText::CUISpinText::SetItem(int v)
{
    R_ASSERT(v != -1);
    m_pLines->SetText(m_list[v]._transl.c_str());
}

LPCSTR UISpinText::CUISpinText::GetTokenText()
{
    R_ASSERT(m_curItem != -1);
    return m_list[m_curItem]._orig.c_str();
}

void UISpinText::CUISpinText::SetCurrentOptValue()
{
    CUIOptionsItem::SetCurrentOptValue();

    xr_token* tok = GetOptToken();

    while (tok->name)
    {
        AddItem_(tok->name, tok->id);
        tok++;
    }
    xr_string val = GetOptTokenValue();

    for (u32 i = 0; i < m_list.size(); i++)
        if (val == m_list[i]._orig.c_str())
        {
            m_curItem = i;
            break;
        }

    SetItem(m_curItem);
}

void UISpinText::CUISpinText::SaveBackUpOptValue()
{
    CUIOptionsItem::SaveBackUpOptValue();
    m_opt_backup_value = m_curItem;
}

void UISpinText::CUISpinText::UndoOptValue()
{
    m_curItem = m_opt_backup_value;
    SetItem(m_curItem);
    CUIOptionsItem::UndoOptValue();
}

void UISpinText::CUISpinText::SaveOptValue()
{
    CUIOptionsItem::SaveOptValue();
    SaveOptStringValue(m_list[m_curItem]._orig.c_str());
}

bool UISpinText::CUISpinText::IsChangedOptValue() const { return m_opt_backup_value != m_curItem; }

void UISpinText::CUISpinText::OnBtnUpClick()
{
    if (CanPressUp())
    {
        m_curItem++;
        SetItem(m_curItem);
    }

    CUICustomSpin::OnBtnUpClick();
}

void UISpinText::CUISpinText::OnBtnDownClick()
{
    if (CanPressDown())
    {
        m_curItem--;
        SetItem(m_curItem);
    }

    CUICustomSpin::OnBtnDownClick();
}

bool UISpinText::CUISpinText::CanPressUp() { return m_curItem < (int)m_list.size() - 1; }

bool UISpinText::CUISpinText::CanPressDown() { return m_curItem > 0; }
