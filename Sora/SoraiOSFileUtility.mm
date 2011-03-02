/*
 *  SoraiOSFileUtility.mm
 *  Sora-iPhone
 *
 *  Created by GriffinBu on 1/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "SoraiOSFileUtility.h"
#include "SoraStringConv.h"
#include "Debug/SoraInternalLogger.h"

#ifdef OS_IOS

namespace sora {

	inline std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	inline NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
	inline NSString* string2NSString(const char* str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str];
		return buffer;
	}	
	
	SoraWString appResourcePath() {
		SoraWString path(appApplicationPath());
		path += L"/";
		LOG->debugPrintf(ws2s(path).c_str());
		return path;
	}
	
	SoraWString appApplicationPath() {
		return s2ws([[[NSBundle mainBundle] bundlePath] UTF8String]);
	}
	
	SoraWString appDocumentPath() {
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		return s2ws(NSString2String(basePath));
	}
	
	bool appFileExists(const SoraWString& path) {
		NSString* nsPath = [[NSString alloc] initWithUTF8String:ws2s(path).c_str()];
		return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
	}
	
} // namespace sora

#endif // OS_IOS