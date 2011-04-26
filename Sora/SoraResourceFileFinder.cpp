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
	
	SoraResourceFileFinder::SoraResourceFileFinder() {
	}
	
	SoraResourceFileFinder::~SoraResourceFileFinder() {
	}
	
	void SoraResourceFileFinder::attachResourceManager(SoraResourceManager* rm) {
		resourceManagers.push_back(SoraAutoPtr<SoraResourceManager>(rm));
	}
	
	void SoraResourceFileFinder::detachResourceManager(const SoraWString& name) {
		for(size_t i=0; i<resourceManagers.size(); ++i) {
			if(name.compare(resourceManagers[i]->getName()) == 0) {
				resourceManagers.erase(resourceManagers.begin()+i);
				break;
			}
		}
	}
	
	void* SoraResourceFileFinder::readResourceFile(const SoraWString& file, ulong32 size) {
		void* data;
		for(size_t i=0; i<resourceManagers.size(); ++i) {
			if((data = resourceManagers[i]->readResourceFile(file, size)) != NULL)
				return data;
		}
		return 0;
	}
	
	void* SoraResourceFileFinder::getResourceFile(const SoraWString& file, ulong32& size) {
		void* data;
		for(size_t i=0; i<resourceManagers.size(); ++i) {
			if((data = resourceManagers[i]->getResourceFile(file, size)) != NULL)
				return data;
		}
		return 0;
	}
	
	ulong32 SoraResourceFileFinder::getResourceFileSize(const SoraWString& file) { 
		ulong32 size;
		for(size_t i=0; i<resourceManagers.size(); ++i) {
			if((size = resourceManagers[i]->getResourceFileSize(file)) != 0)
				return size;
		}
		return 0;
	}
	
	void SoraResourceFileFinder::freeResourceFile(void* p) {
		if(p) 
			delete (uint8*)p;
	}
	
	ulong32	SoraResourceFileFinder::loadResourcePack(const SoraWString& file) {
		size_t dotPos = file.rfind('.');
		SoraWString fileFormat;
		if(dotPos != SoraWString::npos) {
			fileFormat = file.substr(dotPos+1, file.size());
		}
#ifdef _DEBUG
		printf("load resource pack, %s, format: %s\n", ws2s(file).c_str(), ws2s(fileFormat).c_str());
#endif
		for(size_t i=1; i<resourceManagers.size(); ++i) {
			SoraResourceManager* prm = *resourceManagers[i];
				
			ulong32 r = prm->loadResourcePack(file);
			if(r) {
				prm->attachResourcePack(r);
				return r;
			}
		}
		/*
		 resourceManagers[0] = sorafodlerresoucemanager
		 just add the pack as a folder
		 */
		resourceManagers[0]->attachResourcePack(resourceManagers[0]->loadResourcePack(file));
		return 0;
	}
	
	void SoraResourceFileFinder::attachResourcePack(ulong32 file) {
		// simply does nothing
		// for compability with old versions
	}
	
	void SoraResourceFileFinder::detachResourcePack(ulong32 handle) {
		for(size_t i=0; i<resourceManagers.size(); ++i) {
			resourceManagers[i]->detachResourcePack(handle);
		}
	}
} // namespace sora