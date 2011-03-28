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
 */
#ifdef __GNUC__
#include <ext/hash_map>
using __gnu_cxx::hash_map;
#else
#include <hash_map>
#endif

#ifdef WIN32
using std::hash_map;
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

typedef		signed int		int32;
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
} // namespace sora


#include <string>
typedef std::string SoraString;
typedef std::wstring SoraWString;

#include <cassert>

#ifdef WIN32
#include <Windows.h>
#else
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

#ifdef __APPLE_CC__
#include <Availability.h>
#endif

// we are building a dll
#if defined(WIN32) && defined(SORA_DLL_EXPORT)
#define SORAEXPORT __declspec(dllexport)
#else
#define SORAEXPORT
#endif

#if defined(WIN32)
#define SORACALL __stdcall
#else
#define SORACALL
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

#elif defined(WIN32)
	#define FONT_PATH L"C:/Windows/Fonts"
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

// iphone resolution
// retina display for iPod Touch 4gen or iPhone 4 gen and up
// OS_IPAD for 1024*768
#ifdef OS_IOS

#if (UI_USER_INTERFACE_IDIOM == UIUserInterfaceIdiomPad)
#define OS_IOS_IPAD
#endif

#define _IS_RETINA_DISPLAY() {\
	if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && \
		([UIScreen mainScreen].scale == 2.0)) { \
		return true; \
	} \
	return false; \
}

#ifndef OS_IOS_IPAD

#if _IS_RETINA_DISPLAY
#define SORA_IPHONE_RETINA_DISPLAY_SUPPORTED
#endif

#endif

#ifndef OS_IOS_IPAD
	#ifdef SORA_IPHONE_RETINA_DISPLAY_SUPPORTED
		#define SORA_IOS_WINDOW_WIDTH		640
		#define SORA_IOS_WINDOW_HEIGHT		960
		#define SORA_IOS_WINDOW_WIDTH_ROT	960
		#define SORA_IOS_WINDOW_HEIGHT_ROT	640
	#else
		#define SORA_IOS_WINDOW_WIDTH		320
		#define SORA_IOS_WINDOW_HEIGHT		480
		#define SORA_IOS_WINDOW_WIDTH_ROT	480
		#define SORA_IOS_WINDOW_HEIGHT_ROT	320
	#endif // SORA_IPHONE_RETINA_DISPLAY_SUPPORTED
#else
	#define SORA_IOS_WINDOW_WIDTH		768
	#define SORA_IOS_WINDOW_HEIGHT		1024
	#define SORA_IOS_WINDOW_WIDTH_ROT	1024
	#define SORA_IOS_WINDOW_HEIGHT_ROT	768
#endif // OS_IOS_IPAD

#endif


// android resolution defines
#ifdef OS_ANDROID



#endif

#ifdef OS_WIN32
#define sora_fopenw(path, mode) _wfopen(path.c_str(), TEXT(mode))
#else
#define sora_fopenw(path, mode) fopen(ws2s(path).c_str(), mode)
#endif

#include <stdint.h>

#ifdef OS_WIN32
#define s_int64 __int64
#else
#define s_int64 uint64_t
#endif

typedef enum {
	OS_TYPE_WIN32 = 1,
	OS_TYPE_OSX	  = 2,
	OS_TYPE_LINUX = 3,
	OS_TYPE_IOS = 4,
	OS_TYPE_ANDROID = 5,
	OS_TYPE_OTHER = 6,
} OS_TYPE_ENUM;

static int OS_TYPE() {
#if defined(OS_OSX)
	return OS_TYPE_OSX;
#endif

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
	return OS_TYPE_WIN32;
#endif

#if defined(OS_IOS)
	return OS_TYPE_IOS;
#endif

	return OS_TYPE_LINUX;

	//return OS_TYPE_OTHER;
}

#ifndef _DEBUG
#define _DEBUG
#endif

#endif // _SORA_PLATFORM_H_

