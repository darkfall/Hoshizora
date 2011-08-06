/*
 *  SoraMemoryFile.h
 *  Sora
 *
 *  Created by Griffin Bu on 3/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef _SORA_MEMORY_FILE_H_
#define _SORA_MEMORY_FILE_H_

#include "SoraMemoryBuffer.h"
#include "SoraNamedObject.h"
#include "AutoContainer.h"
#include "RefCounted.h"
#include "SoraCore.h"

namespace sora {
	
	/* memory file with auto data count reference */
	class SORA_API SoraMemoryFile: 
				public SoraMemoryBuffer,
				public SoraNamedObject {
	public:
		SoraMemoryFile(const SoraWString& filename): dataPtr(NULL) { read(filename); }
		SoraMemoryFile(stringId filename): dataPtr(NULL) { read(filename); }
		SoraMemoryFile(): dataPtr(NULL) {}
					
		virtual ~SoraMemoryFile() {
            release();
        }	
					
		void release() {
			if(dataPtr) {
				SORA->freeResourceFile(dataPtr);
				dataPtr = NULL;
			}
		}
					
		void read(const SoraWString& filename) {
			ulong32 size;
			void* pData = SORA->getResourceFile(filename.c_str(), size);
			if(pData && size != 0) {
				memoryFile.set(pData, size);
				setName(str2id(filename));
            }
		}
					
		void read(stringId filename) {
			read(id2strw(filename));
		}
					
		ulong32 size() {
			return memoryFile.size();
		}
                        	
	private:
		struct compareObjectName: public std::binary_function<SoraMemoryFile*, stringId, bool> {
			bool operator() (SoraNamedObject* obj, stringId name) const {
				return name == obj->getName();
			}
		};
		SoraMemoryBuffer memoryFile;
		void* dataPtr;
	};
	
} // namespace sora

#endif

