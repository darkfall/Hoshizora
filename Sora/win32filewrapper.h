/*
 *  win32filewrapper.h
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef WIN32_FILE_WRAPPER_H_
#define WIN32_FILE_WRAPPER_H_

#include "SoraPlatform.h"

#ifdef WIN32

#include <vector>

namespace sorawin32 {

	void enumFilesInFolder(std::vector<SoraWString>& cont, const wchar_t* path);

} // namespace sorawin32

#endif

#endif