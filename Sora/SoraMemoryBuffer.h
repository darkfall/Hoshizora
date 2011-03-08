/*
 *  SoraMemoryBuffer.h
 *  lAnm
 *
 *  Created by GriffinBu on 3/7/11.
 *  Copyright 2010 Griffin Bu(Project Hoshizora). All rights reserved.
 *
 */

#ifndef _SORA_MEMORY_BUFFER_H_
#define _SORA_MEMORY_BUFFER_H_

#include <memory.h>
#if !defined(__APPLE__)
#include <malloc.h>
#endif

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {

	/*
	 memory buffer using autoptr
	 */
	class SoraMemoryBuffer {
	public:
		SoraMemoryBuffer(): length(0), currPos(0) {}
		SoraMemoryBuffer(void* pData, ulong32 _length):
			currPos(0), length(_length) {
				apCData = (uint8*)pData;
			}

		virtual ~SoraMemoryBuffer() {}
		
		void set(void* pData, ulong32 _length) {
			currPos = 0;
			length = _length;
			apCData = (uint8*)pData;
		}
		
		/* alloc a block of memory */
		bool alloc(ulong32 size) {
			if(valid()) {
				apCData.release();
			}
			/* round size to be divisible by 16 */
			size = (size + 15) & uint32(-16);
			uint8* data = new uint8[size];
			if(!data) return false;
			apCData = data;
			length = size;
			seek(0);
		}
		
		bool push(void* pdata, ulong32 size) {
			if(currPos + size <= length) {
				memcpy((void*)(get()+currPos), pdata, size);
				currPos += size;
				return true;
			}
			return false;
		}

		/* 
		 read a block of memory, size = sizeof(T) 
		 */
		template<typename T>
		bool read(T* t) {
			if(!valid()) return false;
			if(currPos == length) return false;

			ulong32 size = sizeof(T);
			if(currPos+size <= length && t) {
				memcpy(t, (void*)get()+currPos, size);
				currPos += size;
				return true;
			}
			memset(t, 0, size);
			return false;
		}

		/* 
		 read a block of memory, size specified
		 the memory would be copied to pv
		 so remember to free
		 if buffersize < size, then the size would be set to the bytes accuraly read
		 */
		bool read(void* pv, ulong32& size) {
			if(!valid()) return false;
			if(currPos == length) return false;
			
			assert(pv != NULL);
			
			if(currPos+size > length) {
				size = length - currPos;
			}
			memcpy(pv, (void*)(get()+currPos), size);
			currPos += size;
			return true;
		}
		
		/*
		 read a block of memory from offset 
		 */
		bool read(ulong32 offset, void* pv, ulong32& size) {
			if(!valid()) return false;
			
			ulong32 tempPos = currPos;
			if(!seek(offset))
				seek(0);
			bool result = read(pv, size);
			seek(tempPos);
			return result;
		}
		
		uint8* get() { return apCData.pointer(); }
		uint8* get(ulong32 offset) { 
			if(offset > length) offset = 0;
			return (apCData.pointer()+offset);
		}
		template<typename datatype> datatype& get(ulong32 offset) const { 
			if(offset > length+sizeof(datatype)) offset = 0;
			return *(datatype*)(apCData.pointer()+offset);
		}

		bool seek(ulong32 pos) {
			if(pos <= length) {
				currPos = pos;
				return true;
			}
			return false;
		}

		inline ulong32 size() const { return length; }
		inline ulong32 offset() const { return currPos; }
		inline bool valid() { return apCData.valid(); }
		
		SoraMemoryBuffer& operator=(const SoraMemoryBuffer& rhs) {
			currPos = rhs.currPos;
			length = rhs.length;
			apCData = rhs.apCData;
			return *this;
		}

	private:
		SoraAutoPtr<uint8> apCData;
		ulong32 length;

		ulong32 currPos;
	};
	
}

#endif
