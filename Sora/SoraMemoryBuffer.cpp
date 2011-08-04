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
	
	SoraMemoryBuffer::SoraMemoryBuffer(): length(0), currPos(0), realSize(0) {}
	SoraMemoryBuffer::SoraMemoryBuffer(void* pData, ulong32 _length):
	currPos(0), length(_length) {
		apCData = (uint8*)pData;
		// to avoid release data
		apCData.assign(apCData);
	}
	
	SoraMemoryBuffer::~SoraMemoryBuffer() {}
	
	void SoraMemoryBuffer::set(void* pData, ulong32 _length) {
		currPos = 0;
		length = _length;
		apCData = (uint8*)pData;
		apCData.assign(apCData);
	}
	
	/* alloc a block of memory */
	bool SoraMemoryBuffer::alloc(ulong32 size) {
		
		/* round size to be divisible by 16 */
		//	size = (size + 15) & uint32(-16);
		uint8* data = new uint8[size];
		if(!data) return false;
		memset(data, 0, length);
		apCData.assign(data);
		length = size;
		seek(0);
		return true;
	}
	
	// reduce size to real size, becare that length have different meaning if you have used seek
	void SoraMemoryBuffer::resize() {
		if(currPos != 0) {
			uint8* tmpData = new uint8[realSize];
			memcpy(tmpData, (void*)(get()), realSize);
			length = realSize;
			
			apCData.assign(tmpData);
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
	
	bool SoraMemoryBuffer::writeToFile(const SoraWString& path) {
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
	
	
	
	/* 
	 read a block of memory, size specified
	 the memory would be copied to pv
	 so remember to free
	 if buffersize < size, then the size would be set to the bytes accuraly read
	 */
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
	
	/*
	 read a block of memory from offset 
	 */
	ulong32 SoraMemoryBuffer::read(ulong32 offset, void* pv, ulong32 size) {
		if(!valid()) return false;
		
		ulong32 tempPos = currPos;
		if(!seek(offset))
			seek(0);
		ulong32 result = read(pv, size);
		seek(tempPos);
		return result;
	}
	
	uint8* SoraMemoryBuffer::get() { return apCData.get(); }
	uint8* SoraMemoryBuffer::get(ulong32 offset) { 
		if(offset > length) offset = 0;
		return (apCData.get()+offset);
	}
	
	template<typename datatype> 
	datatype& SoraMemoryBuffer::get(ulong32 offset) const { 
		if(offset > length+sizeof(datatype)) offset = 0;
		return *(datatype*)(apCData.get()+offset);
	}
	
	bool SoraMemoryBuffer::seek(ulong32 pos) {
		if(pos <= length) {
			currPos = pos;
			return true;
		}
		return false;
	}
	
	ulong32 SoraMemoryBuffer::size() const { return length; }
	ulong32 SoraMemoryBuffer::realsize() const { return realSize; }
	ulong32 SoraMemoryBuffer::offset() const { return currPos; }
	bool SoraMemoryBuffer::valid() const { return apCData.valid(); }
	
	SoraMemoryBuffer& SoraMemoryBuffer::operator=(const SoraMemoryBuffer& rhs) {
		currPos = rhs.currPos;
		length = rhs.length;
		apCData = rhs.apCData;
		return *this;
	}


} // namespace sora