#pragma once
#include <regex>
#include <string>
#include "PPMdType.h"

#ifdef XRCRYRAY_EXPORTS
#define XRCRYRAY_API __declspec(dllexport)
#else
#define XRCRYRAY_API __declspec(dllimport)
#endif