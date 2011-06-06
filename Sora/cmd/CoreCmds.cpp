/*
 *  CoreCmds.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/6/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "CoreCmds.h"
#include "SoraConsole.h"

#include "SoraConsole.h"
#include "Debug/SoraInternalLogger.h"

#include "SoraCore.h"

namespace sora {

	SoraCoreCmdHandler::SoraCoreCmdHandler() {
		registerEventFunc(this, &SoraCoreCmdHandler::onCmdEvent);
		
		sora::SoraConsole::Instance()->registerCmdHandler(this, "set");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "log");
		
		sora::SoraConsole::Instance()->registerCmdHandler(this, "exit");
		
		sora = SoraCore::Instance();
	}
	
	void SoraCoreCmdHandler::onCmdEvent(SoraConsoleEvent* cev) {
		std::vector<std::string> params;
		deliStr(params, cev->getParams().c_str(), ' ');
		
		if(cev->getCmd().compare("set") == 0) {
			if(params.size() > 0) {
				std::string p1 = params[0];
				size_t dotPos = p1.find('.');
				if(dotPos != std::string::npos) {
					std::string p1Prev = p1.substr(0, dotPos);
					if(p1Prev.compare("window") == 0 && params.size() >= 2) {
						std::string p1Param = p1.substr(dotPos+1, p1.size());
						
						if(p1Param.compare("width") == 0) {
							int32 width = atoi(params[1].c_str());
							sora->setWindowSize(width, sora->getScreenHeight());
							
							cev->setResults("Window.Width seted to "+params[1]);
						} else if(p1Param.compare("height") == 0) {
							int32 height = atoi(params[1].c_str());
							sora->setWindowSize(sora->getScreenWidth(), height);
							
							cev->setResults("Window.Height seted to "+params[1]);
						} else if(p1Param.compare("title") == 0) {
							sora->setWindowTitle(s2ws(params[1]).c_str());
							
							cev->setResults("Window.Title seted to "+params[1]);
						}
					}
				}
			}
		} else if(cev->getCmd().compare("log") == 0) {
			if(params.size() > 0) {
				for(size_t i=0; i<params.size(); ++i) {
					DebugPtr->log(params[i]);
				}
			}
		} else if(cev->getCmd().compare("exit") == 0) {
			sora->shutDown();
		}
	}
	
} // namespace sora