/*
 *  lZipPacker.cpp
 *  lpack
 *
 *  Created by GriffinBu on 12/8/10.
 *  Copyright 2010 GriffinBu(Project L). All rights reserved.
 *
 */

#include "lZipPacker.h"

#include "ZLIB/zlib.h"
#include <cassert>

void* lZipPacker::getData(FILE* pf, uint32& size, uint32& origSize) {
	uint32 fsize = 0;
	fseek(pf, 0, SEEK_END);
	fsize = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	origSize = fsize;
	
	int ret;
	z_stream strm;
	
	unsigned char in[chunkSize];
	unsigned char out[chunkSize];
	
	char* pout = (char*)malloc(fsize);
	if(!pout) 
		return 0;
	
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, Z_BEST_COMPRESSION);
	if (ret != Z_OK)
		return 0;
	
	uint32 have, flush;
	uint32 totalOut = 0;
    do {
        strm.avail_in = fread(in, 1, chunkSize, pf);
        if(ferror(pf)) {
            (void)deflateEnd(&strm);
            return 0;
        }
        flush = feof(pf) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;
		
		do {
            strm.avail_out = chunkSize;
            strm.next_out = out;
            ret = deflate(&strm, flush); 
            //assert(ret != Z_STREAM_ERROR); 
            have = chunkSize - strm.avail_out;
			
			memcpy(pout, out, have);
			
			assert(have == strm.total_out-totalOut);
			pout += have;
			totalOut += have;
			assert(strm.total_out <= fsize && totalOut <= fsize);
			
        } while (strm.avail_out == 0);
	} while (flush != Z_FINISH);
	
	assert(totalOut == strm.total_out);
	size = strm.total_out;
	pout -= size;
	
	char* prout = (char*)malloc(strm.total_out);
	if(!prout)
		return 0;
	
	printf("origSize=%d, deflatedSize=%d\n", origSize, size);
	
	memcpy(prout, pout, strm.total_out);
	free(pout);
	
	(void)deflateEnd(&strm);	
	return prout;
}

void lZipPacker::setChunkSize(uint32 size) {
	chunkSize = size;
}

void lZipPacker::setCompressLevel(uint32 l) {
	level = l;
}

uint32 lZipPackLoader::getData(void* pDataIn, void* pDataOut, LPACKER_NODE node) {
	int ret;
    unsigned have;
    z_stream strm;
	
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return 0;
	
	unsigned char* pIn = (unsigned char*)pDataIn;
	unsigned char* pOut = (unsigned char*)pDataOut;
	
	uint32 totalIn = 0;
	uint32 totalOut = 0;
    do {
		uint32 ain = node.size-totalIn>chunkSize ? chunkSize : node.size-totalIn;
		strm.avail_in = ain;
        strm.next_in = pIn;
		
        do {
            strm.avail_out = chunkSize;
            strm.next_out = pOut;
            ret = inflate(&strm, Z_NO_FLUSH);
            //assert(ret != Z_STREAM_ERROR);
            switch (ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return 0;
            }
            have = chunkSize - strm.avail_out;
			
			assert(have == strm.total_out-totalOut);
			pOut += have;
			totalOut += have;
			assert(totalOut <= node.origSize);
		} while (strm.avail_out == 0);
		
		pIn += ain;
		totalIn += ain;
		
    } while (ret != Z_STREAM_END);
	
    (void)inflateEnd(&strm);
	
	printf("insize: %d, totalsize: %d, totalIn: %d, totalOut: %d\n", node.size, strm.total_out, totalIn, totalOut);
    return strm.total_out;
}

void lZipPackLoader::setChunkSize(uint32 size) {
	chunkSize = size;
}
