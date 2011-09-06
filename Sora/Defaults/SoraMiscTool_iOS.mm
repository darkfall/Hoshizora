/*
 *  SoraMiscTool_iOS.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */
#include "SoraMiscTool_iOS.h"

#ifdef OS_IOS

#include <CoreFoundation/CoreFoundation.h>

namespace sora {

	// to do, delegate
	int32 SoraMiscTooliOS::messageBox(const util::String& sMessage, const util::String& sTitle, int32 msgCode) {
		NSString* nsMessage = [[NSString alloc] initWithUTF8String:sMessage.c_str()];
		NSString* nsTitle = [[NSString alloc] initWithUTF8String:sTitle.c_str()];
		
		UIAlertView* alert = [[UIAlertView alloc]
							  initWithTitle:nsTitle
							  message:nsMessage
							  delegate:nil 
							  cancelButtonTitle:(msgCode&MB_OK)?@"OK":nil
							  otherButtonTitles:nil];
		[alert show];
		return 1;
	}


} // namespace sora

#endif // OS_IOS