#pragma once
#include "../../xrEngine/Environment.h"


namespace XRay
{
	public ref class EnvDescriptor
	{
	internal:

		CEnvDescriptor *pNativeObject;

	public:
		EnvDescriptor(::System::IntPtr InNativeObject);

		//CurrentEnvironment(MEnvironment self);
	};
}
