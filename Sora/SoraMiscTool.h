/*
 *  SoraMiscTool.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_MISC_TOOL_H
#define SORA_MISC_TOOL_H

#include "SoraPlatform.h"

namespace sora {
	
	class SORA_API SoraMiscTool {
	public:
		virtual ~SoraMiscTool() {}
		
		virtual int32 messageBox(const char* sMessage, const char* sTitle, int32 msgCode) = 0;
		virtual int32 messageBox(const wchar_t* sMessage, const wchar_t* sTitle, int32 msgCode) = 0;
		virtual void setMainWindowHandle(ulong32 handle) = 0;
		
		virtual SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) = 0;
		virtual SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) = 0;		
	};
} // namespace sora

#endif // SORA_MISC_TOOL_H_
