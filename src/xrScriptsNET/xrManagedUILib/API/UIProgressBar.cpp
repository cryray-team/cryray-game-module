#include "stdafx.h"
#include "UIProgressBar.h"
#include "../xrUICore/UIProgressBar.h"

XRay::UIProgressBar::UIProgressBar()
{
	pNativeStatic = new CUIProgressBar();
}

XRay::UIProgressBar::UIProgressBar(::System::IntPtr pObject)
{
	pNativeStatic = (CUIProgressBar*)pObject.ToPointer();
}

XRay::UIProgressBar::~UIProgressBar()
{
	delete pNativeStatic;
}