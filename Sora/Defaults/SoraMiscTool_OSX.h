/*
 *  SoraMiscTool_OSX.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_MISCTOOL_OSX
#define SORA_MISCTOOL_OSX

#include "SoraDefaultMiscTool.h"
#include "SoraStringConv.h"
#include "SoraPlatform.h"

#ifdef OS_OSX

#include <CoreFoundation/CoreFoundation.h>

namespace sora {
	
	class SoraMiscToolOSX: public SoraDefaultMiscTool {
	public:		
		inline int32 msgCodeTokCFNotificationLevel(int32 msgCode) {
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
		
		int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
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
		
		int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
			return messageBox(ws2s(sMessage), ws2s(sTitle), msgCode);
		}
	};
	
} // namespace sora

#endif // OS_OSX

#endif // SORA_MISCTOOL_WIN32