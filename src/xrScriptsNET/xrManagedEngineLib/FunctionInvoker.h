//Giperion October 2018
//[EUREKA] 3.7
//XRay Oxygen Project
#pragma once
#include "TemplatedStrings.h"
#include "ISpectreEngineLib.h"
#include "IFunctionInvoker.h"

using namespace System::Reflection;

class ConvertToObject
{
public:
	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, int Elem)
	{
		InOutArray[InOutIndex++] = ::System::Int32(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, unsigned int Elem)
	{
		InOutArray[InOutIndex++] = ::System::UInt32(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, float Elem)
	{
		InOutArray[InOutIndex++] = ::System::Single(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, double Elem)
	{
		InOutArray[InOutIndex++] = ::System::Double(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, char Elem)
	{
		InOutArray[InOutIndex++] = ::System::Byte(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, unsigned char Elem)
	{
		InOutArray[InOutIndex++] = ::System::Byte(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, unsigned short Elem)
	{
		InOutArray[InOutIndex++] = ::System::UInt16(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, short Elem)
	{
		InOutArray[InOutIndex++] = ::System::Int16(Elem);
		return 0;
	}

	static int Convert(array<::System::Object^>^ InOutArray, int& InOutIndex, void* Elem)
	{
		InOutArray[InOutIndex++] = ::System::IntPtr(Elem);
		return 0;
	}
};

class FunctionSearcher
{
public:

	static MethodInfo^ GetObjectAndMethod(u32 ObjectHandle, u32 NameCRC32, System::Object^& OutObject);
};

template<typename... FunctionsArgs>
struct FunctionInvoker : public IFunctionInvoker<FunctionsArgs...>
{
	FunctionInvoker(const string64 InFuncName)
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

	virtual void Invoke(u32 ObjectHandle, FunctionsArgs... FArgs) override
	{
		System::Object^ Object = nullptr;
		MethodInfo^ methodInfo = FunctionSearcher::GetObjectAndMethod(ObjectHandle, NameCRC32, Object);
		if (methodInfo == nullptr)
		{
			//XRay::Log::Warning("Can't invoke a method");
			return;
		}

		constexpr int NumberOfArguments = sizeof...(FArgs);

		// Allocate managed array. As far as I know, there is no way to expand variadic template into GC array directly
		array<::System::Object^>^ args = gcnew array<::System::Object ^>(NumberOfArguments);
		int IndexSeq = 0;
		int dummy[] = { ConvertToObject::Convert(args, IndexSeq, FArgs...) };

		methodInfo->Invoke(Object, args);
	}

	u32 NameCRC32;
	FuncNode ThisFuncNode;
};


struct MANAGED_ENGINE_API FunctionInvokerVoid : public IFunctionInvoker<void>
{
	FunctionInvokerVoid(const string64 InFuncName);

	virtual void Invoke(u32 ObjectHandle) override;

	u32 NameCRC32;
	FuncNode ThisFuncNode;
};