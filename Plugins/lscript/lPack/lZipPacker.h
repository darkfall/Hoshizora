/*
 *  lZipPacker.h
 *  lpack
 *
 *  Created by GriffinBu on 12/8/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

/*
 12-08 2010, rev 1.0 first draft
 */

#ifndef _LZIP_PACKER_H_
#define _LZIP_PACKER_H_

#include "lPacker.h"

#define LZIP_PACKER_MAJOR_VER 1
#define LZIP_PACKER_MINOR_VER 0

class lZipPacker: public lPacker {
public:
	lZipPacker(): chunkSize(16384), level(9) { setVer(LZIP_PACKER_MAJOR_VER, LZIP_PACKER_MINOR_VER); }
	
	void* getData(FILE* pf, uint32& size, uint32& origSize);
	
	void setChunkSize(uint32);
	void setCompressLevel(uint32);
	
private:
	uint32 chunkSize;
	uint32 level;
};

class lZipPackLoader: public lPackLoader {
public:
	lZipPackLoader(): chunkSize(16384){ setVer(LZIP_PACKER_MAJOR_VER, LZIP_PACKER_MINOR_VER); }
	
	uint32 getData(void* pDataIn, void* pDataOut, LPACKER_NODE node);
	
	void setChunkSize(uint32);
	
private:
	uint32 chunkSize;
};

#endif // _LZIP_PACKER_H_
