#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace XRay
{
	public ref class ClassRegistrator sealed
	{
	private:
		static IntPtr currentTarget = IntPtr::Zero;

	public:
		static void Register(Type^ UserClass, UInt64 BaseClassID);

		static Type^ GetTypeForClassId(UInt64 ClassID);

		static IntPtr GetFactoryTarget();
		static void SetFactoryTarget(IntPtr InNativeObject);
		static void ResetFactoryTarget();
	private:

		static void ConditionalInitialize();

		void LoadBaseClassDefinitions();
		Dictionary<UInt64, Type^> UserDefinitions;
		Dictionary<UInt64, Type^> BaseDefinitions;

		static ClassRegistrator^ sInstance;
	};
}
