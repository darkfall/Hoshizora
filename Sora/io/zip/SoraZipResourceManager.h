#ifndef SORA_ZIP_RESOURCE_MANAGER_H
#define SORA_ZIP_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraAutoPtr.h"

#include "SoraZipFile.h"

#include "SoraResourceManager.h"

#include <map>

namespace sora {

	typedef ulong32 SoraResourceHandle;
	
	class SoraZipResourceManager: public SoraResourceManager {
		typedef std::map<ulong32, SoraAutoPtr<SoraZipFile> > RESOURCE_PACK;
		
	public:
		~SoraZipResourceManager();

		SoraResourceHandle	loadResourcePack	(const util::String& file);
		void			attachResourcePack	(SoraResourceHandle handle);
		void			detachResourcePack  (SoraResourceHandle handle);
		SoraZipFile*	getResourcePack		(SoraResourceHandle handle);

		void* readResourceFile				(const util::String& file, ulong32 size);
		void* getResourceFile				(const util::String& file, ulong32& size);
		ulong32 getResourceFileSize	(const util::String& file);
		ulong32 getResourceFileCRC	(const util::String& file);
		void  freeResourceFile(void* p);
		
		util::String getName() const { return L"ZipRM"; }
		util::String getFormat() const { return L"zip;rfResource"; }
		bool isFormatSupported(const util::String& format) const;
		
		bool enumFiles(std::vector<util::String>& cont, const SoraWString& folder);
		
	private:
		RESOURCE_PACK resourcePacks;
	};

} // namespace sora;


#endif