/*
 *  SoraOSXFileUtility.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_OSX_FILE_UTILITY_H_
#define SORA_OSX_FILE_UTILITY_H_

#include "SoraPlatform.h"

namespace sora {
	SoraWString osxApplicationPath();
	bool osxFileExists(const wchar_t* path);
} // namespace sora

#endif