#pragma once

struct ShaderSourceEntry
{
	const char* Source;
	const unsigned int SourceSize;
	const char* Code;
	const unsigned int CodeSize;
};

extern const ShaderSourceEntry shaders[];