#include "stdafx.h"
#include "../xrEngine/Console/Console.h"
#include "EngineConsole.h"
using namespace XRay;
CConsole* pConsole = ::Console;

bool XRay::EngineConsole::Check(::System::String^ Command)
{
	string64 NativeString = {};
	ConvertDotNetStringToAscii(Command, NativeString);

	return pConsole->GetBool(NativeString);
}

void XRay::EngineConsole::Execute(::System::String^ Command)
{
	string64 NativeString = {};
	ConvertDotNetStringToAscii(Command, NativeString);

	return pConsole->ExecuteCommand(NativeString, false);
}

void XRay::EngineConsole::Execute(::System::String^ Command, bool bEnable)
{
	string64 NativeString = {};
	ConvertDotNetStringToAscii(Command, NativeString);

	return pConsole->ExecuteCommand(NativeString, bEnable);
}