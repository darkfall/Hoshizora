/*
 *  SoraMiscTool.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_MISC_TOOL_H
#define SORA_MISC_TOOL_H

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
	
	class SORA_API SoraMiscTool {
	public:
		virtual ~SoraMiscTool() {}
		
		virtual int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) = 0;
		virtual int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) = 0;
		virtual void setMainWindowHandle(ulong32 handle) = 0;
		
		virtual StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) = 0;
		virtual StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) = 0;		
	};
} // namespace sora

#endif // SORA_MISC_TOOL_H_
