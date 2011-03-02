#ifndef SORA_RESOURCE_MANAGER_H
#define SORA_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include <vector>

namespace sora {

	typedef ulong32 HSORARESOURCE;
	
	class SoraResourceManager {
		
	public:
		virtual ~SoraResourceManager() {}

		virtual HSORARESOURCE	loadResourcePack (const SoraWString& file) = 0;
		virtual void			attachResourcePack	(HSORARESOURCE handle) = 0;
		virtual void			detachResourcePack  (HSORARESOURCE handle) = 0;
		
		virtual void* readResourceFile  (const SoraWString& file, ulong32 size) = 0;
		virtual void* getResourceFile (const SoraWString& file, ulong32& size) = 0;
		virtual ulong32 getResourceFileSize (const SoraWString& file) = 0;
		virtual void freeResourceFile(void* p) = 0;

		/* name of this resource manager */
		virtual SoraWString getName() const = 0;
		/* format of files this resource manager supports
		 format strimped by ;
		 zip;bzip;tar etc
		 */
		virtual SoraWString getFormat() const = 0;
		/*
		 determines if a format is supported by this resource manager
		 @param format, resource format string, zip etc
		 */
		virtual bool isFormatSupported(const SoraWString& format) const = 0;
	};

} // namespace sora;


#endif