#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;

namespace XRay
{
	public ref class NativeObject
	{
	public:
		
		static NativeObject^ Create(IntPtr InNativeObject, Type^ InTargetType);

		bool IsValid();
		virtual void OnDestroyed();

		IntPtr GetNativeObject();
		void AddVirtualMethod(String^ MethodName);
		MethodInfo^ GetCachedMethod(UInt32 CRC32);

		BEGIN_DEFINE_EVENT(OnObjectBeingDestroyed)
		END_DEFINE_EVENT(OnObjectBeingDestroyed)

	protected:
		NativeObject(IntPtr InNativeObject);
		Type^ TargetType = nullptr;
		DLL_Pure* pNativeObject;
		Dictionary<UInt32, MethodInfo^> DynamicMethodsTable;
	};
}