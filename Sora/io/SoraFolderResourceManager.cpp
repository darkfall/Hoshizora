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
    
    SoraFolderResourceManager::SoraFolderResourceManager(): applicationPath(SoraFileUtility::getApplicationPath()) {
#ifdef OS_IOS
        loadResourcePack(SoraPath::writtable());
        loadResourcePack(SoraPath::resource());
#else
        loadResourcePack(SoraPath::resource());
#endif
        loadResourcePack(SoraPath::font());
#ifdef OS_OSX
        loadResourcePack(L"./");
#endif
    }

	SoraResourceHandle SoraFolderResourceManager::loadResourcePack(const StringType& folder) {
		folders.push_back(folderDescription(folder));
        return (SoraResourceHandle)folders.back().folderHash;
	}

	void SoraFolderResourceManager::attachResourcePack(SoraResourceHandle handle) {
		// for compability with old version
	}

	void SoraFolderResourceManager::detachResourcePack(SoraResourceHandle handle) {
		for(size_t i=0; i<folders.size(); ++i) {
			if(folders[i].folderHash == handle) {
				folders.erase(folders.begin()+i);
				break;
			}
		}
	}

	StringType SoraFolderResourceManager::getFullPath(const StringType& fileName) {
		FILE_CACHE::iterator itFile = fileCache.find(fileName);
		if(itFile != fileCache.end()) {
			return itFile->second;
		}

        if(SoraFileUtility::fileExists(fileName))
            return fileName;
		

		for(size_t i=0; i<folders.size(); ++i) {
			StringType fullPath = folders[i].folderName.toString() + fileName;
			if(SoraFileUtility::fileExists(fullPath))
                return fullPath;
		}
        return StringType();
	}

	void* SoraFolderResourceManager::readResourceFile(const StringType& file, ulong32 size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
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

	void* SoraFolderResourceManager::getResourceFile(const StringType& file, ulong32& size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
            uint8* pdata = (uint8*)sora_malloc((ulong32)stream.size()+1);
			if(pdata != NULL) {
                size = stream.read(pdata, stream.size());
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	ulong32 SoraFolderResourceManager::getResourceFileSize(const StringType& file) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
            return (ulong32)stream.size();
        }
        return 0;
	}

	void SoraFolderResourceManager::freeResourceFile(void* p) {
		if(p) {
            sora_free(p);
        }
	}
	
	bool SoraFolderResourceManager::enumFiles(std::vector<SoraWString>& cont, const StringType& folder) {
        SoraDirectoryIterator itDir(folder.get());
        while(!itDir.isEnd()) {
            cont.push_back(itDir.path().toWString());
            ++itDir;
        }
		return true;
	}
	
} // namespace sora
