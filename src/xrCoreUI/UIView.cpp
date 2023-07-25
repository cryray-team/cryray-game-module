#include "stdafx.h"
/*
#include "UIView.h"
#include "ui_base.h"

CUIView::CUIView(CUIXml& xml, XML_NODE* node)
{
    m_bVisible = xml.ReadAttribBool(node, "visibility");
    m_bEnabled = xml.ReadAttribBool(node, "enabled");
    m_position.x = xml.ReadAttribFlt(node, "x");
    m_position.y = xml.ReadAttribFlt(node, "y");
    m_size.x = xml.ReadAttribFlt(node, "width");
    m_size.y = xml.ReadAttribFlt(node, "height");

    m_name = xml.ReadAttrib(node, "name", nullptr);
}

CUIView::~CUIView() {}

void CUIView::AssignParent(CUIView* parent)
{
    if (!m_parent)
    {
        m_parent = parent;
    }
    else if (!parent)
    {
        m_parent = nullptr;
    }
}

CUIView* CUIView::FindViewByName(shared_str name)
{
    R_ASSERT(name);
    if (!name)
        return nullptr;

    return FindViewByNameInternal(name);
}

CUIView* CUIView::FindViewByNameInternal(shared_str name)
{
    if (name != nullptr && name == m_name)
    {
        return this;
    }
    return nullptr;
}

void CUIView::Draw() {}
*/