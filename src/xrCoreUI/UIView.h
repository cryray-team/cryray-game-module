#pragma once
/*
#include "linker.h"
#include "uiabstract.h"
#include "xrUIXmlParser.h"

class GAME_API CUIView
{
protected:
    bool m_bVisible;
    bool m_bEnabled;
    Fvector2 m_position;
    Fvector2 m_size;
    EWindowAlignment m_alignment = waNone;

    shared_str m_name;

    CUIView* m_parent;

public:
    CUIView(CUIXml& xml, XML_NODE* node);
    virtual ~CUIView();

    IC bool IsVisible() const { return m_bVisible; }

    IC bool IsEnabled() const { return m_bEnabled; }

    const shared_str GetName() const { return m_name; }

    virtual CUIView* GetParent() final { return m_parent; }

    virtual void AssignParent(CUIView* parent) final;

    virtual CUIView* FindViewByName(shared_str name) final;

protected:
    virtual CUIView* FindViewByNameInternal(shared_str name);

    virtual void Draw();
};
*/