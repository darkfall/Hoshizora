/*
 *  SoraLocalizer.h
 *  Sora
 *
 *  Created by GriffinBu on 1/14/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_LOCALIZER_H_
#define SORA_LOCALIZER_H_

#include "SoraPlatform.h"
#include "SoraStringConv.h"
#include "SoraSingleton.h"
#include "SoraCore.h"
#include "llexer/llexer.h"

#include <map>

namespace sora {
	
	class SoraLocalizedString;
		
	class SoraLocalizer: public SoraSingleton<SoraLocalizer> {
		friend class SoraSingleton<SoraLocalizer>;
		friend class SoraLocalizedString;
		
	public:
		/*
		 add a locale configuration file
		 if the ident of the conf already exist, then the old one would be replaced
		 @param confPath, path of the configuration file
		 */
		void addLocaleConf(const SoraWString& confPath);
		
		/*
		 get a str depend on current locale
		 @param ident, string ident id in the configuration file
		 */
		SoraWString getStr(const SoraString& ident);
		
		/*
		 set current locale, this would affect the result of getStr
		 @param localeIdent, locale ident id in the configuration file
		 */
		void setCurrentLocale(const SoraString& localeIdent);
		SoraString getCurrentLocale() const { return currentLocale; }
		
		void localizeResourceName(SoraWString& resourceName);
		
	private:
		typedef std::map<SoraString, SoraWString> LOCALE_STRING_MAP;		
		typedef std::map<SoraString, LOCALE_STRING_MAP > LOCALE_CONF_MAP;
		LOCALE_CONF_MAP localeConfs;
		LOCALE_CONF_MAP::iterator currentLocaleMap;
		
		SoraString currentLocale;
		SoraWString currentLocaleW;
		
		void registerLocalizedString(SoraLocalizedString* pLocalizedStr);
		void unregisterLocalizedString(SoraLocalizedString* pLocalizedStr);
		
		typedef std::map<ulong32, SoraLocalizedString*> LOCALIZED_STRING_MAP;
		LOCALIZED_STRING_MAP localizedStrs;
		
	protected:
		inline bool readToken(llexer* lexer, Token tokenExpected);
		inline SoraString readLocaleIdent(llexer* lexer);
		inline void readLocaleString(llexer* lexer, LOCALE_STRING_MAP& strMap);
		
		inline const wchar_t* strToWideStr(const char*);
		
		SoraLocalizer() {}
		~SoraLocalizer();
	};
	
	static SoraLocalizer* SORA_LOCALIZER = SoraLocalizer::Instance();
#define GET_LOCAL_STR(ident) SORA_LOCALIZER->getStr(ident)
#define GET_LOCAL_RESOURCE(ident) SORA_LOCALIZER->localizeResourceName(ident)

} // namespace sora

#endif