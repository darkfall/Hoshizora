/*
 *  SoraFolderResourceManager.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraFolderResourceManager.h"
#include "SoraFileUtility.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {

	HSORARESOURCE SoraFolderResourceManager::loadResourcePack(const SoraWString& folder) {
		//if(!SoraFileUtility::fileExists(folder)) {
			folders.push_back(folderDescription(folder));
			return folders.back().folderHash;
		//}
		return 0;
	}

	void SoraFolderResourceManager::attachResourcePack(HSORARESOURCE handle) {
		// for compatication with old version
	}

	void SoraFolderResourceManager::detachResourcePack(HSORARESOURCE handle) {
		for(size_t i=0; i<folders.size(); ++i) {
			if(folders[i].folderHash == handle) {
				folders.erase(folders.begin()+i);
				break;
			}
		}
	}

	FILE* SoraFolderResourceManager::openFile(const SoraWString& fileName) {
		FILE* pfile = NULL;
		FILE_CACHE::iterator itFile = fileCache.find(str2idnc(fileName));
		if(itFile != fileCache.end()) {
			return sora_fopenw(itFile->second, "rb");
		}

		pfile = fopen(ws2s(fileName).c_str(), "rb");
		if(pfile != NULL)
            return pfile;

		for(size_t i=0; i<folders.size(); ++i) {
			SoraWString fullPath = folders[i].folderName + fileName;
			pfile = sora_fopenw(fullPath, "rb");
			if(pfile) {
				fileCache[str2id(fileName)] = fullPath;
				break;
			}
		}
		return pfile;
	}

	void* SoraFolderResourceManager::readResourceFile(const SoraWString& file, ulong32 size) {
		FILE* pfile = openFile(file);
		if(pfile != NULL) {
			void* pdata = malloc(size);
			if(pdata != NULL) {
				fread(pdata, size, 1, pfile);
				return pdata;
			}
		}
		return 0;
	}

	void* SoraFolderResourceManager::getResourceFile(const SoraWString& file, ulong32& size) {
		FILE* pfile = openFile(file);
		if(pfile != NULL) {
			fseek(pfile, 0, SEEK_END);
			size = ftell(pfile);
			fseek(pfile, 0, SEEK_SET);

			void* pdata = malloc(size);
			if(pdata != NULL) {
				fread(pdata, size, 1, pfile);
				return pdata;
			}
		}
		return 0;
	}

	ulong32 SoraFolderResourceManager::getResourceFileSize(const SoraWString& file) {
		FILE* pfile = openFile(file);
		if(pfile != NULL) {
			ulong32 size;

			fseek(pfile, 0, SEEK_END);
			size = ftell(pfile);
			fseek(pfile, 0, SEEK_SET);

			return size;
		}
		return 0;
	}

	void SoraFolderResourceManager::freeResourceFile(void* p) {
		free(p);
	}

} // namespace sora
