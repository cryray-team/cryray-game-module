#include "stdafx.h"
#include "SpectreEngine.h"
#include "API/Log.h"
#include "API/ClassRegistrator.h"
#include <msclr\marshal_cppstd.h>
#define XR_MODULE "[xrScriptsNET]"

using namespace System;
using namespace System::Reflection;

SpectreEngine::SpectreEngine()
{
	ModInstances = gcnew List<Object^>();
	ScriptCompiler = gcnew xrScriptCompiler();
}

void SpectreEngine::Shutdown()
{
	Msg("* Unloading spectre engine...");

	for (int i = 0; i < ModInstances->Count; ++i)
	{
		Object^ ModInstance = ModInstances[i];

		R_ASSERT2(ModInstance, "SpectreEngine: Invalid ModInstance");

		Type^ type = ModInstance->GetType();
		MethodInfo^ OnShutdownInfo = type->GetMethod("OnShutdown");
		if (OnShutdownInfo != nullptr)
		{
			OnShutdownInfo->Invoke(ModInstance, nullptr);
		}
	}
}

SpectreEngine^ SpectreEngine::Instance()
{
	if (!SpectreEngine::gInstance)
	{
		SpectreEngine::gInstance = gcnew SpectreEngine();
	}

	return SpectreEngine::gInstance;
}

void SpectreEngine::xrCoreInit(String^ appName)
{
	Debug.Initialize(false);

	string512 AppNameStr;
	ConvertDotNetStringToAscii(appName, AppNameStr);
	Core.Initialize(AppNameStr, nullptr, false, "fs.ltx");
}

void SpectreEngine::xrCoreInit(String^ appName, String^ fsName)
{
	Debug.Initialize(false);

	string128 fsNameStr = { 0 };
	ConvertDotNetStringToAscii(fsName, fsNameStr);

	string128 AppNameStr = { 0 };
	ConvertDotNetStringToAscii(appName, AppNameStr);

	//msclr::interop::marshal_context Marshal;
	//AppNameStr = (msclr::interop::marshal_as<std::string, System::String^>(fsName)).c_str();
	Core.Initialize(AppNameStr, nullptr, true, fsNameStr);
}

void SpectreEngine::xrEngineInit()
{
	HMODULE hManagedEngineLib = GetModuleHandle("xrManagedEngineLib");
	if (hManagedEngineLib == NULL)
	{
		String^ BinFileName = Assembly::GetCallingAssembly()->Location;
		String^ BinDirectory = System::IO::Path::GetDirectoryName(BinFileName);
		String^ LoadFilename = BinDirectory + "\\xrManagedEngineLib.dll";

		Assembly::LoadFrom(LoadFilename);
	}
	hManagedEngineLib = GetModuleHandle("xrManagedEngineLib");
	R_ASSERT(hManagedEngineLib);

	FARPROC pXrEngineInit = GetProcAddress(hManagedEngineLib, "xrEngineInit");
	R_ASSERT(pXrEngineInit);

	pXrEngineInit();
}

void SpectreEngine::xrRenderInit()
{
}

void SpectreEngine::GameLibInit()
{
	HMODULE hManagedGameLib = GetModuleHandle("xrManagedGameLib");
	if (hManagedGameLib == NULL)
	{
		String^ BinFileName = Assembly::GetCallingAssembly()->Location;
		String^ BinDirectory = System::IO::Path::GetDirectoryName(BinFileName);
		String^ LoadFilename = BinDirectory + "\\xrManagedGameLib.dll";

		Assembly^ GameAssembly = Assembly::LoadFrom(LoadFilename);
		R_ASSERT2(GameAssembly != nullptr, "Can't load xrManagedGameLib.dll");

		// Invoke class registrator
		array<Type^>^ GameTypes = GameAssembly->GetTypes();

		for each (Type^ var in GameTypes)
		{
			array<::System::Object^>^ ClassRegistratorAttribs = var->GetCustomAttributes(XRay::ClassRegistratorDecorator::typeid, true);
			if (ClassRegistratorAttribs->Length > 0)
			{
				MethodInfo^ RegisterTypesMethod = var->GetMethod("RegisterTypes", BindingFlags::Public | BindingFlags::Static);
				if (RegisterTypesMethod != nullptr)
				{
					RegisterTypesMethod->Invoke(nullptr, nullptr);
				}
				else
				{
					XRay::Log::Warning("xrManagedGameLib has GameClassRegistrator, but without RegisterTypes method, which is unusual");
				}
			}
		}
	}
}

void SpectreEngine::CompileScripts()
{
	if (!ScriptCompiler->CompileScripts())
	{
		XRay::Log::Error("[Spectre]: Can't compile scripts");
		return;
	}

	Assembly^ ScriptAssembly = ScriptCompiler->GetAssembly();

	// Create all ModInstances' and invoke 'OnLoad'
	array<Type^>^ Types = ScriptAssembly->GetTypes();

	for (int i = 0; i < Types->Length; i++)
	{
		Type^ type = Types[i];

		if (type->IsClass && type->BaseType != nullptr && type->BaseType->FullName->Equals("XRay.ModInstance"))
		{
			Object^ ModInstanceObj = System::Activator::CreateInstance(type);
			if (ModInstanceObj)
			{
				ModInstances->Add(ModInstanceObj);
				MethodInfo^ OnLoadInfo = type->GetMethod("OnLoad");
				if (OnLoadInfo != nullptr)
				{
					OnLoadInfo->Invoke(ModInstanceObj, nullptr);
				}
			}
		}
	}
}

// Bridge interface for unmanaged libs
struct SpectreCoreServer : ISpectreCoreServer
{
	virtual void CompileScripts() override
	{
		SpectreEngine::Instance()->CompileScripts();
	}

	virtual void LoadGameLib() override
	{
		SpectreEngine::Instance()->GameLibInit();
	}

	void Shutdown() override
	{
		SpectreEngine::Instance()->Shutdown();
	}
};

SpectreCoreServer SpectreAPI;

MANAGED_API ISpectreCoreServer* GetCoreInterface()
{
	return &SpectreAPI;
}