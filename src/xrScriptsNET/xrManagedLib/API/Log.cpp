#include "stdafx.h"
#include "Log.h"

using namespace XRay;
#define XR_MODULE "[xrScriptsNET]"

void Log::Info(String^ message)
{
	if (message == nullptr) return;
	pin_ptr<const wchar_t> pinMessage = PtrToStringChars(message);
	Msg("%S", pinMessage);

#ifndef DEBUG
	Console::WriteLine("{0}", message);
#endif
}

void Log::Warning(String^ message)
{
	if (message == nullptr) return;
	pin_ptr<const wchar_t> pinMessage = PtrToStringChars(message);
	Msg("* %S", pinMessage);

#ifndef DEBUG
	Console::WriteLine("* {0}", message);
#endif
}

void Log::Error(String^ message)
{
	if (message == nullptr) return;
	pin_ptr<const wchar_t> pinMessage = PtrToStringChars(message);
	Msg("! %S", pinMessage);

#ifndef DEBUG
	Console::WriteLine("! {0}", message);
#endif
}

void XRay::Log::Break() { DebugBreak(); }
