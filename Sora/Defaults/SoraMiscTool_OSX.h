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

#include "SoraPlatform.h"
#include "SoraDefaultMiscTool.h"
#include "SoraStringConv.h"

#ifdef OS_OSX

namespace sora {
	
	class SoraMiscToolOSX: public SoraDefaultMiscTool {
	public:		
		inline int32 msgCodeTokCFNotificationLevel(int32 msgCode);
		
		int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode);
		int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode);
		
		SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);
	};
	
} // namespace sora

#endif // OS_OSX

#endif // SORA_MISCTOOL_WIN32