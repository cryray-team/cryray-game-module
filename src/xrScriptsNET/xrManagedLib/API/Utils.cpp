#include "stdafx.h"
#include "Utils.h"
#include "Log.h"

::System::Numerics::Vector3 XRay::Utils::FromFvector(System::IntPtr InVec)
{
	Fvector& Vec3 = *((Fvector*)InVec.ToPointer());
	return ::System::Numerics::Vector3(Vec3.x, Vec3.y, Vec3.z);
}

::System::Numerics::Matrix4x4 XRay::Utils::FromXForm(System::IntPtr InXForm)
{
	Fmatrix& XForm = *((Fmatrix*)InXForm.ToPointer());
	return ::System::Numerics::Matrix4x4(XForm._11, XForm._12, XForm._13, XForm._14,
										XForm._21, XForm._22, XForm._23, XForm._24,
										XForm._31, XForm._32, XForm._33, XForm._34,
										XForm._41, XForm._42, XForm._43, XForm._44);
}

System::UInt32 XRay::Utils::CRC32(System::String^ Str)
{
	if (Str->Length > 511)
	{
		XRay::Log::Error("You can't compute crc32 on long strings with that function (more than 511 symbols)");
		return 0;
	}

	string512 AsciiText = {0};
	ConvertDotNetStringToAscii(Str, AsciiText);

	return crc32(AsciiText, Str->Length);
}
