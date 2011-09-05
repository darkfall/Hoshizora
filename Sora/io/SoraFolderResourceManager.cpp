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
#include "SoraPath.h"
#include "SoraFileStream.h"
#include "SoraDirectoryIterator.h"
#include "SoraLogger.h"

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

	HSORARESOURCE SoraFolderResourceManager::loadResourcePack(const SoraWString& folder) {
		//if(!SoraFileUtility::fileExists(folder)) {
			folders.push_back(folderDescription(folder));
			return (HSORARESOURCE)folders.back().folderHash;
		//}
		return 0;
	}

	void SoraFolderResourceManager::attachResourcePack(HSORARESOURCE handle) {
		// for compability with old version
	}

	void SoraFolderResourceManager::detachResourcePack(HSORARESOURCE handle) {
		for(size_t i=0; i<folders.size(); ++i) {
			if(folders[i].folderHash == handle) {
				folders.erase(folders.begin()+i);
				break;
			}
		}
	}

	SoraWString SoraFolderResourceManager::getFullPath(const SoraWString& fileName) {
		FILE_CACHE::iterator itFile = fileCache.find(str2idnc(fileName));
		if(itFile != fileCache.end()) {
			return itFile->second;
		}

        if(SoraFileUtility::fileExists(fileName))
            return fileName;
		

		for(size_t i=0; i<folders.size(); ++i) {
			SoraWString fullPath = folders[i].folderName + fileName;
			if(SoraFileUtility::fileExists(fullPath))
                return fullPath;
		}
        return SoraWString();
	}

	void* SoraFolderResourceManager::readResourceFile(const SoraWString& file, ulong32 size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file))) {
            uint8* pdata = (uint8*)sora_malloc((ulong32)stream.size()+1);
			if(pdata != NULL) {
                if(stream.read(pdata, size) != size) {
                
                }
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	void* SoraFolderResourceManager::getResourceFile(const SoraWString& file, ulong32& size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file))) {
            uint8* pdata = (uint8*)sora_malloc((ulong32)stream.size()+1);
			if(pdata != NULL) {
                size = stream.read(pdata, stream.size());
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	ulong32 SoraFolderResourceManager::getResourceFileSize(const SoraWString& file) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file))) {
            return (ulong32)stream.size();
        }
        return 0;
	}

	void SoraFolderResourceManager::freeResourceFile(void* p) {
		if(p) {
            sora_free(p);
        }
	}
	
	bool SoraFolderResourceManager::enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder) {
        SoraDirectoryIterator itDir(folder);
        while(!itDir.isEnd()) {
            cont.push_back(itDir.path().toWString());
            ++itDir;
        }
		return true;
	}
	
} // namespace sora
