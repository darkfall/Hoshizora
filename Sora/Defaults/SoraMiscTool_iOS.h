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

#include "../SoraStringConv.h"
#include "../SoraPlatform.h"

#include "SoraDefaultMiscTool.h"

#ifdef OS_IOS

namespace sora {
	
	class SoraMiscTooliOS: public SoraDefaultMiscTool {
	public:		
		int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode);
		int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
			return messageBox(ws2s(sMessage), ws2s(sTitle), msgCode);
		}
		
		SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
		}
		
		SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
		}
	};
	
} // namespace sora

#endif // OS_OSX

#endif