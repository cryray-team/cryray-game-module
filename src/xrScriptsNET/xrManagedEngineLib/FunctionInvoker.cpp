#include "stdafx.h"
#include "FunctionInvoker.h"
#include "ObjectPool.h"

FunctionInvokerVoid::FunctionInvokerVoid(const string64 InFuncName)
{
	memcpy(Name, InFuncName, sizeof(Name));

	// Calc name crc32
	u32 NameLen = xr_strlen(Name);
	R_ASSERT2(NameLen, "Do not create a FunctionInvoker with empty name!");
	NameCRC32 = crc32(&Name[0], NameLen);

	// register this on global function table
	ThisFuncNode.FuncInvokerInst = this;
	ThisFuncNode.Name = Name;

	// Init root node
	if (pRootNode == nullptr)
	{
		pRootNode = &ThisFuncNode;
		pRootNode->NextNode = nullptr;
	}

	// Append (or create) node chain. So, all instances of FunctionInvoker will be available from pRootNode.
	// Basically that's linked list that initialized in module loading time
	if (pCurrentNode == nullptr)
	{
		pCurrentNode = &ThisFuncNode;
		pCurrentNode->NextNode = nullptr;
	}
	else
	{
		pCurrentNode->NextNode = &ThisFuncNode;
		pCurrentNode = &ThisFuncNode;
		pCurrentNode->NextNode = nullptr;
	}
}

void FunctionInvokerVoid::Invoke(u32 ObjectHandle)
{
	MethodInfo^ methodInfo = nullptr;
	XRay::NativeObject^ Object = XRay::ObjectPool::GetObjectByHandle(ObjectHandle);
	if (Object == nullptr) return;

	// Get function name from type param
	methodInfo = Object->GetCachedMethod(NameCRC32);
	if (methodInfo == nullptr)
	{
		//XRay::Log::Warning("Can't invoke a method");
		return;
	}

	methodInfo->Invoke(Object, nullptr);
}

System::Reflection::MethodInfo^ FunctionSearcher::GetObjectAndMethod(u32 ObjectHandle, u32 NameCRC32, System::Object^& OutObject)
{
	XRay::NativeObject^ Object = XRay::ObjectPool::GetObjectByHandle(ObjectHandle);
	if (Object == nullptr) return nullptr;
	OutObject = Object;

	// Get function name from type param
	return Object->GetCachedMethod(NameCRC32);
}

