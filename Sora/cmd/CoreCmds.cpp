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

#include "../Debug/SoraInternalLogger.h"

#include "../SoraCore.h"
#include "../SoraLocalizer.h"
#include "../SoraEventManager.h"

namespace sora {

	SoraCoreCmdHandler::SoraCoreCmdHandler() {
		registerEventFunc(this, &SoraCoreCmdHandler::onCmdEvent);
		
		sora::SoraConsole::Instance()->registerCmdHandler(this, "set");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "get");

		
		sora::SoraConsole::Instance()->registerCmdHandler(this, "log");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "save");
		sora::SoraConsole::Instance()->registerCmdHandler(this, "exit");
		
		sora::SoraConsole::Instance()->registerCmdHandler(this, "setlocale");
		
		sora = SoraCore::Instance();
	}
	
	void SoraCoreCmdHandler::onCmdEvent(SoraConsoleEvent* cev) {
		std::vector<std::string> params;
		deliStr(params, cev->getParams().c_str(), ' ');
		
		if(cev->getCmd().compare("set") == 0) {
			_doCmdSet(cev, params);
		} else if(cev->getCmd().compare("get") == 0) {
			_doCmdGet(cev, params);
		} else {
			_doCmdMisc(cev, params);
		}
	}
	
	void SoraCoreCmdHandler::_doCmdSet(SoraConsoleEvent* cev, const std::vector<std::string>& params) {
		if(params.size() > 0) {
			std::string p1 = params[0];
			size_t dotPos = p1.find('.');
			std::string p1Prev;
			if(dotPos != std::string::npos) {
				p1Prev = p1.substr(0, dotPos);
			} else {
				p1Prev = p1;
			}

			if(dotPos != std::string::npos) {
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
			} else  {
				if(p1Prev.compare("int") == 0 && params.size() >= 3) {
					int32 val = atoi(params[2].c_str());
					
					SET_ENV_INT(params[1].c_str(), val);
					
					cev->setResults("EnvValue "+params[1]+" has been set to "+params[2]);
					
				} else if(p1Prev.compare("float") == 0 && params.size() >= 3) {
					float32 val = atof(params[2].c_str());
					
					SET_ENV_FLOAT(params[1].c_str(), val);
					
					cev->setResults("EnvValue "+params[1]+" has been set to "+params[2]);
					
				} else if(p1Prev.compare("bool") == 0 && params.size() >= 3) {
					bool val = false;
					if(params[2].compare("true") == 0 ||
					   params[2].compare("1") == 0)
						val = true;
					
					SET_ENV_BOOL(params[1].c_str(), val);
					
					cev->setResults("EnvValue "+params[1]+" has been set to "+params[2]);
					
				} else if(p1Prev.compare("string") == 0 && params.size() >= 3) {					
					SET_ENV_STRING(params[1].c_str(), params[2].c_str());
					
					cev->setResults("EnvValue "+params[1]+" has been set to "+params[2]);
				} 
				
				else if(p1Prev.compare("file_check_interval") == 0 && params.size() >= 1) {
					SoraEventManager::Instance()->setFileChangeDetectionInterval(atof(params[0].c_str()));
				}
			}
		}
	}
	
	void SoraCoreCmdHandler::_doCmdGet(SoraConsoleEvent* cev, const std::vector<std::string>& params) {
		std::string results;
		if(params.size() > 0) {
			std::string p1 = params[0];
			size_t dotPos = p1.find('.');
			std::string p1Prev;
			if(dotPos != std::string::npos) {
				p1Prev = p1.substr(0, dotPos);
			} else {
				p1Prev = p1;
			}
			
			if(dotPos != std::string::npos) {
				if(p1Prev.compare("window") == 0) {
					std::string p1Param = p1.substr(dotPos+1, p1.size());
					
					if(p1Param.compare("width") == 0) {
						cev->setResults(vamssg("Window.Width = %d", sora->getScreenWidth()));
										
					} else if(p1Param.compare("height") == 0) {
						cev->setResults(vamssg("Window.Width = %d", sora->getScreenHeight()));
										
					} else if(p1Param.compare("title") == 0) {
						cev->setResults(vamssg("Window.Width = %s", sora->getMainWindow()->getWindowName().c_str()));
					
					} 
				} 
			} else {
				if(p1Prev.compare("objnum") == 0) {
					cev->setResults(vamssg("Current alive SoraObject number = %d", SoraObject::members.size()));
			
				} else if(p1Prev.compare("memuse") == 0) {
					cev->setResults(vamssg("Current memory use = %dkb", sora->getEngineMemoryUsage()));

				} else if(p1Prev.compare("int") == 0 && params.size() >= 2) {
					cev->setResults("EnvValue "+vamssg("%s = %d", params[1].c_str(), GET_ENV_INT(params[1].c_str(), 0)));
					
				} else if(p1Prev.compare("float") == 0 && params.size() >= 2) {
					cev->setResults("EnvValue "+vamssg("%s = %f", params[1].c_str(), GET_ENV_FLOAT(params[1].c_str(), 0.f)));

					
				} else if(p1Prev.compare("bool") == 0 && params.size() >= 2) {
					cev->setResults("EnvValue "+vamssg("%s = %s", params[1].c_str(), GET_ENV_BOOL(params[1].c_str(), false)?"true":"false"));
					
				} else if(p1Prev.compare("string") == 0 && params.size() >= 2) {					
					cev->setResults("EnvValue "+vamssg("%s = %s", params[1].c_str(), GET_ENV_STRING(params[1].c_str(), "???").c_str()));
				}
			}
		}
	}
	
	void SoraCoreCmdHandler::_doCmdMisc(SoraConsoleEvent* cev, const std::vector<std::string>& params) {
		if(cev->getCmd().compare("log") == 0) {
			if(params.size() > 0) {
				for(size_t i=0; i<params.size(); ++i) {
					DebugPtr->log(params[i]);
				}
			}
		} else if(cev->getCmd().compare("exit") == 0) {
			sora->shutDown();
		} else if(cev->getCmd().compare("save") == 0) {
			if(params.size() > 0) {
				std::string result = "Log have been wrote to ";
				for(size_t i=0; i<params.size(); ++i) {
					DebugPtr->writeToFile(params[i].c_str());
					result += params[i] + ", ";
				}
				cev->setResults(result);
			}
		} else if(cev->getCmd().compare("setlocale") == 0) {
			if(params.size() == 1) {
				SoraLocalizer::Instance()->setCurrentLocale(params[0]);
			}
		}
			
	}
	
	
} // namespace sora