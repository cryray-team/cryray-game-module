#include "stdafx.h"
#include "xrManagedUILib.h"
#include "../xrUICore/UIHelper.h"

XRay::Test::Test()
{
	CUIXml xml;
	xml.Load(CONFIG_PATH, UI_PATH, "ui_mm_loading_screen.xml");

	XML_NODE* pNode = xml.GetRoot()->FirstChild();
	while (pNode)
	{
		XML_NODE* pElement = pNode->ToElement();
		if (pElement)
		{
			Log::Info(gcnew String(pElement->Value()));
			if (strstr(pElement->Value(), "ViewGroup"))
			{
				CUIViewGroup* view = new CUIViewGroup(xml, pElement);
				Log::Info("view group is created");
			}
			else if (strstr(pElement->Value(), "View"))
			{
				CUIView* view = new CUIView(xml, pElement);
				Log::Info("view is created");
			}
		}
		pNode = pNode->NextSibling();
	}
}

XRay::Test::~Test()
{

}

// Да простят меня духи за этот маросс
#define TranslateCodeToNativeAndBack(Type, Delegate) \
	string128 XMLClassNameStr = {}; \
	ConvertDotNetStringToAscii(XMLClassName, XMLClassNameStr); \
	CXml* refNativeXML = (CXml*)(xml->NativeXml.ToPointer()); \
	Type^ NetObject = gcnew Type((::System::IntPtr)UIHelper::Delegate(*refNativeXML, XMLClassNameStr, (CUIWindow*)wnd->EngineWNDClass().ToPointer())); \
	return NetObject

XRay::UIProgressBar^ XRay::UIHelperNET::CreateProgressBar(Xml^ xml, ::System::String^ XMLClassName, Window^ wnd)
{
	TranslateCodeToNativeAndBack(UIProgressBar, CreateProgressBar);
}

XRay::UIStatic^ XRay::UIHelperNET::CreateStatic(Xml^ xml, ::System::String^ XMLClassName, Window^ wnd)
{
	TranslateCodeToNativeAndBack(UIStatic, CreateStatic);
}