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

namespace sora {

	/*
	 memory buffer using autoptr
	 */
	class SoraMemoryBuffer {
	public:
		SoraMemoryBuffer();
		SoraMemoryBuffer(void* pData, ulong32 _length);
		
		virtual ~SoraMemoryBuffer();
		
		void set(void* pData, ulong32 _length);
		
		/* alloc a block of memory */
		bool alloc(ulong32 size);
		
		// reduce size to real size, becare that length have different meaning if you have used seek
		void resize();
		
		bool push(void* pdata, ulong32 size);
		
		template<typename T>
		bool push(T t) {
			return push(&t, sizeof(t));
		}
		
		bool writeToFile(const SoraWString& path);

		/* 
		 read a block of memory, size = sizeof(T) 
		 */
		template<typename T>
		T read() {
			if(!valid()) return 0;
			if(currPos == length) return 0;
			
			ulong32 size = sizeof(T);
			if(currPos+size <= length) {
				T t;
				memcpy(&t, (void*)(get()+currPos), size);
				currPos += size;
				return t;
			}
			return 0;
		}

		/* 
		 read a block of memory, size specified
		 the memory would be copied to pv
		 so remember to free
		 if buffersize < size, then the size would be set to the bytes accuraly read
		 */
		bool read(void* pv, ulong32& size);		
		/*
		 read a block of memory from offset 
		 */
		bool read(ulong32 offset, void* pv, ulong32& size);
		
		uint8* get();
		uint8* get(ulong32 offset);
		
		template<typename datatype> datatype& get(ulong32 offset) const;
		bool seek(ulong32 pos);
		
		ulong32 size() const;
		ulong32 realsize() const;
		ulong32 offset() const;
		bool valid() const;
		
		SoraMemoryBuffer& operator=(const SoraMemoryBuffer& rhs);

	private:
		SoraAutoPtr<uint8> apCData;
		ulong32 length;
		ulong32 realSize;
		ulong32 currPos;
	};
	
}

#endif
