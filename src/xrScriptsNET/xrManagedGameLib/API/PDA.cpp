#include "stdafx.h"
#include "API/PDA.h"

XRay::PDA::PDA(IntPtr InNativeObject)
{
	CAST_TO_NATIVE_OBJECT(CPda, InNativeObject);
}
