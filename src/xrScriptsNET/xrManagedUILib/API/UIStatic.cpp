#include "stdafx.h"
#include "UIStatic.h"
#include "../xrUICore/UIStatic.h"

XRay::UIStatic::UIStatic()
{
	pNativeStatic = new CUIStatic();
}

XRay::UIStatic::UIStatic(::System::IntPtr pObject)
{
	pNativeStatic = (CUIStatic*)pObject.ToPointer();
}

XRay::UIStatic::~UIStatic()
{
	delete pNativeStatic;
}

XRay::UILines^ XRay::UIStatic::TextItemControl()
{
	return gcnew UILines(pNativeStatic->TextItemControl());
}

void XRay::UIStatic::InitTexture(::System::String^ Name)
{
	string128 AppNameStr = { 0 };
	ConvertDotNetStringToAscii(Name, AppNameStr);
	pNativeStatic->InitTexture(AppNameStr);
}

void XRay::UIStatic::StretchTexture::set(bool bUse)
{
	pNativeStatic->SetStretchTexture(bUse);
}