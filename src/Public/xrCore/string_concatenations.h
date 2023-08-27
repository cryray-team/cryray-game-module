#pragma once

LPSTR XRCORE_API strconcat(int dest_sz, char* dest, const char* S1, const char* S2);
LPSTR XRCORE_API strconcat(int dest_sz, char* dest, const char* S1, const char* S2, const char* S3);
LPSTR XRCORE_API strconcat(int dest_sz, char* dest, const char* S1, const char* S2, const char* S3, const char* S4);
LPSTR XRCORE_API strconcat(int dest_sz, char* dest, const char* S1, const char* S2, const char* S3, const char* S4, const char* S5);
LPSTR XRCORE_API strconcat(int dest_sz, char* dest, const char* S1, const char* S2, const char* S3, const char* S4, const char* S5, const char* S6);

// warning: do not comment this macro, as stack overflow check is very light
// (consumes ~1% performance of STRCONCAT macro)
#define STRCONCAT(dest, ...) \
 do { \
 xray::core::detail::string_tupples STRCONCAT_tupples_unique_identifier(__VA_ARGS__); \
 u32 STRCONCAT_buffer_size = STRCONCAT_tupples_unique_identifier.size(); \
 xray::core::detail::check_stack_overflow(STRCONCAT_buffer_size); \
 (dest) = (LPSTR)_alloca(STRCONCAT_buffer_size); \
 STRCONCAT_tupples_unique_identifier.concat (dest); \
  } while (0)

#include "string_concatenations_inline.h"
