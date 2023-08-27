#pragma once
#include "../../xrScriptsNET/xrManagedEngineLib/IFunctionInvoker.h"

namespace SpectreCallback
{
	struct IClientCallbackDeclaration
	{
		IClientCallbackDeclaration(const string64 InName);
		string64 FunctionName;
		IClientCallbackDeclaration* pNextNode;

		virtual void InitializeClientCallback(void* pCallbackFunc) = 0;
	};

	extern IClientCallbackDeclaration* pRootNode;

#define DECLARE_CLIENT_CALLBACK_FUNC(FunctionName, ...) \
	extern ENGINE_API IFunctionInvoker<__VA_ARGS__>* FunctionName; \
 	struct FunctionName##CallbackDeclaration : IClientCallbackDeclaration \
 	{ \
		FunctionName##CallbackDeclaration() : IClientCallbackDeclaration(#FunctionName) {} \
		\
		virtual void InitializeClientCallback(void* pCallbackFunc) override; \
	}

#define DEFINE_CLIENT_CALLBACK_FUNC(FunctionName, ...) \
	ENGINE_API IFunctionInvoker<__VA_ARGS__>* FunctionName = nullptr; \
	void FunctionName##CallbackDeclaration::InitializeClientCallback(void* pCallbackFunc) \
	{ \
		FunctionName = reinterpret_cast<IFunctionInvoker<__VA_ARGS__>*>(pCallbackFunc); \
	} \
	FunctionName##CallbackDeclaration FunctionName##Instance;

	DECLARE_CLIENT_CALLBACK_FUNC(shedule_update, u32);
	DECLARE_CLIENT_CALLBACK_FUNC(SecondaryCombine);
}
