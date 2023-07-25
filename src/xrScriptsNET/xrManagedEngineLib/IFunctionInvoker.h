#pragma once


template<typename... FunctionsArgs>
class IFunctionInvoker
{
public:
	virtual void Invoke(u32 ObjectHandle, FunctionsArgs... FArgs) = 0;
	string64 Name;
};


template<>
class IFunctionInvoker<void>
{
public:
	virtual void Invoke(u32 ObjectHandle) = 0;
	string64 Name;
};

