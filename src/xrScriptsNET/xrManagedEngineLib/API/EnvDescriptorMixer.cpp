#include "stdafx.h"
#include "EnvDescriptorMixer.h"

XRay::EnvDescriptorMixer::EnvDescriptorMixer(::System::IntPtr InNativeObject)
	: EnvDescriptor(InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CEnvDescriptorMixer, InNativeObject);
}
