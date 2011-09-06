/*
 *  SoraMemoryBuffer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraMemoryBuffer.h"

namespace sora {
	
	SoraMemoryBuffer::SoraMemoryBuffer(): 
    length(0), 
    currPos(0), 
    realSize(0),
    mInternal(false) {
        
    }
    
	SoraMemoryBuffer::SoraMemoryBuffer(void* pData, ulong32 _length):
	currPos(0), 
    length(_length),
    mInternal(false) {
		apCData = (uint8*)pData;
	}
	
	SoraMemoryBuffer::~SoraMemoryBuffer() {
        if(mInternal && apCData)
            delete apCData;
    }
    
    void SoraMemoryBuffer::release() {
        if(apCData)
            delete apCData;
    }
	
	void SoraMemoryBuffer::set(void* pData, ulong32 _length) {
        if(mInternal && apCData)
            delete apCData;
        
		currPos = 0;
		length = _length;
		apCData = (uint8*)pData;
	}
	
	bool SoraMemoryBuffer::alloc(ulong32 size) {
		/* round size to be divisible by 16 */
		//	size = (size + 15) & uint32(-16);
		uint8* data = new uint8[size];
		if(!data) return false;
		memset(data, 0, length);
		apCData = data;
		length = size;
		seek(0);
        mInternal = true;
		return true;
	}
	
	void SoraMemoryBuffer::resize() {
		if(currPos != 0) {
			uint8* tmpData = new uint8[realSize];
			memcpy(tmpData, (void*)(get()), realSize);
			length = realSize;
			
			apCData = tmpData;
		}
	}
	
	bool SoraMemoryBuffer::push(void* pdata, ulong32 size) {
		if(realSize + size <= length) {
			memcpy((void*)(get()+realSize), pdata, size);
			realSize += size;
			return true;
		} else {
			// buffer not enough, reallocate
			
			// save current data
			if(realSize != 0) {
				uint8* tempData = new uint8[realSize];
				memcpy(tempData, (void*)(get()), realSize);
				
				// rellocate data
				ulong32 prevLength = realSize;
				length = length*2 + 1024;
				alloc(length);
				memcpy((void*)(get()), tempData, prevLength);
				realSize = prevLength;
				
				delete tempData;
				return push(pdata, size);
			} else {
				alloc(size);
				memcpy((void*)(get()), pdata, size);
				realSize = size;
				return true;
			}
		}
		return false;
	}
	
	bool SoraMemoryBuffer::writeToFile(const util::String& path) {
		FILE* pFile = sora_fopenw(path.c_str(), "wb");
		if(pFile) {
			ulong32 wrote = fwrite((void*)(get()), realSize, 1, pFile);
			if(wrote != realSize) {
				fclose(pFile);
				return false;
			}
		}
		fclose(pFile);
		return true;
	}
	
	ulong32 SoraMemoryBuffer::read(void* pv, ulong32 size) {
		if(!valid()) return 0;
		if(currPos == length) return 0;
		
		assert(pv != NULL);
		
		if(currPos+size > length) {
			size = length - currPos;
		}
		memcpy(pv, (void*)(get()+currPos), size);
		currPos += size;
		return size;
	}
	
	ulong32 SoraMemoryBuffer::read(ulong32 offset, void* pv, ulong32 size) {
		if(!valid()) return false;
		
		ulong32 tempPos = currPos;
		if(!seek(offset))
			seek(0);
		ulong32 result = read(pv, size);
		seek(tempPos);
		return result;
	}
	
	uint8* SoraMemoryBuffer::get() { 
        return apCData;
    }
    
	uint8* SoraMemoryBuffer::get(ulong32 offset) { 
		if(offset > length) offset = 0;
		return (apCData+offset);
	}
	
	bool SoraMemoryBuffer::seek(ulong32 pos) {
		if(pos <= length) {
			currPos = pos;
			return true;
		}
		return false;
	}
	
	ulong32 SoraMemoryBuffer::size() const { 
        return length;
    }
    
	ulong32 SoraMemoryBuffer::realsize() const { 
        return realSize;
    }
    
	ulong32 SoraMemoryBuffer::offset() const { 
        return currPos; 
    }
    
	bool SoraMemoryBuffer::valid() const { 
        return apCData != NULL;
    }

} // namespace sora