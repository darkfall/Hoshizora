//////////////////////////////////////////////////////////////////////////////////////
//
// Copyright(C) 2009 BOGY.CN
//
// Author: Î¢ÃîµÄÆ½ºâ
// Mailto: bogy.cn@gmail.com
//
//////////////////////////////////////////////////////////////////////////////////////


#include "PackMemory.h"

#include <memory.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif

namespace BOGY
{
struct zlib_memory_info
{
	voidpf	buff;
	uLong	size;
};

struct zlib_memory_file : public zlib_memory_info
{
	uLong	mode;
	uLong	pos;
};

zlib_memory_info* create_zlib_memory_info(voidpf buff, uLong size)
{
	if (0 == buff || 0 == size)
		return 0;

	zlib_memory_info* zmi = new zlib_memory_info;
	if (0 != zmi)
	{
		memset(zmi, 0, sizeof(zlib_memory_info));
		zmi->buff	= buff;
		zmi->size	= size;
	}
	return zmi;
}

void delete_zlib_memory_info(zlib_memory_info* zmi)
{
	if (0 != zmi)
		delete zmi;
}

zlib_memory_file* create_zlib_memory_file(zlib_memory_info* zmi)
{
	if (0 == zmi)
		return 0;

	zlib_memory_file* zmf = new zlib_memory_file;
	if (0 != zmf)
	{
		memset(zmf, 0, sizeof(zlib_memory_file));
		zmf->buff = zmi->buff;
		zmf->size = zmi->size;
	}
	return zmf;
}

void delete_zlib_memory_file(zlib_memory_file* zmf)
{
	if (0 != zmf)
		delete zmf;
}

voidpf open_file_func(voidpf opaque, const char* filename, int mode)
{
	if (0 == opaque || 0 != filename)
		return 0;

	if (ZLIB_FILEFUNC_MODE_READ != (mode & ZLIB_FILEFUNC_MODE_READ))
		return 0;

	if (ZLIB_FILEFUNC_MODE_WRITE == (mode & ZLIB_FILEFUNC_MODE_WRITE))
		return 0;

	zlib_memory_file* zmf = create_zlib_memory_file((zlib_memory_info*)opaque);
	if (0 != zmf)
	{
		zmf->mode = mode;
		zmf->pos = 0;
	}

	return (voidpf)zmf;
}

uLong read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size)
{
	zlib_memory_file* zmf = (zlib_memory_file*)stream;
	if (0 != zmf)
	{
		if (size > (zmf->size - zmf->pos))
			size = (zmf->size - zmf->pos);

		memcpy(buf, (const char*)zmf->buff + zmf->pos, size);
		zmf->pos += size;
		return size;
	}

	return 0;
}

uLong write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
	return 0;
}

 long tell_file_func(voidpf opaque, voidpf stream)
{
	zlib_memory_file* zmf = (zlib_memory_file*)stream;
	if (0 == zmf)
		return -1;
	return zmf->pos;
}

 long seek_file_func(voidpf opaque, voidpf stream, uLong offset, int origin)
{
	zlib_memory_file* zmf = (zlib_memory_file*)stream;
	if (0 == zmf)
		return -1;

	if (ZLIB_FILEFUNC_SEEK_CUR == origin)
	{
		if ((zmf->pos + offset) < 0)
			return -1;
		zmf->pos = zmf->pos + offset;
	}
	else if (ZLIB_FILEFUNC_SEEK_END == origin)
	{
		if (offset < 0 && (zmf->size + offset) < 0)
			return -1;
		zmf->pos = zmf->size + offset;
	}
	else if (ZLIB_FILEFUNC_SEEK_SET == origin)
	{
		if (offset < 0)
			return -1;
		zmf->pos = offset;
	}
	else
	{
		return -1;
	}

	return 0;
}

int close_file_func(voidpf opaque, voidpf stream)
{
	delete_zlib_memory_file((zlib_memory_file*)stream);
	return 0;
}

int error_file_func(voidpf opaque, voidpf stream)
{
	return 0;
}

unzFile unzOpenFromMemory(voidpf data, uLong size)
{
	unzFile hZipFile = 0;

	zlib_memory_info* zsi = create_zlib_memory_info(data, size);
	if (0 != zsi)
	{
		zlib_filefunc_def zf;
		zf.zopen_file	= open_file_func;
		zf.zread_file	= read_file_func;
		zf.zwrite_file	= write_file_func;
		zf.ztell_file	= tell_file_func;
		zf.zseek_file	= seek_file_func;
		zf.zclose_file	= close_file_func;
		zf.zerror_file	= error_file_func;
		zf.opaque		= (voidpf)zsi;

		hZipFile = unzOpen2(0, &zf);

		delete_zlib_memory_info(zsi);
	}
	return hZipFile;
}

};//namespace BOGY
