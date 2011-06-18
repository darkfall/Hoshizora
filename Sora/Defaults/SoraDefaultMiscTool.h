/*
 *  SoraMiscTool.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_DEFAULT_MISC_TOOL_H
#define SORA_DEFAULT_MISC_TOOL_H

#include "../SoraMiscTool.h"
#include "../Debug/SoraInternalLogger.h"

namespace sora {
	
	class SoraDefaultMiscTool: public SoraMiscTool {
	public:
		SoraDefaultMiscTool(){}
		virtual ~SoraDefaultMiscTool() { }

		virtual void setLogFile(const SoraString& sfile) { }

		virtual int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
			DebugPtr->log("MessageBox: "+sTitle + ": " + sMessage);
			return 0;
		}
		virtual int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
			DebugPtr->log(L"MessageBox: "+ sTitle + L": " + sMessage);
			return 0;
		}
	
		virtual void log(const SoraString& sMessage) {
			DebugPtr->log(sMessage);
		}
		virtual void log(const SoraWString& sMessage) {
			DebugPtr->log(sMessage);
		}
		
		SoraWString fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
			return L"\0";
		}
		
		SoraWString fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
			return L"\0";
		}
		
		virtual void setMainWindowHandle(ulong32 handle) { }
		
	private:
	};
} // namespace sora

#endif // SORA_DEFAULT_MISC_TOOL_H
