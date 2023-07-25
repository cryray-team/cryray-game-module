#pragma once
#include "../xrEngine/Environment.h"
#include "EnvDescriptor.h"

namespace XRay
{
	public ref class  EnvDescriptorMixer : public EnvDescriptor
	{

	internal:
		CEnvDescriptorMixer *pNativeLevel;

	public:
		EnvDescriptorMixer(::System::IntPtr InNativeObject);
	};
}
