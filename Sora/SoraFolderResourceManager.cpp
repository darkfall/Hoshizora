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
#include "SoraPath.h"

namespace sora {
    
    SoraFolderResourceManager::SoraFolderResourceManager(): applicationPath(SoraFileUtility::getApplicationPath()+L"/") {
#ifdef OS_IOS
        loadResourcePack(SoraPath::writtableW());
        loadResourcePack(SoraPath::resourceW());
#else
        loadResourcePack(SoraPath::resourceW());
#endif
        loadResourcePack(SoraPath::fontW());
    }

	HSORARESOURCE SoraFolderResourceManager::loadResourcePack(const wchar_t* folder) {
		//if(!SoraFileUtility::fileExists(folder)) {
			folders.push_back(folderDescription(folder));
			return (HSORARESOURCE)folders.back().folderHash;
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

	FILE* SoraFolderResourceManager::openFile(const wchar_t* fileName) {
		FILE* pfile = NULL;
		FILE_CACHE::iterator itFile = fileCache.find(str2idnc(fileName));
		if(itFile != fileCache.end()) {
			return sora_fopenw(itFile->second.c_str(), "rb");
		}

		pfile = sora_fopenw(fileName.c_str(), "rb");
		if(pfile != NULL)
            return pfile;

		for(size_t i=0; i<folders.size(); ++i) {
			SoraWString fullPath = folders[i].folderName + fileName;
			pfile = sora_fopenw(fullPath.c_str(), "rb");
			if(pfile) {
				fileCache[str2id(fileName)] = fullPath;
				break;
			}
		}
		return pfile;
	}

	void* SoraFolderResourceManager::readResourceFile(const wchar_t* file, ulong32 size) {
		FILE* pfile = openFile(file);
		if(pfile != NULL) {
            uint8* pdata = new uint8[size+1];
			if(pdata != NULL) {
				fread(pdata, size, 1, pfile);
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	void* SoraFolderResourceManager::getResourceFile(const wchar_t* file, ulong32& size) {
		FILE* pfile = openFile(file);
		if(pfile != NULL) {
			fseek(pfile, 0, SEEK_END);
			size = ftell(pfile);
			fseek(pfile, 0, SEEK_SET);

            uint8* pdata = new uint8[size+1];
			if(pdata != NULL) {
				fread(pdata, size, 1, pfile);
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	ulong32 SoraFolderResourceManager::getResourceFileSize(const wchar_t* file) {
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
		if(p) delete (uint8*)p;
	}
	
	bool SoraFolderResourceManager::enumFiles(std::vector<SoraWString>& cont, const wchar_t* folder) {
#ifdef WIN32
		sorawin32::enumFilesInFolder(cont, folder);
#endif
#ifdef OS_OSX
		soraosx::enumFilesInFolder(cont, folder);
#endif
		return true;
	}
	
} // namespace sora
