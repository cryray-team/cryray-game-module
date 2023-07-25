#include "stdafx.h"
#include "UILayoutInflater.h"

/*
CUILayoutInflater::CUILayoutInflater() {}

CUILayoutInflater::~CUILayoutInflater() {}

CUIView* CUILayoutInflater::Inflate(const char* filename, CUIWindow* root)
{
    return Inflate(filename, root, root != nullptr);
}

CUIView* CUILayoutInflater::Inflate(const char* filename, CUIWindow* root, bool attachToRoot)
{
#ifdef DEBUG
    Msg("Inflating %s", filename);
#endif
    CUIXml xml;
    xml.Load(CONFIG_PATH, UI_PATH, filename);

    return Inflate(xml, root, attachToRoot);
}

CUIView* CUILayoutInflater::Inflate(CUIXml& xml, CUIWindow* root) { return Inflate(xml, root, root != nullptr); }

CUIView* CUILayoutInflater::Inflate(CUIXml& xml, CUIWindow* root, bool attachToRoot) { return nullptr; }
*/