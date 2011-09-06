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

		SoraResourceHandle	loadResourcePack	(const SoraWString& file);
		void			attachResourcePack	(SoraResourceHandle handle);
		void			detachResourcePack  (SoraResourceHandle handle);
		SoraZipFile*	getResourcePack		(SoraResourceHandle handle);

		void* readResourceFile				(const SoraWString& file, ulong32 size);
		void* getResourceFile				(const SoraWString& file, ulong32& size);
		ulong32 getResourceFileSize	(const SoraWString& file);
		ulong32 getResourceFileCRC	(const SoraWString& file);
		void  freeResourceFile(void* p);
		
		SoraWString getName() const { return L"ZipRM"; }
		SoraWString getFormat() const { return L"zip;rfResource"; }
		bool isFormatSupported(const SoraWString& format) const;
		
		bool enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder);
		
	private:
		RESOURCE_PACK resourcePacks;
	};

} // namespace sora;


#endif