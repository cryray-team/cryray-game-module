#pragma once
#include "ExternalCallbackTypes.h"
__interface __declspec(dllimport) ISpectreCoreServer;
__interface __declspec(dllimport) ISpectreEngineLib;

class ENGINE_API SpectreEngineClient
{
public:
	static void Initialize();
	static void Shutdown();
	static DWORD CreateProxyObject(DLL_Pure* pObject);
	static void DestroyProxyObject(DLL_Pure* pObject);

private:

	static SpectreCallback::IClientCallbackDeclaration* GetCallbackDeclForName(char* FuncName);

	static ISpectreCoreServer* CoreAPI;
	static ISpectreEngineLib* EngineLibAPI;

	static HMODULE hManagedLib;
	static HMODULE hGameManagedLib;
};

extern bool gSpectreIsLoaded;