/*
 *  SoraiOSStringConv.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#include "SoraiOSStringConv.h"

#if defined(OS_IOS) || defined(OS_OSX)

#import <Foundation/Foundation.h>

namespace sora {
    
	std::wstring iOSString2WString(const std::string& str) {
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
        
		NSString* nsstr = [[NSString alloc] initWithUTF8String: str.c_str()];
        NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE );   
		NSData* pSData              =   [ nsstr dataUsingEncoding : pEncode ];    
		
        std::wstring result = std::wstring ( (wchar_t*) [ pSData bytes ], [ pSData length] / sizeof ( wchar_t ) ); 
     //   [pSData release];
        [nsstr release];
        
        [pool drain];
        return result;
	}
	
	std::string iOSWString2String(const std::wstring& str) {
        NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
        
        NSString* pString = [ [ NSString alloc ]    
							 initWithBytes : (char*)str.data()   
							 length : str.size() * sizeof(wchar_t)   
                             encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];   
        
        std::string result = [pString UTF8String];
        [pString release];
        [pool drain];
		return result;
	}


} // namespace sora

#endif