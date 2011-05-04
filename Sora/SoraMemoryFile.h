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
		SoraMemoryFile(const SoraWString& filename) { read(filename); }
		SoraMemoryFile(stringId filename) { read(filename); }
		SoraMemoryFile() {}
					
		virtual ~SoraMemoryFile() {}	
					
		void read(const SoraWString& filename) {
			ObjList::iterator itObj = std::find_if(members.begin(), members.end(), std::bind2nd(compareObjectName(), str2id(filename)));
			if(itObj != members.end()) {
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
					
		void read(stringId filename) {
			read(id2strw(filename));
		}
		
		SoraMemoryFile& operator=(const SoraMemoryFile& rhs) {
			memoryFile = rhs.memoryFile;
			setName(rhs.getName());
			return *this;
		}
					
		virtual void unserialize(SoraMemoryBuffer& bufferStream) {
			SoraNamedObject::unserialize(bufferStream);
			read(getName());
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

