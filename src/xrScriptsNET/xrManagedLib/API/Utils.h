#pragma once


namespace XRay
{
	public ref class Utils abstract
	{
	public:
		static ::System::Numerics::Vector3 FromFvector(System::IntPtr InVec);
		static ::System::Numerics::Matrix4x4 FromXForm(System::IntPtr InXForm);
		static ::System::UInt32 CRC32(::System::String^ Str);
	};
}

