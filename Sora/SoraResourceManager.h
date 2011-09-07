#ifndef SORA_RESOURCE_MANAGER_H
#define SORA_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraString.h"
#include <vector>

namespace sora {
	
	class SORA_API SoraResourceManager {
	public:
		virtual ~SoraResourceManager() {}

		virtual SoraResourceHandle	loadResourcePack    (const StringType& file) = 0;
		virtual void                attachResourcePack	(SoraResourceHandle handle) = 0;
		virtual void                detachResourcePack  (SoraResourceHandle handle) = 0;
		
		virtual void*   readResourceFile        (const StringType& file, ulong32 size) = 0;
		virtual void*   getResourceFile         (const StringType& file, ulong32& size) = 0;
		virtual ulong32 getResourceFileSize     (const StringType& file) = 0;
		virtual void    freeResourceFile        (void* p) = 0;

		/* name of this resource manager */
		virtual StringType getName() const = 0;
    
		/*
		 determines if a format is supported by this resource manager
		 @param format, resource format string, zip etc
		 */
		virtual bool isFormatSupported(const StringType& format) const = 0;
		
		virtual bool enumFiles(std::vector<SoraWString>& cont, const StringType& folder) { return false; }
	};
	

} // namespace sora;


#endif