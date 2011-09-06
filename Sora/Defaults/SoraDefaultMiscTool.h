/*
 *  SoraMiscTool.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_DEFAULT_MISC_TOOL_H
#define SORA_DEFAULT_MISC_TOOL_H

#include "SoraMiscTool.h"
#include "SoraLogger.h"

namespace sora {
	
	class SoraDefaultMiscTool: public SoraMiscTool {
	public:
		SoraDefaultMiscTool(){}
		virtual ~SoraDefaultMiscTool() { }

		virtual void setLogFile(const util::String& sfile) { }

		virtual int32 messageBox(const util::String& sMessage, const util::String& sTitle, int32 msgCode) {
			log_mssg("MessageBox: "+sTitle + ": " + sMessage);
			return 0;
		}
		virtual int32 messageBox(const util::String& sMessage, const util::String& sTitle, int32 msgCode) { 
			log_mssg(L"MessageBox: "+ sTitle + L": " + sMessage);
			return 0;
		}
	
		virtual void log(const util::String& sMessage) {
			log_mssg(sMessage);
		}
		virtual void log(const util::String& sMessage) {
			log_mssg(sMessage);
		}
		
		util::String fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
			return L"\0";
		}
		
		util::String fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
			return L"\0";
		}
		
		virtual void setMainWindowHandle(ulong32 handle) { }
		
	private:
	};
} // namespace sora

#endif // SORA_DEFAULT_MISC_TOOL_H
