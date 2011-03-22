/*
 *  SoraLocalizer.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/14/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraLocalizer.h"

namespace sora {
	
	SoraLocalizer::~SoraLocalizer() {
	}
	
	bool SoraLocalizer::readToken(llexer* lexer, Token tokenExpected) {
		return (lexer->getNextToken() == tokenExpected);
	}
	
	const wchar_t* SoraLocalizer::strToWideStr(const char* str) {
		size_t _Dsize = strlen(str);
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest, str, _Dsize);
		//_Dest[_Dsize] = L'\0';
		return _Dest;
	}
	
	SoraString SoraLocalizer::readLocaleIdent(llexer* confLexer) {
		readToken(confLexer, TOKEN_TYPE_IDENT);
		if(strcmp(confLexer->getCurrLexeme(), "locale") == 0) {
			readToken(confLexer, TOKEN_TYPE_OP);
			readToken(confLexer, TOKEN_TYPE_IDENT);
#ifdef _DEBUG
			printf("read locale: %s\n", confLexer->getCurrLexeme());
#endif
			return confLexer->getCurrLexeme();
		}
		return "\0";
	}
	
	void SoraLocalizer::readLocaleString(llexer* confLexer, LOCALE_STRING_MAP& strMap) {
		SoraString strIdent = confLexer->getCurrLexeme();
		readToken(confLexer, TOKEN_TYPE_OP);
		readToken(confLexer, TOKEN_TYPE_STRING);
		strMap[strIdent] = s2ws(confLexer->getCurrLexeme());
#ifdef _DEBUG
		printf("read locale str: %s=%s\n", strIdent.c_str(), ws2s(strMap[strIdent]).c_str());
#endif
	}
	
	void SoraLocalizer::addLocaleConf(const SoraWString& confPath) {
		llexer* confLexer = new llexer;
		confLexer->addOperator("@", 1);
		confLexer->addOperator("=", 2);
		
		ulong32 confSize;
		void* confData = SoraCore::Instance()->getResourceFile(confPath, confSize);
		if(confData && confLexer->loadFileMem(confData, confSize) == LEX_NO_ERROR) {
			SoraString localeIdent;
			LOCALE_STRING_MAP localeStrMap;
			
			Token token = confLexer->getNextToken();
			while(token != TOKEN_TYPE_END_OF_STREAM && token != TOKEN_TYPE_INVALID_INPUT) {
				if(token == TOKEN_TYPE_OP) {
					if(confLexer->getCurrOp() == 1) {
						localeIdent = readLocaleIdent(confLexer);
					}
				} else if(token == TOKEN_TYPE_IDENT) {
					readLocaleString(confLexer, localeStrMap);
				}
				token = confLexer->getNextToken();
			}
			
			delete confLexer;
			SORA->freeResourceFile(confData);
			
			if(localeIdent.size() != 0) {
				localeConfs[localeIdent] = localeStrMap;
				if(currentLocale.size() == 0) {
					currentLocale = localeIdent;
					currentLocaleW = s2ws(currentLocale);
					currentLocaleMap = localeConfs.find(localeIdent);
				}
			}
			else
				throw SORA_EXCEPTION("no locale ident read");
		} else {
#ifdef _DEBUG
			printf("Error loading locale configuration file\n");
#endif
		}
	}
	
	SoraWString SoraLocalizer::getStr(const SoraString& ident) {
		LOCALE_STRING_MAP::iterator itStr = currentLocaleMap->second.find(ident);
		if(itStr != currentLocaleMap->second.end())
			return itStr->second;
		return L"\0";
	}

	void SoraLocalizer::setCurrentLocale(const SoraString& localeIdent) {
		LOCALE_CONF_MAP::iterator itConf = localeConfs.find(localeIdent);
		if(itConf != localeConfs.end()) {
			currentLocale = localeIdent;
			currentLocaleW = s2ws(localeIdent);
			currentLocaleMap = itConf;
		}
	}
		
	void SoraLocalizer::localizeResourceName(SoraWString& resourceName) {
		size_t dotPos = resourceName.rfind(L'.');
		if(dotPos != std::string::npos) {
			resourceName.insert(dotPos, L"_"+currentLocaleW);
		}
	}


} // namespace sora