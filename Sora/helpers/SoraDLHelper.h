/*
 *  SoraDLHelper.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_DYNAMIC_LIBRARY_HELPER
#define SORA_DYNAMIC_LIBRARY_HELPER

#include "../SoraPlatform.h"
#include "../SoraSingleton.h"

#include <map>

namespace sora {
	
	class SoraDLHelper: public SoraSingleton<SoraDLHelper> {
		friend class SoraSingleton<SoraDLHelper>;
		
		SoraDLHelper() {}
		~SoraDLHelper();
		
	public:
		ulong32 opendl(const SoraString& path);
		ulong32 getdl(const SoraString& name);
		void closedl(ulong32);
		void* getProc(ulong32 handle, const SoraString& name);
		
	private:
		typedef std::map<SoraString, ulong32> DLL_MAP;
		DLL_MAP mOpenedDll;
	};
	
} // namespace sora

#endif // SORA_DYNAMIC_LIBRARY_HELPER