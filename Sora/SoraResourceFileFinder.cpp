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
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraMutexGuard guard(mThreadMutex);

        mIOThreadPool.stop();
#endif
	}
	
	void SoraResourceFileFinder::attachResourceManager(SoraResourceManager* rm) {
		mResourceManagers.push_back(SoraAutoPtr<SoraResourceManager>(rm));
	}
	
	void SoraResourceFileFinder::detachResourceManager(const StringType& name) {
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if(name.compare(mResourceManagers[i]->getName()) == 0) {
				mResourceManagers.erase(mResourceManagers.begin()+i);
				break;
			}
		}
	}

	void* SoraResourceFileFinder::readResourceFile(const StringType& file, ulong32 size) {
		void* data;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((data = mResourceManagers[i]->readResourceFile(file, size)) != NULL) {
#ifdef SORA_ENABLE_MULTI_THREAD
                SoraMutexGuard guard(mIOMutex);
#endif
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
	
	void* SoraResourceFileFinder::getResourceFile(const StringType& file, ulong32& size) {
		void* data;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((data = mResourceManagers[i]->getResourceFile(file, size)) != NULL) {
#ifdef SORA_ENABLE_MULTI_THREAD
                SoraMutexGuard guard(mIOMutex);
#endif
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
	
	ulong32 SoraResourceFileFinder::getResourceFileSize(const StringType& file) { 
		ulong32 size;
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			if((size = mResourceManagers[i]->getResourceFileSize(file)) != 0) {

				return size;
            }
		}
		return 0;
	}
	
	void SoraResourceFileFinder::freeResourceFile(void* p) {
		if(!p)
            return;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraMutexGuard guard(mIOMutex);
#endif
        
        AvailableResourceMap::iterator itResource = mResources.find((ulong32)p);
        if(itResource != mResources.end()) {
            ResourceMemory -= itResource->second.mSize;
            mResources.erase(itResource);
        }
	}
	
	ulong32	SoraResourceFileFinder::loadResourcePack(const StringType& file) {
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
	
	void SoraResourceFileFinder::detachResourcePack(ulong32 handle) {
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			mResourceManagers[i]->detachResourcePack(handle);
		}
	}
	
	bool SoraResourceFileFinder::enumFiles(std::vector<SoraWString>& cont, const StringType& folder) {
		for(size_t i=1; i<mResourceManagers.size(); ++i) {
			SoraResourceManager* prm = mResourceManagers[i].get();
			
			if(prm->enumFiles(cont, folder))
				return true;
		}
		
		return mResourceManagers[0]->enumFiles(cont, folder);
	}
    
#ifdef SORA_ENABLE_MULTI_THREAD
    
    void SoraResourceFileFinder::ioThreadFunc(void* arg) {
        
        IoThreadParam* param = static_cast<IoThreadParam*>(arg);
        ulong32 size;
        void* pdata = getResourceFile(param->mName, size);
        param->mNotification(pdata, size, param->mUserData);
        
        delete param;
    }
    
    void SoraResourceFileFinder::loadResourceFileAsync(const StringType& file, const AsyncNotification& notification, void* userdata) {
        if(!mIOThreadPool.isRunning()) {
            mIOThreadPool.start(SORA_MAX_IO_THREAD);
        }
        SoraThreadTask task(&SoraResourceFileFinder::ioThreadFunc, this);
        IoThreadParam* param = new IoThreadParam;
        param->mName = file;
        param->mNotification = notification;
        param->mUserData = userdata;
        task.setArg(param);
        
        SoraMutexGuard guard(mThreadMutex);
        mIOThreadPool.run(task);
    }
    
#endif
    
} // namespace sora