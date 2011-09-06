/*
 *  SoraLocalizer.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/14/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraLocalizer.h"
#include "SoraGlobalMessageRouter.h"
#include "function/SoraBind.h"

namespace sora {

	SoraLocalizer* SoraLocalizer::mInstance = NULL;

	SoraLocalizer* SoraLocalizer::Instance() {
		if(!mInstance)
			mInstance = new SoraLocalizer;
		return mInstance;
	}

	void SoraLocalizer::Destroy() {
		if(mInstance) {
			delete mInstance;
			mInstance = NULL;
		}
	}
    
    SoraLocalizer::SoraLocalizer() {
        AcceptMessage("setlocale", sora::Bind(this, &SoraLocalizer::onMessage));
    }
	
	SoraLocalizer::~SoraLocalizer() {
	}
	
    void SoraLocalizer::onMessage(SoraMessageEvent* message) {
        try {
            util::String data = message->getData<util::String>();
            setCurrentLocale(data);
            
            message->consume();
        } catch(...) {
            
        }
    }
    
	bool SoraLocalizer::readToken(llexer* lexer, Token tokenExpected) {
		return (lexer->getNextToken() == tokenExpected);
	}
	
	util::String SoraLocalizer::readLocaleIdent(llexer* confLexer) {
		readToken(confLexer, TOKEN_TYPE_IDENT);
		if(strcmp(confLexer->getCurrLexeme(), "locale") == 0) {
			readToken(confLexer, TOKEN_TYPE_OP);
			readToken(confLexer, TOKEN_TYPE_IDENT);

			return confLexer->getCurrLexeme();
		}
		return "\0";
	}
	
	bool SoraLocalizer::readLocaleString(llexer* confLexer, LocaleStringMap& strMap) {
		util::String strIdent = confLexer->getCurrLexeme();
		if(!readToken(confLexer, TOKEN_TYPE_OP)) return false;
		if(!readToken(confLexer, TOKEN_TYPE_STRING)) return false;
		strMap[strIdent] = s2ws(confLexer->getCurrLexeme());
		return true;
	}
	
	bool SoraLocalizer::readLocaleResource(llexer* confLexer, LocaleStringMap& strMap) {
		util::String strIdent = confLexer->getCurrLexeme();
		if(!readToken(confLexer, TOKEN_TYPE_OP)) return false;
		if(!readToken(confLexer, TOKEN_TYPE_STRING)) return false;
		strMap[strIdent] = s2ws(confLexer->getCurrLexeme());
		return true;
	}
	
	void SoraLocalizer::reportError(llexer* lexer) {
		util::String strErrorMssg = ("Error loading locale file\nCurrent Line=");
		SORA->messageBox(util::String(strErrorMssg+lexer->getCurrLine()+"\nCurrent Char="+lexer->getCurrInvalidChar()),
						 "Error",
						 MB_OK | MB_ICONERROR);
	}
	
	bool SoraLocalizer::addLocaleConf(const util::String& confPath) {
		llexer* confLexer = new llexer;
		confLexer->addOperator("@", 1);
		confLexer->addOperator("=", 2);
		
		ulong32 confSize;
		void* confData = SoraCore::Instance()->getResourceFile(confPath, confSize);
		if(confData && confLexer->loadFileMem(confData, confSize) == LEX_NO_ERROR) {
			util::String localeIdent;
			LocaleStringMap localeStrMap;
			
			Token token = confLexer->getNextToken();
			while(token != TOKEN_TYPE_END_OF_STREAM && token != TOKEN_TYPE_INVALID_INPUT) {
				if(token == TOKEN_TYPE_OP) {
					if(confLexer->getCurrOp() == 1) {
						localeIdent = readLocaleIdent(confLexer);
					}
				} else if(token == TOKEN_TYPE_IDENT) {
					if(!readLocaleString(confLexer, localeStrMap)) {
						reportError(confLexer);
						return false;
					}
					
				} else if(token == TOKEN_TYPE_STRING) {
					if(readLocaleResource(confLexer, localeStrMap)) {
						reportError(confLexer);
						return false;
					}
				}
				token = confLexer->getNextToken();
			}
			
			delete confLexer;
			SORA->freeResourceFile(confData);
			
			if(localeIdent.size() != 0) {
				localeConfs[localeIdent].insert(localeStrMap.begin(), localeStrMap.end());
				if(currentLocale.size() == 0) {
					currentLocale = localeIdent;
					currentLocaleW = s2ws(currentLocale);
					currentLocaleMap = localeConfs.find(localeIdent);
					
					return true;
				}
			} else if(currentLocale.size() != 0) {
				localeConfs[currentLocale].insert(localeStrMap.begin(), localeStrMap.end());
				
				return true;
			} else
				THROW_SORA_EXCEPTION(NotFoundException, "no locale ident read");
		} else {
			sora::SORA->messageBoxW(L"Cannot find locale file: "+confPath, L"Error", MB_OK | MB_ICONERROR);
			return false;
		}
		return false;
	}
	
	util::String SoraLocalizer::getStr(const util::String& ident) {
		LocaleStringMap::iterator itStr = currentLocaleMap->second.find(ident);
		if(itStr != currentLocaleMap->second.end())
			return itStr->second;
		return std::wstring();
	}

	void SoraLocalizer::setCurrentLocale(const util::String& localeIdent) {
		LocaleConfMap::iterator itConf = localeConfs.find(localeIdent);
		if(itConf != localeConfs.end()) {
			currentLocale = localeIdent;
			currentLocaleW = s2ws(localeIdent);
			currentLocaleMap = itConf;
		}
	}
		
	util::String SoraLocalizer::localizeResourceName(util::String& resourceName) {
		size_t dotPos = resourceName.rfind(L'.');
		if(dotPos != std::string::npos) {
			resourceName.insert(dotPos, L"_"+currentLocaleW);
		}
		return resourceName;
	}


} // namespace sora