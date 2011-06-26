/*
 *  SoraDLHelper.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraDLHelper.h"

#ifndef OS_WIN32
#include <dlfcn.h>
#else
#endif 

#include "../Debug/SoraInternalLogger.h"

#include "../SoraFileUtility.h"

namespace sora {
	
	SoraDLHelper::~SoraDLHelper() {
		DLL_MAP::iterator itDll = mOpenedDll.begin();
		while(itDll != mOpenedDll.end()) {
			closedl(itDll->second);
			++itDll;
		}
	}
	
	ulong32 SoraDLHelper::getdl(const SoraString& name) {
		DLL_MAP::iterator itDll = mOpenedDll.find(name);
		if(itDll != mOpenedDll.end())
			return itDll->second;
		return 0;
	}

	ulong32 SoraDLHelper::opendl(const SoraString& path) {
		ulong32 result = 0;
		
#ifndef OS_WIN32
		void* h = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if(h != NULL)
			result = (ulong32)h;
		else
			DebugPtr->log(dlerror(), LOG_LEVEL_ERROR);
#else
		result = (ulong32)LoadLibrary(path.c_str());
#endif
		if(result != 0) {
			mOpenedDll[SoraFileUtility::getFileName(path)] = result;
		}
		return result;
	}
	
	void SoraDLHelper::closedl(ulong32 handle) {
#ifndef OS_WIN32
		dlclose((void*)handle);
#else
		FreeLibrary((HMODULE)handle);
#endif
		DLL_MAP::iterator itDll = mOpenedDll.begin();
		while(itDll != mOpenedDll.end()) {
			if(itDll->second == handle) {
				mOpenedDll.erase(itDll);
				break;
			}
			++itDll;
		}
	}
	
	void* SoraDLHelper::getProc(ulong32 handle, const SoraString& name) {
#ifndef OS_WIN32
		return dlsym((void*)handle, name.c_str());
#else
		return (void*)GetProcAddress((HMODULE)handle, name.c_str());
#endif
	}

} // namespace sora