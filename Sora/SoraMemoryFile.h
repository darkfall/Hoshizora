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
#include "SoraCore.h"

namespace sora {
	
	/* memory file with auto data count reference */
	class SoraMemoryFile: 
				public SoraMemoryBuffer,
				public SoraNamedObject,
				public AutoListElement<SoraMemoryFile> {
	public:
		SoraMemoryFile(const SoraWString& filename) {			
			ObjList memoryFiles = getAllMembers();
			ObjList::iterator itObj = std::find_if(memoryFiles.begin(), memoryFiles.end(), std::bind2nd(compareObjectName(), str2id(filename)));
			if(itObj != memoryFiles.end()) {
				*this = **itObj;
				return;
			}
			
			ulong32 size;
			void* pData = SORA->getResourceFile(filename, size);
			if(pData && size != 0) {
				memoryFile.set(pData, size);
				setName(str2id(filename));
			}
		}
					
		virtual ~SoraMemoryFile() {}	
		
		SoraMemoryFile& operator=(const SoraMemoryFile& rhs) {
			memoryFile = rhs.memoryFile;
		}
					
	private:
		struct compareObjectName: public std::binary_function<SoraMemoryFile*, stringId, bool> {
			bool operator() (SoraNamedObject* obj, stringId name) const {
				return name == obj->getName();
			}
		};
		SoraMemoryBuffer memoryFile;
	};
	
} // namespace sora

#endif

