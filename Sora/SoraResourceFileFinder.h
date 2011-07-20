/*
 *  SoraResourceManagerSolver.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_RESOURCE_MANAGER_SOLVER_H_
#define SORA_RESOURCE_MANAGER_SOLVER_H_

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraAutoPtr.h"

#include <vector>
#include <map>

namespace sora {
	
	class SoraResourceManager;
	
	class SoraResourceFileFinder {
	public:
		SoraResourceFileFinder();
		~SoraResourceFileFinder();
		
		void attachResourceManager(SoraResourceManager* rm);
		void detachResourceManager(const wchar_t* name);
		
		ulong32	loadResourcePack (const wchar_t* file);
		void	attachResourcePack	(ulong32 handle);
		void	detachResourcePack  (ulong32 handle);
		
		void* readResourceFile				(const wchar_t* file, ulong32 size);
		void* getResourceFile				(const wchar_t* file, ulong32& size);
		ulong32 getResourceFileSize			(const wchar_t* file);
		void freeResourceFile(void* p);
		
		bool enumFiles(std::vector<SoraWString>& cont, const wchar_t* folder);
		
	private:		
		typedef std::vector<SoraAutoPtr<SoraResourceManager> > RESOURCE_MANAGER_CONT;
		RESOURCE_MANAGER_CONT resourceManagers;
	};
	
}

#endif