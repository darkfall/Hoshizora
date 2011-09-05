#include "SoraZipResourceManager.h"
#include "SoraCore.h"

namespace sora {
    
	SoraZipResourceManager::~SoraZipResourceManager() {
		resourcePacks.clear();
	}

	SoraResourceHandle SoraZipResourceManager::loadResourcePack(const SoraWString& file) {
		SoraZipFile* pfile = new SoraZipFile;
		if(pfile) {
			if(pfile->open(file)) {
                attachResourcePack((SoraResourceHandle)pfile);
				return (SoraResourceHandle)pfile;
			}
		}
        delete pfile;
		return 0;
	}

	void SoraZipResourceManager::attachResourcePack(SoraResourceHandle file) {
		SoraAutoPtr<SoraZipFile> t((SoraZipFile*)file);
		resourcePacks[(ulong32)file] = t;
	}

	void SoraZipResourceManager::detachResourcePack(SoraResourceHandle handle) {
		RESOURCE_PACK::iterator p = resourcePacks.find(handle);
		if( p != resourcePacks.end() ) 
			resourcePacks.erase( p );
	}

	SoraZipFile* SoraZipResourceManager::getResourcePack(SoraResourceHandle handle) {
		RESOURCE_PACK::iterator p = resourcePacks.find(handle);
		if( p != resourcePacks.end() ) 
			return p->second.get();
		return 0;
	}

	void* SoraZipResourceManager::getResourceFile(const SoraWString& file, ulong32& size) {
		RESOURCE_PACK::iterator p = resourcePacks.begin();
		while( p != resourcePacks.end() ) {
			ulong32 s;
			if( (s = p->second->getFileSize( file ) ) != 0 ) {
				size = s;
				return p->second->getFile( file, s );
			}
			++p;
		}
		size = 0;
		return 0;
	}
	
	void SoraZipResourceManager::freeResourceFile(void* p) {
		if(p) delete (uint8*)(p);
	}

	ulong32 SoraZipResourceManager::getResourceFileSize(const SoraWString& file) {
		RESOURCE_PACK::iterator p = resourcePacks.begin();
		while( p != resourcePacks.end() ) {
			ulong32 s;
			if( (s = p->second->getFileSize( file ) ) != 0 )
				return s;
			++p;
		}
		return 0;
	}

	ulong32 SoraZipResourceManager::getResourceFileCRC(const SoraWString& file) {
		RESOURCE_PACK::iterator p = resourcePacks.begin();
		while( p != resourcePacks.end() ) {
			ulong32 s;
			if( (s = p->second->getFileSize( file ) ) != 0 )
				return p->second->getFileCRC( file );
			++p;
		}
		return 0;
	}

	void* SoraZipResourceManager::readResourceFile(const SoraWString& file, ulong32 size) {
		RESOURCE_PACK::iterator p = resourcePacks.begin();
		while( p != resourcePacks.end() ) {
			ulong32 s;
			if( (s = p->second->getFileSize( file ) ) != 0 )
				return p->second->getFile( file, size );
			++p;
		}
		return 0;
	}
	
	bool SoraZipResourceManager::isFormatSupported(const SoraWString& format) const {
		if(getFormat().find(format) != std::string::npos) 
			return true;
		return false;
	}
	
	bool SoraZipResourceManager::enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder) { 
		RESOURCE_PACK::iterator p = resourcePacks.begin();
		while( p != resourcePacks.end() ) {
			if(p->second->enumFiles(cont, folder))
				return true;
			++p;
		}
		return false;
	}
    
} // namespace sora;