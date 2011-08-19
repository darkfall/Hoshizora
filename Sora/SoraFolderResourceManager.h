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

#include "hash.h"
#include "osxfilewrapper.h"
#include "win32filewrapper.h"

#include <map>

namespace sora {
	
	class SORA_API SoraFolderResourceManager: public SoraResourceManager {
	public:
		SoraFolderResourceManager();
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
		
		bool enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder);
	private:
		struct folderDescription {
			SoraWString folderName;
			stringId folderHash;
			folderDescription(const SoraWString& folder): folderName(folder), folderHash(crc32(folder)) {}
		};
		typedef std::vector<folderDescription> FOLDER_CONT;
		FOLDER_CONT folders;
		SoraWString applicationPath;
		
		typedef std::map</*fileNameHash*/stringId, /*fileFullPath*/SoraWString> FILE_CACHE;
		FILE_CACHE fileCache;
		
		inline SoraWString getFullPath(const SoraWString& fileName);
	};

	
} // namespace sora

#endif