#ifndef SORA_RESOURCE_MANAGER_H
#define SORA_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"

#include <vector>

namespace sora {

	typedef ulong32 SoraResourceHandle;
	
	class SORA_API SoraResourceManager {
	public:
		virtual ~SoraResourceManager() {}

		virtual SoraResourceHandle	loadResourcePack    (const util::String& file) = 0;
		virtual void			attachResourcePack	(SoraResourceHandle handle) = 0;
		virtual void			detachResourcePack  (SoraResourceHandle handle) = 0;
		
		virtual void*   readResourceFile        (const util::String& file, ulong32 size) = 0;
		virtual void*   getResourceFile         (const util::String& file, ulong32& size) = 0;
		virtual ulong32 getResourceFileSize     (const util::String& file) = 0;
		virtual void    freeResourceFile        (void* p) = 0;

		/* name of this resource manager */
		virtual util::String getName() const = 0;
    
		/*
		 determines if a format is supported by this resource manager
		 @param format, resource format string, zip etc
		 */
		virtual bool isFormatSupported(const util::String& format) const = 0;
		
		virtual bool enumFiles(std::vector<util::String>& cont, const util::String& folder) { return false; }
	};
	

} // namespace sora;


#endif