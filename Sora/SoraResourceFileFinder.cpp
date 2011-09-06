/*
 *  SoraResourceFileFinder.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraResourceFileFinder.h"
#include "SoraResourceManager.h"

namespace sora {
    
    ulong32 SoraResourceFileFinder::ResourceMemory = 0;
	
	SoraResourceFileFinder::SoraResourceFileFinder() {
	}
	
	SoraResourceFileFinder::~SoraResourceFileFinder() {
	}
	
	void SoraResourceFileFinder::attachResourceManager(SoraResourceManager* rm) {
		mResourceManagers.push_back(SoraAutoPtr<SoraResourceManager>(rm));
	}
	
	void SoraResourceFileFinder::detachResourceManager(const SoraWString& name) {
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if(name.compare(mResourceManagers[i]->getName()) == 0) {
				mResourceManagers.erase(mResourceManagers.begin()+i);
				break;
			}
		}
	}

	void* SoraResourceFileFinder::readResourceFile(const SoraWString& file, ulong32 size) {
		void* data;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((data = mResourceManagers[i]->readResourceFile(file, size)) != NULL) {
                ResourceMemory += size;
                
                AutoPtrType resource(static_cast<uint8*>(data));
                
                ResourceInfo info;
                info.mResource = resource;
                info.mSize = size;
                mResources.insert(std::make_pair((ulong32)data, info));
                
				return data;
            }
		}
		return 0;
	}
	
	void* SoraResourceFileFinder::getResourceFile(const SoraWString& file, ulong32& size) {
		void* data;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((data = mResourceManagers[i]->getResourceFile(file, size)) != NULL) {
                ResourceMemory += size;
                
                AutoPtrType resource(static_cast<uint8*>(data));
                
                ResourceInfo info;
                info.mResource = resource;
                info.mSize = size;
                mResources.insert(std::make_pair((ulong32)data, info));

				return data;
            }
		}
		return 0;
	}
	
	ulong32 SoraResourceFileFinder::getResourceFileSize(const SoraWString& file) { 
		ulong32 size;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((size = mResourceManagers[i]->getResourceFileSize(file)) != 0) {

				return size;
            }
		}
		return 0;
	}
	
	void SoraResourceFileFinder::freeResourceFile(void* p) {
		sora_assert(p);
        AvailableResourceMap::iterator itResource = mResources.find((ulong32)p);
        if(itResource != mResources.end()) {
            ResourceMemory -= itResource->second.mSize;
            mResources.erase(itResource);
        }
	}
	
	ulong32	SoraResourceFileFinder::loadResourcePack(const SoraWString& file) {
		size_t dotPos = file.rfind('.');
		SoraWString fileFormat;
		if(dotPos != SoraWString::npos) {
			fileFormat = file.substr(dotPos+1, file.size());
		}

		for(size_t i=1; i<mResourceManagers.size(); ++i) {
			SoraResourceManager* prm = mResourceManagers[i].get();
				
			ulong32 r = prm->loadResourcePack(file);
			if(r) {
				prm->attachResourcePack(r);
				return r;
			}
		}
		/*
		 mResourceManagers[0] = sorafodlerresoucemanager
		 just add the pack as a folder
		 */
        mResourceManagers[0]->attachResourcePack(mResourceManagers[0]->loadResourcePack(file));
		return 0;
	}
	
	void SoraResourceFileFinder::attachResourcePack(ulong32 file) {
		// simply does nothing
		// for compability with old versions
	}
	
	void SoraResourceFileFinder::detachResourcePack(ulong32 handle) {
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			mResourceManagers[i]->detachResourcePack(handle);
		}
	}
	
	bool SoraResourceFileFinder::enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder) {
		for(size_t i=1; i<mResourceManagers.size(); ++i) {
			SoraResourceManager* prm = mResourceManagers[i].get();
			
			if(prm->enumFiles(cont, folder))
				return true;
		}
		
		return mResourceManagers[0]->enumFiles(cont, folder);
	}
} // namespace sora