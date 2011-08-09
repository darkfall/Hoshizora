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

/*
 gcc std extension
 if gcc version >= 4
 then there is tr1 support
 visual studio 2008 sp1 or above also has tr1 support
 to do with vs version check
 khash also available as an external hash option
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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef _PSP
#include <memory.h>
#else
#include <malloc.h>
#include <memory>
#endif

#include "SoraConfig.h"
#include "SoraMemory.h"

typedef		long			long32;
typedef		unsigned long	ulong32;

typedef		float			float32;

typedef unsigned long       ULONG, ulong;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef unsigned int		UINT;
/*
#if defined(WIN32)
typedef unsigned long long int uint64;
typedef long long int          int64;
#elif (__WORDSIZE == 32)
__extension__
typedef long long int          int64;
__extension__
typedef unsigned long long int uint64;
#elif (__WORDSIZE == 64)
typedef uint64_t uint64;
typedef int64_t  int64;
#endif
*/

#include <stdint.h>

namespace sora {
	typedef ulong32 HSORASPRITE;
	typedef ulong32 HSORATEXTURE;
	typedef ulong32 HGUIWIDGET;
    typedef ulong32 HSORATARGET;
	
	enum {
		SORA_LINE				= 0x0001,
		SORA_TRIANGLES			= 0x0002,
		SORA_TRIANGLES_FAN		= 0x0003,
		SORA_TRIANGLES_STRIP	= 0x0004,
		SORA_QUAD				= 0x0005,
	};
    
    
    
    typedef int32 SoraHandle;
    typedef int32 SoraUniqueId;
} // namespace sora


#include <cassert>

#if defined(SORA_STD_CALL)
#define SORACALL __stdcall
#else
#define SORACALL
#endif

#if defined(_WIN32)
#define strcmpnocase stricmp
#else
#define strcmpnocase strcasecmp
#endif

// apple os ( osx / ios )
#if defined(__APPLE__) || defined(__APPLE_CC__)
    #if !defined(__IPHONE_OS_VERSION_MAX_ALLOWED)
        #define OS_OSX
    #else
        #define OS_IOS
    #endif
// android
#elif defined(__ANDROID__)
	#define OS_ANDROID
// windows
#elif defined(_WIN32) || defined(_MSC_VER)
	#define OS_WIN32
// linux
#elif defined(linux) || defined(__linux)
    #define OS_LINUX
// psp
#elif defined(_PSP)
	#define OS_PSP
#endif

/**
 * Ensure that dll_export is default when building under windows with /MD or /MDd
 *  Unless SORA_STATIC is specified for static library
 **/
#if defined(OS_WIN32) && defined(_DLL)
    #if !defined(SORA_DLL_EXPORT) && !defined(SORA_STATIC) && !defined(SORA_DLL_IMPORT)
        #define SORA_DLL_EXPORT
    #endif
#endif

// we are building a dll
#if defined(OS_WIN32) 
    #if defined(SORA_DLL_EXPORT)
        #define SORA_API __declspec(dllexport)
		#define SORA_EXTERN extern
    #elif defined(SORA_DLL_IMPORT)
        #define SORA_API __declspec(dllimport)
		#define SORA_EXTERN extern
    #else
		#define SORA_API
		#define SORA_EXTERN
	#endif
#else
    #define SORA_API
	#define SORA_EXTERN
#endif

#include "ZLIB/zlib.h"

#ifndef OS_PSP
#include <string>
typedef std::string SoraString;
typedef std::wstring SoraWString;

/*#include "SoraString.h"
namespace sora {
    typedef SoraString SoraWString;
}*/
#define HAS_WSTRING

#else
#include <string>
typedef std::string SoraString;
namespace std {
	typedef std::basic_string<wchar_t> wstring;
	typedef basic_ostringstream<wchar_t> wostringstream;
}
typedef std::wstring SoraWString;
#endif


#ifdef OS_WIN32
#define sora_fopenw(path, mode) _wfopen(path, TEXT(mode))

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


#define sora_assert assert

namespace sora {
    template<bool x> struct STATIC_ASSERTION_FAILURE;
    template<> struct STATIC_ASSERTION_FAILURE<true> { enum {Value = 1}; };
    template<int x> struct static_assert_test{};
}


#define SORA_JOIN(x, y) SORA_DO_JOIN(x, y)
#define SORA_DO_JOIN(x ,y) SORA_DO_JOIN_2(x, y)
#define SORA_DO_JOIN_2(x, y) x##y

#define sora_static_assert(B) \
    typedef ::sora::static_assert_test<\
        sizeof(::sora::STATIC_ASSERTION_FAILURE<(bool)(B)>)>\
            SORA_JOIN(__sora_static_assert_typedef_, __LINE__)



/*
 Thread Independent Variables
 */
#ifdef _MSC_VER
#define ThreadLocal __declspec(thread)
#elif defined(__GNUC__) && !defined(OS_OSX) && !defined(OS_IOS)
#define ThreadLocal __thread
#else
// some platforms such as Mac OS X does not support TLS
// so we might need some other solutions
#define ThreadLocal
#endif

#ifndef __GNUC__
#define snprintf _snprintf
#endif

#ifndef _DEBUG
#define _DEBUG
#endif

/*
 flag for shaders in render system
 disable this to get rid of shader
*/
#define SORA_USE_SHADER

/*
 use RTTI for SoraEvents and so on,
 if disabled, then SoraEvent must give it's unique name for type identification
*/
#if !defined(SORA_DLL_EXPORT) && !defined(SORA_DLL_IMPORT)
#define SORA_USE_RTTI
#endif

/*
 if disabled, all exception would be disabled
*/
#define SORA_USE_EXCEPTION

/*
 if disabled, SoraSimpleFSM would not use a NULL state as initial state
*/
#define SORA_FSM_USE_NULL

/*
 enable this if you are using sora in multithread environment
 currently unimplemented, reverved for future use
*/
//#define SORA_ENABLE_MULTI_THREAD

/*
 // default:
 // 8 bit system channel
 // 24 bit user channel
 
 // if use uint64
 // 16 bit system channel
 // 48 bit user channel
 
 // if use uint16
 // 8 bit system channel
 // 8 bit user channel
*/
typedef uint32 EventChannelType;


/****************
	Below are compile options for plugins
	Depends on the plugins you use
	I put them here for convinience
******************/

/*
	SoraThread Plugin Option
	If SORA_WIN32_PTHREAD have been defined, then SoraThread would try to use external pthread library under Windows
	Otherwise it would use Win32API to archive the goal
	Default: NO
*/
#ifdef OS_WIN32
//#define SORA_WIN32_PTHREAD
#endif



#endif // _SORA_PLATFORM_H_

