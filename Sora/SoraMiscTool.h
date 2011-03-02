/*
 *  SoraMiscTool.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_MISC_TOOL_H
#define SORA_MISC_TOOL_H

#include "SoraPlatform.h"

namespace sora {
	
	class SoraMiscTool {
	public:
		virtual int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) = 0;
		virtual int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) = 0;

		virtual void setLogFile(const SoraString& sLogFile) = 0;

		virtual void log(const SoraString& sMessage) = 0;
		virtual void log(const SoraWString& sMessage) = 0;
		
		virtual void setMainWindowHandle(ulong32 handle) = 0;
	};
} // namespace sora

#endif // SORA_MISC_TOOL_H_
