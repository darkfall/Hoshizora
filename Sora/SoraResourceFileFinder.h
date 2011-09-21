/*
 *  SoraResourceManagerSolver.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_RESOURCE_MANAGER_SOLVER_H_
#define SORA_RESOURCE_MANAGER_SOLVER_H_

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraStringId.h"
#include "SoraRefCounted.h"
#include "SoraFunction.h"
#include <vector>
#include <map>

#ifdef SORA_ENABLE_MULTI_THREAD
#include "thread/SoraThreadPool.h"
#include "thread/SoraMutex.h"
#endif

namespace sora {
	
	class SoraResourceManager;
	
	class SoraResourceFileFinder {
	public:
		SoraResourceFileFinder();
		~SoraResourceFileFinder();
		
		void attachResourceManager(SoraResourceManager* rm);
		void detachResourceManager(const StringType& name);
		
		ulong32	loadResourcePack    (const StringType& file);
		void	detachResourcePack  (ulong32 handle);
		
		void*   readResourceFile			(const StringType& file, ulong32 size);
		void*   getResourceFile				(const StringType& file, ulong32& size);
		ulong32 getResourceFileSize			(const StringType& file);
		void    freeResourceFile(void* p);
		
		bool enumFiles(std::vector<SoraWString>& cont, const StringType& folder);
        
        static ulong32 ResourceMemory;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        typedef SoraFunction<void(void*, ulong32, void*)> AsyncNotification;
        void loadResourceFileAsync(const StringType& file, const AsyncNotification& notification, void* userdata);
    private:
        struct IoThreadParam {
            AsyncNotification mNotification;
            StringType mName;
            void* mUserData;
        };
        void ioThreadFunc(void* arg);
#endif
        
	private:		
		typedef std::vector<SoraAutoPtr<SoraResourceManager> > RESOURCE_MANAGER_CONT;
		RESOURCE_MANAGER_CONT mResourceManagers;
        
        template<class C>
        struct ReleasePolicy {
            static void release(C* ptr) {
                sora_free(static_cast<void*>(ptr));
            }
        };
        
        typedef SoraAutoPtr<uint8, autoptr::RefCounter, ReleasePolicy<uint8> > AutoPtrType;
        struct ResourceInfo {
            AutoPtrType mResource;
            ulong32 mSize;
        };
        typedef std::map<SoraStringId, ResourceInfo> AvailableResourceMap;
        AvailableResourceMap mResources;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraThreadPool mIOThreadPool;
        SoraMutexLock mIOMutex;
        SoraMutexLock mThreadMutex;
#endif
	};
	
}

#endif