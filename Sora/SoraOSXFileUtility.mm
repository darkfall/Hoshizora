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
	
	SoraWString osxApplicationPathW() {
		return s2ws([[[NSBundle mainBundle] bundlePath] UTF8String]);
	}
	
    SoraString osxApplicationPath() {
        return [[[NSBundle mainBundle] bundlePath] UTF8String];
    }
    
	bool osxFileExists(const SoraWString& path) {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
		NSString* nsPath = [[NSString alloc] initWithUTF8String:ws2s(path).c_str()];
		bool result = [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
        [nsPath release];
        
        [pool release];
        return result;
	}
} // namespace sora

#endif