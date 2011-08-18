/*
 *  osxfilewrapper.mm
 *  Sora
 *
 *  Created by GriffinBu on 12/27/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "osxfilewrapper.h"

#ifdef OS_OSX

#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

#include "SoraStringConv.h"

namespace soraosx {
	
	void enumFilesInFolder(std::vector<SoraWString>& cont, const SoraWString& folder) {
		NSString *path = [[NSString alloc] initWithUTF8String:sora::ws2s(folder).c_str()];
		NSError *error = nil;
		NSArray *array = [[NSFileManager defaultManager]
						  subpathsOfDirectoryAtPath:path error:&error];
		
		if (array != nil) {
			for(NSString* file in array) {
				NSRange range = [file rangeOfString:@".DS_Store"];
				if(range.location == NSNotFound) {
                    std::wstring path = folder + sora::s2ws([file UTF8String]) + L"/";
                    cont.push_back(path);
				}
			}
		}		
		
		[path release];
		[array release];
	}
		
} // namespace sora

#endif