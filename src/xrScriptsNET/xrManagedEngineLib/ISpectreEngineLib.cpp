#include "stdafx.h"
#include "ISpectreEngineLib.h"
#include "../xrEngine/Engine.h"
#include "../xrEngine/xr_input.h"
#include "../xrEngine/x_ray.h"
#include "../xrCDB/ISpatial.h"
#include "ObjectPool.h"

MANAGED_ENGINE_API FuncNode* pRootNode = nullptr;
MANAGED_ENGINE_API FuncNode* pCurrentNode = nullptr;

MANAGED_ENGINE_API void xrEngineInit()
{
#if 0
	InitSettings();
	InitInput();
	InitConsole();

	pApp = xr_new<CApplication>();
	g_SpatialSpace = xr_new<ISpatial_DB>();
	g_SpatialSpacePhysic = xr_new<ISpatial_DB>();
	InitSound1();
	InitSound2();
#endif
}

struct SpectreEngineLib : public ISpectreEngineLib
{
	virtual DWORD CreateProxyObject(DLL_Pure* pObject) override
	{
		return XRay::ObjectPool::CreateObject(IntPtr(pObject));
	}

	virtual void DestroyProxyObject(DLL_Pure* pObject) override
	{
		XRay::ObjectPool::DestroyObject(IntPtr(pObject));
	}

	virtual FuncNode* GetFunctionLinkedListStart() override
	{
		return pRootNode;
	}

	virtual void OnShutdown() override
	{
		XRay::ObjectPool::DestroyAllObjects();
	}
};

SpectreEngineLib EngineLib;

MANAGED_ENGINE_API ISpectreEngineLib* GetEngineInterface()
{
	return &EngineLib;
}
