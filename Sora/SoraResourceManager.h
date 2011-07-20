#ifndef SORA_RESOURCE_MANAGER_H
#define SORA_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include <vector>

namespace sora {

	typedef ulong32 HSORARESOURCE;
	
	class SORA_API SoraResourceManager {
	public:
		virtual ~SoraResourceManager() {}

		virtual HSORARESOURCE	loadResourcePack (const wchar_t* file) = 0;
		virtual void			attachResourcePack	(HSORARESOURCE handle) = 0;
		virtual void			detachResourcePack  (HSORARESOURCE handle) = 0;
		
		virtual void* readResourceFile  (const wchar_t* file, ulong32 size) = 0;
		virtual void* getResourceFile (const wchar_t* file, ulong32& size) = 0;
		virtual ulong32 getResourceFileSize (const wchar_t* file) = 0;
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
		virtual bool isFormatSupported(const wchar_t* format) const = 0;
		
		virtual bool enumFiles(std::vector<SoraWString>& cont, const wchar_t* folder) { return false; }
	};
	

} // namespace sora;


#endif