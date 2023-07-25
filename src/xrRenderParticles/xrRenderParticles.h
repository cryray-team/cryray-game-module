#pragma once

// Forward references
class	ENGINE_API	CBoneInstance;
class	light;
class	ENGINE_API CRenderDevice;

#pragma pack(push,8)
struct xrDispatchTable
{

};
#pragma pack(pop)

// Binder
// NOTE: Engine calls function named "_xrBindPSGP"
typedef void	__cdecl	xrBinder(xrDispatchTable* T);
