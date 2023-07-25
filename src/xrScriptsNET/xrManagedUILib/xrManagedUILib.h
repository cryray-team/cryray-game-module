#pragma once
#include "../xrXML/xrXMLParser.h"
//#include "../xrUICore/xrUIXmlParser.h"
#include "../xrUICore/UIView.h"
#include "../xrUICore/UIViewGroup.h"
#include "../xrUICore/UIViewInflater.h"
#include "../xrUICore/UILayoutInflater.h"
#include "API/Window.h"
#include "API/UIStatic.h"
#include "API/UIProgressBar.h"

using namespace System;

namespace XRay
{
	public ref class Test sealed
	{
	public:
		Test();
		virtual ~Test();
	};

	public ref class UIHelperNET abstract
	{
	public:
		/// <summary>Create CUIProgressBar</summary>
		static UIProgressBar^ CreateProgressBar(Xml^ xml, ::System::String^ XMLClassName, Window^ wnd);
		/// <summary>Create CUIStatic</summary>
		static UIStatic^ CreateStatic(Xml^ xml, ::System::String^ XMLClassName, Window^ wnd);
		//
		///// <summary>Create CUITextWnd</summary>
		//static	CUITextWnd*			CreateTextWnd(CXml& xml, LPCSTR ui_path, CUIWindow* parent);\
		///// <summary>Create CreateFrameLine</summary>
		//static	CUIFrameLineWnd*	CreateFrameLine(CXml& xml, LPCSTR ui_path, CUIWindow* parent);
		///// <summary>Create CreateFrameWindow</summary>
		//static	CUIFrameWindow*		CreateFrameWindow(CXml& xml, LPCSTR ui_path, CUIWindow* parent);
		///// <summary>Create Create3tButton</summary>
		//static	CUI3tButton*		Create3tButton(CXml& xml, LPCSTR ui_path, CUIWindow* parent);
		///// <summary>Create CreateCheck</summary>
		//static	CUICheckButton*		CreateCheck(CXml& xml, LPCSTR ui_path, CUIWindow* parent);
		///// <summary>Create CreateEditBox</summary>
		//static	CUIEditBox*			CreateEditBox(CXml& xml, LPCSTR ui_path, CUIWindow* parent);
		///// <summary>Create CreateHint</summary>
		//static	UIHint*				CreateHint(CXml& xml, LPCSTR ui_path /*, CUIWindow* parent*/);
		//
	};
}