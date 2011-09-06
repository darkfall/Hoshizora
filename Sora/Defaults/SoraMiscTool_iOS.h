/*
 *  SoraMiscTool_iOS.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_MISCTOOL_IOS_H_
#define SORA_MISCTOOL_IOS_H_

#include "SoraStringConv.h"
#include "SoraPlatform.h"

#include "SoraDefaultMiscTool.h"

#ifdef OS_IOS

namespace sora {
	
	class SoraMiscTooliOS: public SoraDefaultMiscTool {
	public:		
		int32 messageBox(const util::String& sMessage, const util::String& sTitle, int32 msgCode);
		int32 messageBox(const util::String& sMessage, const util::String& sTitle, int32 msgCode) { 
			return messageBox(ws2s(sMessage), ws2s(sTitle), msgCode);
		}
		
		util::String fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
		}
		
		util::String fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
		}
	};
	
} // namespace sora

#endif // OS_OSX

#endif