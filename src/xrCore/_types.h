#pragma once
#include <cstdint>
#include <limits>
#include <atomic>

// Type defs
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed __int64 s64;
typedef unsigned __int64 u64;

typedef float f32;
typedef double f64;

// Type limits
#define type_max(T) (std::numeric_limits<T>::max())
#define type_min(T) (-std::numeric_limits<T>::max())
#define type_zero(T) (std::numeric_limits<T>::min())
#define type_epsilon(T) (std::numeric_limits<T>::epsilon())

#define int_max type_max(int)
#define int_min type_min(int)
#define int_zero type_zero(int)

#define flt_max type_max(float)
#define flt_min type_min(float)

#undef FLT_MAX
#undef FLT_MIN

#define FLT_MAX flt_max
#define FLT_MIN flt_min

#define flt_zero type_zero(float)
#define flt_eps type_epsilon(float)

#define dbl_max type_max(double)
#define dbl_min type_min(double)
#define dbl_zero type_zero(double)
#define dbl_eps type_epsilon(double)

// Type string
constexpr int max_path = 260;

typedef char string16[16];
typedef char string32[32];
typedef char string64[64];
typedef char string128[128];
typedef char string256[256];
typedef char string512[512];
typedef char string1024[1024];
typedef char string2048[2048];
typedef char string4096[4096];
typedef char string8192[8192];
typedef	char string16384[16384];
typedef char string_path[2 * max_path];

typedef wchar_t wstring16[16];
typedef wchar_t wstring32[32];
typedef wchar_t wstring64[64];
typedef wchar_t wstring128[128];
typedef wchar_t wstring256[256];
typedef wchar_t wstring512[512];
typedef wchar_t wstring1024[1024];
typedef wchar_t wstring2048[2048];
typedef wchar_t wstring4096[4096];
typedef wchar_t wstring8192[8192];
typedef wchar_t wstring16384[16384];
typedef wchar_t wstring_path[2 * max_path];

// Type atomic
using xr_atomic_u8 = std::atomic<u8>;
using xr_atomic_u16 = std::atomic<u16>;
using xr_atomic_u32 = std::atomic<u32>;
using xr_atomic_u64 = std::atomic<u64>;

using xr_atomic_s8 = std::atomic<s8>;
using xr_atomic_s16 = std::atomic<s16>;
using xr_atomic_s32 = std::atomic<s32>;
using xr_atomic_s64 = std::atomic<s64>;
