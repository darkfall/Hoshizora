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

#include "SoraMiscTool.h"
#include "SoraDefaultLogger.h"

namespace sora {
	
	class SoraDefaultMiscTool: public SoraMiscTool {
	public:
		SoraDefaultMiscTool() { pLogger = new SoraDefaultLogger; }
		virtual ~SoraDefaultMiscTool() { delete pLogger; }

		virtual void setLogFile(const SoraString& sfile) { pLogger->setFile(sfile); }

		virtual int32 messageBox(const SoraString& sMessage, const SoraString& sTitle, int32 msgCode) {
			pLogger->log("MessageBox: "+sTitle + ": " + sMessage);
			return 0;
		}
		virtual int32 messageBox(const SoraWString& sMessage, const SoraWString& sTitle, int32 msgCode) { 
			pLogger->log(L"MessageBox: "+ sTitle + L": " + sMessage);
			return 0;
		}
	
		virtual void log(const SoraString& sMessage) {
			pLogger->log(sMessage);
		}
		virtual void log(const SoraWString& sMessage) {
			pLogger->log(sMessage);
		}

		virtual void setMainWindowHandle(ulong32 handle) { }
		
	private:
		SoraDefaultLogger* pLogger;
	};
} // namespace sora

#endif // SORA_DEFAULT_MISC_TOOL_H
