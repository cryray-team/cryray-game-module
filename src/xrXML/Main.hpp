#pragma once
#include "../xrCore/xrCore.h"
#include "../xrCore/_stl_extensions.h"

#ifdef XRXML_EXPORTS
#define XRXML_API __declspec(dllexport)
#else
#define XRXML_API __declspec(dllimport)
#endif