#include "stdafx.h"
#include "UILines.h"
#include "../xrUICore/UILines.h"

XRay::UILines::UILines()
{
	pNativeLines = new CUILines();
}

XRay::UILines::UILines(CUILines* pLines)
{
	pNativeLines = pLines;
}

XRay::UILines::~UILines()
{
	delete pNativeLines;
}
