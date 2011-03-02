/*
 *  rfCommon.h
 *  rfMap
 *
 *  Created by GriffinBu on 12/13/10.
 *  Copyright 2010 Project Reflection. All rights reserved.
 *
 */

#ifndef _RF_COMMON_H_
#define _RF_COMMON_H_

#include <stdio.h>

namespace rf {
	typedef unsigned char	uint8;
	typedef unsigned short	uint16;
	typedef unsigned int	uint32;

	template<typename T>
	inline T freadrf(FILE* file, bool& err) {
		T i;
		fread(&i, sizeof(T), 1, file);
		if(ferror(file))
			err = true;
		return i;
	}

	template<typename T>
	inline bool fwriterf(FILE* file, T i) {
		fwrite(&i, sizeof(T), 1, file);
		if(ferror(file))
			return true;
		return false;
	}

	inline uint32 fgetsize(FILE* file) {
		if(!file) return 0;

		uint32 size;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
		return size;
	}
} // namespace rf

#endif // _RF_COMMON_H_
