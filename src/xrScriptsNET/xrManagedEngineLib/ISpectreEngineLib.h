#pragma once

#ifdef	MANAGED_ENGINE_BUILD
#define MANAGED_ENGINE_API		__declspec(dllexport)
#else
#define MANAGED_ENGINE_API		__declspec(dllimport)
#endif

struct FuncNode
{
	void* FuncInvokerInst = nullptr;
	char* Name = nullptr;
	FuncNode* NextNode = nullptr;
};

extern MANAGED_ENGINE_API FuncNode* pRootNode;
extern MANAGED_ENGINE_API FuncNode* pCurrentNode;

__interface MANAGED_ENGINE_API ISpectreEngineLib
{
	virtual DWORD CreateProxyObject(DLL_Pure* pObject) = 0;
	virtual void DestroyProxyObject(DLL_Pure* pObject) = 0;
	virtual void OnShutdown() = 0;
	virtual FuncNode* GetFunctionLinkedListStart() = 0;
};

extern "C"
{
	MANAGED_ENGINE_API void xrEngineInit();
	MANAGED_ENGINE_API ISpectreEngineLib* GetEngineInterface();
}