#include "stdafx.h"
#include "Torch.h"
#include "../xrGame/Torch.h"

using namespace System;
using XRay::Torch;

Torch::Torch(IntPtr InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CTorch, InNativeObject);
}

bool Torch::IsEnabled::get()
{
	return pNativeObject->torch_active();
}

void Torch::IsEnabled::set(bool value)
{
	pNativeObject->Switch(value);
}