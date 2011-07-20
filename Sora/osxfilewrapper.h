/*
 *  osx_filewrapper.h
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef OSX_FILE_WRAPPER_H_
#define OSX_FILE_WRAPPER_H_

#include "SoraPlatform.h"
#include <vector>

#ifdef OS_OSX

namespace soraosx {
	
	void enumFilesInFolder(std::vector<SoraWString>& cont, const wchar_t* folder);
	
} // namespace soraosx

#endif

#endif