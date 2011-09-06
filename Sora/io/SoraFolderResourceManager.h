/*
 *  SoraFolderResourceManager.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_FOLDER_RESOURCE_MANAGER_H_
#define SORA_FOLDER_RESOURCE_MANAGER_H_

#include "SoraResourceManager.h"
#include "SoraFileUtility.h"
#include "SoraStringId.h"
#include "SoraLogger.h"
#include "SoraPath.h"
#include "util/SoraHash.h"
#include <map>

namespace sora {
	
	class SORA_API SoraFolderResourceManager: public SoraResourceManager {
	public:
		SoraFolderResourceManager();
		~SoraFolderResourceManager() {}
		
		SoraResourceHandle	loadResourcePack        (const util::String& folder);
		void                attachResourcePack      (SoraResourceHandle handle);
		void                detachResourcePack      (SoraResourceHandle handle);
		
		void* readResourceFile(const util::String& file, ulong32 size);
		void* getResourceFile(const util::String& file, ulong32& size);
		ulong32 getResourceFileSize(const util::String& file);
		void freeResourceFile(void* p);
		
		util::String getName() const		{ return L"FolderRM"; }
		util::String getFormat() const	{ return L""; }
		bool isFormatSupported(const util::String& format) const { return format.size()==0; }
		
		bool enumFiles(std::vector<util::String>& cont, const util::String& folder);
	private:
		struct folderDescription {
			SoraPath folderName;
			SoraStringId folderHash;
			folderDescription(const util::String& folder): folderName(folder), folderHash(crc32(folder)) {
                folderName.makeDirectory();
                printf("asPath: %s\n", folderName.toString().c_str());
            }
		};
		typedef std::vector<folderDescription> FOLDER_CONT;
		FOLDER_CONT folders;
		util::String applicationPath;
		
		typedef std::map</*fileNameHash*/SoraStringId, /*fileFullPath*/util::String> FILE_CACHE;
		FILE_CACHE fileCache;
		
		inline util::String getFullPath(const util::String& fileName);
	};

	
} // namespace sora

#endif