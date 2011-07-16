/*
 *  SoraMiscTool_OSX.mm
 *  Sora
 *
 *  Created by Griffin Bu on 5/31/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraMiscTool_OSX.h"

#ifdef OS_OSX

#import <Cocoa/Cocoa.h>	
#import <Foundation/Foundation.h>

namespace sora {
	
	inline int32 SoraMiscToolOSX::msgCodeTokCFNotificationLevel(int32 msgCode) {
		// to do
		if(msgCode & MB_ICONERROR)
			return kCFUserNotificationCautionAlertLevel;
		else if(msgCode & MB_ICONSTOP)
			return kCFUserNotificationStopAlertLevel;
		else if(msgCode & MB_ICONINFORMATION) 
			return kCFUserNotificationNoteAlertLevel;
		else if(msgCode & MB_ICONWARNING)
			return kCFUserNotificationPlainAlertLevel;
		
		return kCFUserNotificationPlainAlertLevel;
	}
	
	int32 SoraMiscToolOSX::messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
		CFStringRef header_ref   = CFStringCreateWithCString(NULL, sTitle.c_str(), sTitle.size());
		CFStringRef message_ref  = CFStringCreateWithCString(NULL, sMessage.c_str(), sMessage.size());
		
		CFOptionFlags result;
		int32 notificate = msgCodeTokCFNotificationLevel(msgCode);
		CFUserNotificationDisplayAlert(0,
									   notificate,
									   NULL,
									   NULL,
									   NULL,
									   header_ref,
									   message_ref,
									   NULL, // default "ok"
									   (msgCode&MB_OKCANCEL)?CFSTR("Cancel"):NULL, // alternate
									   NULL, //other button title
									   &result
									   );
		
		CFRelease( header_ref );
		CFRelease( message_ref );
		
		switch(result) {
			case kCFUserNotificationDefaultResponse: return IDOK;
			case kCFUserNotificationCancelResponse: return IDCANCEL;
			case kCFUserNotificationAlternateResponse: return IDCANCEL;
			case kCFUserNotificationOtherResponse: return IDABORT;
		}
		return 0;
	}
	
	int32 SoraMiscToolOSX::messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
		return messageBox(ws2s(sMessage), ws2s(sTitle), msgCode);
	}

	SoraWString SoraMiscToolOSX::fileOpenDialog(const char* filter, const char* defaultPath) {
		NSOpenPanel* openDlg = [NSOpenPanel openPanel];
		
		[openDlg setAllowsMultipleSelection:NO];
		
		NSMutableArray* fileTypes = nil;
		if(filter != NULL) {
			SoraString strFilter = filter;
			fileTypes = [NSMutableArray arrayWithObject:@"*"];
			size_t prevPos = 0;
			for(size_t i=0; i<strFilter.size(); ++i) {
				if(filter[i] == ';') {
					[fileTypes addObject: [[NSString alloc] initWithUTF8String: strFilter.substr(prevPos, i).c_str()]];
					prevPos = i+1;
				}
			}
		}
		
		NSString* filePath;
		if(defaultPath)
			filePath = [[NSString alloc] initWithUTF8String: defaultPath];
		else
			filePath = [[NSString alloc] initWithUTF8String: "~/"];
		
		ulong32 result = [openDlg runModalForDirectory:filePath
											  file:nil 
											 types:fileTypes];
		
		if (result == NSOKButton) {
            [filePath release];
			NSArray *filesToOpen = [openDlg filenames];
			ulong32 count = [filesToOpen count];
			if(count > 0) {
				std::string fileName = [[filesToOpen objectAtIndex:0] UTF8String];
				return sora::s2ws(fileName);
			}
		}
		return L"\0";			
	}
	
	SoraWString SoraMiscToolOSX::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		NSSavePanel* saveDlg = [NSSavePanel savePanel];
		
		if(defaultExt != NULL) {
			NSString* fileExt = [[NSString alloc] initWithUTF8String: defaultExt];
			[saveDlg setRequiredFileType:fileExt];
		}
		
		NSString* filePath;
		if(defaultPath != NULL)
			filePath= [[NSString alloc] initWithUTF8String: defaultPath];
		else
			filePath = [[NSString alloc] initWithUTF8String: "~/"];
		
		int result = [saveDlg runModalForDirectory:filePath
											  file:nil];
		
		if (result == NSOKButton) {
            [filePath release];
			std::string fileName = [[saveDlg filename] UTF8String];
			return sora::s2ws(fileName);
		}
		return L"\0";
		
	}

}

#endif // OS_OSX