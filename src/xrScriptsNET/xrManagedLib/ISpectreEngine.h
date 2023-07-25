#pragma once

#ifdef	MANAGED_BUILD
#define MANAGED_API		__declspec(dllexport)
#else
#define MANAGED_API		__declspec(dllimport)
#endif

// Bridge interface for unmanaged libs
__interface MANAGED_API ISpectreCoreServer
{
	virtual void CompileScripts() = 0;
	virtual void LoadGameLib() = 0;
	virtual void Shutdown() = 0;
};

extern "C"
{
	MANAGED_API ISpectreCoreServer* GetCoreInterface();
}