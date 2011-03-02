//////////////////////////////////////////////////////////////////////////////////////
//
// Copyright(C) 2009 BOGY.CN
//
// Author: Î¢ÃîµÄÆ½ºâ
// Mailto: bogy.cn@gmail.com
//
//////////////////////////////////////////////////////////////////////////////////////

//Zlib Example Port to MacOS by Griffin Bu
//Copyright Studio Symphonie & GameMaster & Ring0Dev

#ifndef PACKMEMORY_H_
#define PACKMEMORY_H_

#define NOCRYPT
#include "ZLIB/unzip.h"

namespace BOGY
{

#ifndef voidpf
	typedef void* voidpf;
#endif

//#define ZLIB_FILEFUNC_SEEK_CUR 1
//#define ZLIB_FILEFUNC_SEEK_END 2
//#define ZLIB_FILEFUNC_SEEK_SET 4

unzFile unzOpenFromMemory(voidpf data, uLong size);

};//namespace BOGY

#endif
