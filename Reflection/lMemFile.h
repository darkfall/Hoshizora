/*
 *  lMemFile.h
 *  lAnm
 *
 *  Created by GriffinBu on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LMEMFILE_H_
#define _LMEMFILE_H_

#include <memory.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif

class lMemFile {
public:
	lMemFile(void* pData, unsigned long _length):
		currPos(0), length(_length) {
			pCData = (unsigned char*)pData;
		}

	~lMemFile() {}

	template<typename T>
	bool read(T* t) {
		unsigned long size = sizeof(T);
		if(currPos+size <= length && t) {
			memcpy(t, pCData, size);
			currPos += size;
			pCData += size;
			return true;
		}
		memset(t, 0, size);
		return false;
	}

	bool read(void* pv, unsigned long size) {
		if(currPos+size <= length && pv) {
			memcpy(pv, pCData, size);
			currPos += size;
			pCData += size;
			return true;
		}
		memset(pv, 0, size);
		return false;
	}

	bool seek(unsigned long pos) {
		if(pos <= length) {
			pCData -= currPos;
			pCData += pos;
			currPos = pos;
			return true;
		}
		return false;
	}

	unsigned long size() const { return length; }
	const void* data() { return (const void*)pCData; }

	bool valid() {
		if(pCData) return true;
		return false;
	}

	void free() {
		if(pCData)
			::free(pCData);
	}

private:
	lMemFile();
	lMemFile(const lMemFile&);
	lMemFile& operator=(lMemFile&);

	unsigned char* pCData;
	unsigned long length;

	unsigned long currPos;
};

#endif
