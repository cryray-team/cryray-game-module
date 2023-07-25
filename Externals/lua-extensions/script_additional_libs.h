#pragma once

#include "build_engine_config.h"

#include "lua.hpp"

#include <cstdlib>
#include <cctype>

typedef unsigned long DWORD;
typedef unsigned char BYTE;

void open_additional_libs(lua_State*);