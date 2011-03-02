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
#include "stringId.h"
#include <vector>

#include "hash.h"

#ifdef OS_IOS
#include "soraiOSFileUtility.h"
#endif

namespace sora {
	
	class SoraFolderResourceManager: public SoraResourceManager {
	public:
		SoraFolderResourceManager(): applicationPath(SoraFileUtility::getApplicationPath()+L"/") {
#ifdef OS_IOS
			loadResourcePack(appResourcePath());
#else
			loadResourcePack(DEFAULT_RESOURCE_SEARCH_PATH);
#endif
			loadResourcePack(FONT_PATH);
		}
		~SoraFolderResourceManager() {}
		
		HSORARESOURCE	loadResourcePack	(const SoraWString& folder);
		void			attachResourcePack	(HSORARESOURCE handle);
		void			detachResourcePack  (HSORARESOURCE handle);
		
		void* readResourceFile(const SoraWString& file, ulong32 size);
		void* getResourceFile(const SoraWString& file, ulong32& size);
		ulong32 getResourceFileSize(const SoraWString& file);
		void freeResourceFile(void* p);
		
		SoraWString getName() const		{ return L"FolderRM"; }
		SoraWString getFormat() const	{ return L""; }
		bool isFormatSupported(const SoraWString& format) const { return format.size()==0; }
		
	private:
		struct folderDescription {
			SoraWString folderName;
			ulong32 folderHash;
			folderDescription(const SoraWString& folder): folderName(folder), folderHash(crc32(folder)) {}
		};
		typedef std::vector<folderDescription> FOLDER_CONT;
		FOLDER_CONT folders;
		SoraWString applicationPath;
		
		typedef hash_map</*fileNameHash*/ulong32, /*fileFullPath*/SoraWString> FILE_CACHE;
		FILE_CACHE fileCache;
		
		inline FILE* openFile(const SoraWString& fileName);
	};

	
} // namespace sora

#endif