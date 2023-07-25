#pragma once
#include "API/NativeObject.h"

using namespace System;
using namespace System::Collections::Generic;

namespace XRay
{
	private ref class ObjectPool sealed
	{
	public:

		delegate XRay::NativeObject^ ScriptedObjectActivator();
		delegate XRay::NativeObject^ ObjectActivator(IntPtr InNativeObject);

		static UInt32 CreateObject(IntPtr pDllPure);
		static void DestroyObject(IntPtr pDllPure);
		static void DestroyAllObjects();

		static Dictionary < IntPtr, List<XRay::NativeObject^>^ >^ GlobalRegistry;

		// Can be removed later
		static XRay::NativeObject^ GetObjectByHandle(UInt32 Handle);
	private:

		static void ConditionalInitialize();

		static System::Object^ CreateFastObjectActivator(Type^ ClassType);

		static Dictionary < Type^, System::Object^ >^ ConstructorCache;
		static Dictionary < UInt32, XRay::NativeObject^>^ IndexRegistry;
		static UInt32 IndexCounter;
	};
}