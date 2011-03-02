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
			std::setlocale(LC_CTYPE, "chs"); 

			for(NSString* file in array) {
				const char* _Source = [file UTF8String];
				size_t _Dsize = [file length] + 1;
				wchar_t *_Dest = new wchar_t[_Dsize];
				wmemset(_Dest, 0, _Dsize);
				mbstowcs(_Dest,_Source,_Dsize);
				
				cont.push_back(_Dest);
				delete []_Dest;
			}
			
			std::setlocale(LC_CTYPE, "C");
		}		
		
		[path release];
		[array release];
	}
		
} // namespace sora

#endif