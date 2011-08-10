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

#if !defined(__APPLE__)
#include <malloc.h>
#endif

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraStream.h"
#include "uncopyable.h"

namespace sora {

	class SORA_API SoraMemoryBuffer: uncopyable {
	public:
		SoraMemoryBuffer();
		SoraMemoryBuffer(void* pData, ulong32 _length);
		
		virtual ~SoraMemoryBuffer();
        
        void release();
		
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
        
        template<typename T>
        bool read(T* t) {
            if(!valid()) return 0;
			if(currPos == length) return 0;
			
			ulong32 size = sizeof(T);
			if(currPos+size <= length) {
                memcpy(t, (void*)(get()+currPos), size);
                return true;
            }
            return false;
        }

		/* 
		 read a block of memory, size specified
		 the memory would be copied to pv
		 so remember to free
		 if buffersize < size, then the size would be set to the bytes accuraly read
		 */
		ulong32 read(void* pv, ulong32 size);		
		/*
		 read a block of memory from offset 
		 */
		ulong32 read(ulong32 offset, void* pv, ulong32 size);
		
		uint8* get();
		uint8* get(ulong32 offset);
		
		template<typename datatype> 
        datatype& get(ulong32 offset) const { 
            if(offset > length+sizeof(datatype)) offset = 0;
            return *(datatype*)(apCData+offset);
        }
        
		bool seek(ulong32 pos);
		
		ulong32 size() const;
		ulong32 realsize() const;
		ulong32 offset() const;
		bool valid() const;
		        
        template<typename T>
        inline SoraMemoryBuffer& operator << (const T& val) {
            push(&val, sizeof(T));
            return *this;
        }
        
        inline SoraMemoryBuffer& operator << (const std::string& val) {
            char* cval = const_cast<char*>((val+"\0").c_str());
            push(static_cast<void*>(cval), val.size()+1);
            return *this;
        }

        template<typename T>
        inline SoraMemoryBuffer& operator >> (T& val) {
            read(&val, sizeof(T));
            return *this;
        }
        
        inline SoraMemoryBuffer& operator >> (std:: string& val) {
            ulong32 n = currPos;
            const char* data = (const char*)apCData;
            while(n < realSize && data[n] != '\0') {
                ++n;
            }
            if(n > currPos) {
                val.resize(n-currPos);
                memcpy(&val[0], &data[currPos], n-currPos);
            }
            if(n == realSize)
                currPos = n;
            else
                currPos = n + 1;
            return *this;
        }
        
	private:
        
		uint8* apCData;
		ulong32 length;
		ulong32 realSize;
		ulong32 currPos;
        bool mInternal;
	};
	
}

#endif
