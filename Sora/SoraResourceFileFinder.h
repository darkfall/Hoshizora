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
#include <vector>
#include <map>

namespace sora {
	
	class SoraResourceManager;
	
	class SoraResourceFileFinder {
	public:
		SoraResourceFileFinder();
		~SoraResourceFileFinder();
		
		void attachResourceManager(SoraResourceManager* rm);
		void detachResourceManager(const util::String& name);
		
		ulong32	loadResourcePack    (const util::String& file);
		void	attachResourcePack	(ulong32 handle);
		void	detachResourcePack  (ulong32 handle);
		
		void* readResourceFile				(const util::String& file, ulong32 size);
		void* getResourceFile				(const util::String& file, ulong32& size);
		ulong32 getResourceFileSize			(const util::String& file);
		void freeResourceFile(void* p);
		
		bool enumFiles(std::vector<util::String>& cont, const util::String& folder);
        
        static ulong32 ResourceMemory;
        
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
	};
	
}

#endif