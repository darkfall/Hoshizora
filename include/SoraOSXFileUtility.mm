/*
 *  SoraOSXFileUtility.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraOSXFileUtility.h"
#include "SoraStringConv.h"

#ifdef OS_OSX

#include <Foundation/Foundation.h>

namespace sora {
	
	SoraWString osxApplicationPath() {
		return s2ws([[[NSBundle mainBundle] bundlePath] UTF8String]);
	}
	
	bool osxFileExists(const SoraWString& path) {
		NSString* nsPath = [[NSString alloc] initWithUTF8String:ws2s(path).c_str()];
		return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
	}
} // namespace sora

#endif