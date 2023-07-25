#pragma once 

namespace linestdd
{
	typedef xr_string::size_type StrSize;

	constexpr const char* BEGIN = "%c[";
    constexpr const char* END = "]";

	const StrSize npos = (StrSize)xr_string::npos;
} // namespace linestdd