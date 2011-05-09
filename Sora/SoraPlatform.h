/*
 *  SoraPlatform.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 Griffin Bu(Project HoshiNoSora). All rights reserved.
 *
 */

#ifndef _SORA_PLATFORM_H_
#define _SORA_PLATFORM_H_

#ifdef WIN32
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#endif

/*
 gcc std extension
 if gcc version >= 4
 then there is tr1 support
 visual studio 2008 sp1 or above also has tr1 support
 to do with vs version check
 khash also available as an external kash option
 */


#ifdef __GNUC__
	#if __GNUC__ >= 4
		#include <tr1/unordered_map>
namespace sora {
	
		using std::tr1::unordered_map;
		#define hash_map unordered_map
}
	#else
		#include <ext/hash_map>
namespace sora {

		using __gnu_cxx::hash_map;
}
	#endif
#else
	#include <hash_map>
#endif

#ifdef WIN32
namespace sora {

using std::hash_map;

}
#endif

#ifdef USE_KMALLOC
	#include "support/kalloc.h"
	#define mymalloc kmalloc
	#define myfree kfree
	#define myrealloc krealloc
#else
	#define mymalloc malloc
	#define myfree free
	#define myrealloc realloc
#endif

#ifdef USE_TR1_SHARED_PTR

#include <tr1/memory>
#define SoraSharedPtr std::tr1::shared_ptr

#else
#define SoraSharedPtr SoraAutoPtr
#endif


#ifndef __GNUC__

static inline int lrint (double const x) { // Round to nearest integer
    int n;
#if defined(__unix__) || defined(__GNUC__)
    // 32-bit Linux, Gnu/AT&T syntax:
    __asm ("fldl %1 \n fistpl %0 " : "=m"(n) : "m"(x) : "memory" );
#else
    // 32-bit Windows, Intel/MASM syntax:
    __asm fld qword ptr x;
    __asm fistp dword ptr n;
#endif
    return n;
}

#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>

#define PLATFORM_32

#ifdef PLATFORM_32

typedef	    int				int32;
typedef		unsigned int	uint32;
typedef		signed short	int16;
typedef		unsigned short	uint16;
typedef		signed char		int8;
typedef		unsigned char	uint8;

typedef		float			float32;
typedef		double			double32;

typedef		long			long32;
typedef		unsigned long	ulong32;

typedef unsigned long       DWORD, ULONG;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned int		UINT;


#endif // PLATFORM_32

namespace sora {
	typedef ulong32 HSORASPRITE;
	typedef ulong32 HSORATEXTURE;
	typedef ulong32 HGUIWIDGET;
    typedef ulong32 HSORATARGET;
} // namespace sora


#include <string>
typedef std::string SoraString;
typedef std::wstring SoraWString;

#include <cassert>


#ifdef __APPLE_CC__
#include <Availability.h>
#endif

// we are building a dll
#if defined(WIN32) && defined(SORA_DLL_EXPORT)
#define SORAEXPORT __declspec(dllexport)
#else
#define SORAEXPORT
#endif

#if defined(WIN32) && defined(SORA_STD_CALL)
#define SORACALL __stdcall
#else
#define SORACALL
#endif

#if defined(WIN32)
#define strcmpnocase stricmp
#else
#define strcmpnocase strcasecmp
#endif

#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
	#define FONT_PATH L"/System/Library/Fonts/"
	#define DEFAULT_RESOURCE_SEARCH_PATH L"./"

	#define USE_GLFW_KEYMAP

	#define OS_OSX

#elif defined(__IPHONE_OS_VERSION_MAX_ALLOWED)
	#define FONT_PATH L"/System/Library/Fonts/"
	#define DEFAULT_RESOURCE_SEARCH_PATH L"./"

	#define USE_GLFW_KEYMAP

	#define OS_IOS

#elif defined(__ANDROID__)
	#define FONT_PATH L"./"
	#define DEFAULT_RESOURCE_SEARCH_PATH L"./"
	#define USE_GLFW_KEYMAP

	#define OS_ANDROID

#elif defined(WIN32) || defined(_MSC_VER)
	#define FONT_PATH L"C:/Windows/Fonts/"
	#define DEFAULT_RESOURCE_SEARCH_PATH L"./"

	#define USE_HGE_KEYMAP

	#define OS_WIN32

#elif defined(linux) || defined(__linux)
    // linux defines are just experimental, i'm not sure it's right or not
    #define USE_GLFW_KEYMAP
    #define FONT_PATH L"/usr/share/fonts/truetype"
    #define DEFAULT_RESOURCE_SEARCH_PATH L"./"

    #define OS_LINUX

#endif


// android resolution definitions
#ifdef OS_ANDROID



#endif

#ifdef OS_WIN32
#define sora_fopenw(path, mode) _wfopen(path.c_str(), TEXT(mode))

#include <windows.h>
static void msleep(uint32 msec) {
	Sleep(msec);
}

#else
#define sora_fopenw(path, mode) fopen(ws2s(path).c_str(), mode)
#include <time.h>
#include <errno.h>
// sleep for milliseconds
// because sleep under windows is guaranteed in millisecond precision
// but under *nix is second precision
// so we need high precision sleep udner *nix
// this is guaranteed in milliseconds
// see my http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/ for more information
static void msleep(uint32_t msec) {
	struct timespec timeout0;
	struct timespec timeout1;
	struct timespec* tmp;
	struct timespec* t0 = &timeout0;
	struct timespec* t1 = &timeout1;
	
	t0->tv_sec = msec / 1000;
	t0->tv_nsec = (msec % 1000) * (1000 * 1000);
	
	while ((nanosleep(t0, t1) == (-1)) && (errno == EINTR)) {
		tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
}

#define MB_OK				1
#define MB_OKCANCEL			2
#define MB_ICONERROR		4
#define MB_ICONSTOP			8
#define MB_ICONWARNING		16
#define MB_ICONINFORMATION	32

#define IDOK				1
#define IDCANCEL			2
#define IDABORT				3
#define IDRETRY				4
#define IDYES				6
#define IDNO				7

#endif

#include <stdint.h>

#define BIT_32_64_EXTEND

#if defined(OS_OSX) || defined(OS_IOS) || defined(OS_LINUX)
#undef BIT_32_64_EXTEND
#endif

#ifdef OS_WIN32
#define s_int64 __int64
#else
#define s_int64 uint64_t
#endif

#ifdef BIT_32_64_EXTEND
// 64bit extend
typedef		long long		long64;
typedef		unsigned long long ulong64;
#else
typedef     long long64;
typedef     unsigned long ulong64;
#endif

typedef enum {
	OS_TYPE_WIN32 = 1,
	OS_TYPE_OSX	  = 2,
	OS_TYPE_LINUX = 3,
	OS_TYPE_IOS = 4,
	OS_TYPE_ANDROID = 5,
	OS_TYPE_OTHER = 6,
} OS_TYPE_ENUM;

#ifndef _DEBUG
#define _DEBUG
#endif

#endif // _SORA_PLATFORM_H_

