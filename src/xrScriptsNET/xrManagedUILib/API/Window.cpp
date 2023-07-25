#include "stdafx.h"
#include "xrUICore/UIWindow.h"
#include "Window.h"

XRay::Window::Window(IntPtr InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CUIWindow, InNativeObject);
}


System::Numerics::Vector2 XRay::Window::Position::get()
{
	Fvector2 wndPos;
	wndPos = pNativeObject->GetWndPos();

	return Vector2(wndPos.x, wndPos.y);
}

void XRay::Window::Position::set(Vector2 InPos)
{
	Fvector2 wndPos;
	wndPos.set(InPos.X, InPos.Y);
	pNativeObject->SetWndPos(wndPos);
}

System::Numerics::Vector2 XRay::Window::Size::get()
{
	Fvector2 wndSize;
	wndSize = pNativeObject->GetWndSize();

	return Vector2(wndSize.x, wndSize.y);
}

void XRay::Window::Size::set(Vector2 InSize)
{
	Fvector2 wndSize;
	wndSize.set(InSize.X, InSize.Y);
	pNativeObject->SetWndSize(wndSize);
}

float XRay::Window::Width::get()
{
	return pNativeObject->GetWidth();
}

void XRay::Window::Width::set(float InWidth)
{
	pNativeObject->SetWidth(InWidth);
}

float XRay::Window::Height::get()
{
	return pNativeObject->GetHeight();
}

void XRay::Window::Height::set(float InHeight)
{
	pNativeObject->SetHeight(InHeight);
}

XRay::WindowAligment XRay::Window::Aligment::get()
{
	EWindowAlignment windowAligment = pNativeObject->GetAlignment();
	return WindowAligment(windowAligment);
}

void XRay::Window::Aligment::set(WindowAligment value)
{
	int eValue = (int)value;
	EWindowAlignment windowAligment = (EWindowAlignment)eValue;
	pNativeObject->SetAlignment(windowAligment);
}
