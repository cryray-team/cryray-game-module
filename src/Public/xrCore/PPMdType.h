/****************************************************************************
 *  This file is part of PPMd project                                       *
 *  Written and distributed to public domain by Dmitry Shkarin 1997,        *
 *  1999-2001                                                               *
 *  Contents: compilation parameters and miscelaneous definitions           *
 *  Comments: system & compiler dependent file                              *
 ****************************************************************************/
#if !defined(_PPMDTYPE_H_)
#define _PPMDTYPE_H_

#include <stdio.h>

#define _WIN32_ENVIRONMENT_
//#define _DOS32_ENVIRONMENT_
//#define _POSIX_ENVIRONMENT_
//#define _UNKNOWN_ENVIRONMENT_
#if defined(_WIN32_ENVIRONMENT_)+defined(_DOS32_ENVIRONMENT_)+defined(_POSIX_ENVIRONMENT_)+defined(_UNKNOWN_ENVIRONMENT_) != 1
#error Only one environment must be defined
#endif /* defined(_WIN32_ENVIRONMENT_)+defined(_DOS32_ENVIRONMENT_)+defined(_POSIX_ENVIRONMENT_)+defined(_UNKNOWN_ENVIRONMENT_) != 1 */

#include <windows.h>

const DWORD PPMdSignature=0x84ACAF8F, Variant='I';
const int MAX_O=16;                         /* maximum allowed model order  */

#define _USE_PREFETCHING                    /* for puzzling mainly          */

#if !defined(_UNKNOWN_ENVIRONMENT_) && !defined(__GNUC__)
#define _FASTCALL __fastcall
#define _STDCALL  __stdcall
#else
#define _FASTCALL
#define _STDCALL
#endif /* !defined(_UNKNOWN_ENVIRONMENT_) && !defined(__GNUC__) */

#if defined(__GNUC__)
#define _PACK_ATTR __attribute__ ((packed))
#else /* "#pragma pack" is used for other compilers */
#define _PACK_ATTR
#endif /* defined(__GNUC__) */

/* PPMd module works with file streams via ...GETC/...PUTC macros only      */
#ifndef VERIFY
# define VERIFY(a)
# define VERIFY2(a,b)
# define VERIFY_is_defined_in_ppmdtype_h
#endif // VERIFY
 typedef unsigned char u8;
 typedef unsigned int u32;
# include "compression_ppmd_stream.h"

#ifdef VERIFY_is_defined_in_ppmdtype_h
# undef VERIFY
# undef VERIFY2
# undef VERIFY_is_defined_in_ppmdtype_h
#endif

#include "compression_ppmd_stream.h"
typedef compression::ppmd::stream _PPMD_FILE;
#define _PPMD_E_GETC(fp)   fp->get_char()
#define _PPMD_E_PUTC(c,fp) fp->put_char((c))
#define _PPMD_D_GETC(fp)   fp->get_char()
#define _PPMD_D_PUTC(c,fp) fp->put_char((c))
#endif /* !defined(_PPMDTYPE_H_) */
