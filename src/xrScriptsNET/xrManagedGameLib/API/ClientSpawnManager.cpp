#include "stdafx.h"
#include "ClientSpawnManager.h"
#include "../xrGame/Level.h"
#include "../xrGame/client_spawn_manager.h"

//XRay::ClientSpawnManager::ClientSpawnManager() = default;


void XRay::ClientSpawnManager::Add(::System::UInt16 RequestingId, ::System::UInt16 RequestedId, Functor ^ Funct)
{
	CObject *object = Level().Objects.net_Find(RequestingId);
	if (object) 
	{
		Funct();
		return;
	}

	auto I = pNativeObject->m_registry.find(RequestingId);
	if (I == pNativeObject->m_registry.end())
	{
		Funct();
		return;
	}

	auto J = (*I).second.find(RequestedId);
	if (J == (*I).second.end()) 
	{
		Funct();
		return;
	}
}

void XRay::ClientSpawnManager::Del(::System::UInt16 RequestingId, ::System::UInt16 RequestedId)
{
	pNativeObject->remove(RequestingId, RequestedId);
}

void XRay::ClientSpawnManager::Clear()
{
	pNativeObject->clear();
}
