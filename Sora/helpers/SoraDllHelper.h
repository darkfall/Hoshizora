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
	
	class SORA_API SoraDllHelper: public SoraSingleton<SoraDllHelper> {
		friend class SoraSingleton<SoraDllHelper>;
		
		SoraDllHelper() {}
		~SoraDllHelper();
		
	public:
		ulong32 opendl(const char* path);
		ulong32 getdl(const char* name);
		void closedl(ulong32);
		void* getProc(ulong32 handle, const char* name);
		
	private:
		typedef std::map<SoraString, ulong32> DLL_MAP;
		DLL_MAP mOpenedDll;
	};
	
} // namespace sora

#endif // SORA_DYNAMIC_LIBRARY_HELPER