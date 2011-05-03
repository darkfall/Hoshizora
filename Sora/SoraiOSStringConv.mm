/*
 *  SoraiOSStringConv.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#include "SoraiOSStringConv.h"

#ifdef OS_IOS

#import <Foundation/Foundation.h>

namespace sora {
	
	std::wstring NSStringToStringW ( NSString* Str )   
	{   
		NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingHZ_GB_2312 );   
		NSData* pSData              =   [ Str dataUsingEncoding : pEncode ];    
		
		return std::wstring ( (wchar_t*) [ pSData bytes ], [ pSData length] / sizeof ( wchar_t ) );   
	}   
	
	NSString* StringWToNSString ( const std::wstring& Str )   
	{   
		NSString* pString = [ [ NSString alloc ]    
							 initWithBytes : (char*)Str.data()   
							 length : Str.size() * sizeof(wchar_t)   
                             encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];   
		return pString;   
	} 
	
	std::wstring iOSString2WString(const std::string& str) {
		NSString* nsstr = [[NSString alloc] initWithUTF8String: str.c_str()];
		return NSStringToStringW(nsstr);
	}
	
	std::string iOSWString2String(const std::wstring& str) {
		return [StringWToNSString(str) UTF8String];
	}


} // namespace sora

#endif