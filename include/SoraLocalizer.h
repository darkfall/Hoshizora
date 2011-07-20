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
	
	/*
		@locale file format for sora
	 
		file should contain a locale name start with @locale
		@locale = CHN etc
		if the file does not contain this line and there is a current locale set
		the locale of the file would be assigned as current locale
		otherwise a error would be generated
	 
		for every localized string, start with a identifier, then follows then localized string
		MAP1_LEVEL1_NAME = "Obey Or Die!" etc
	 
		for every resource file string, start with a string identifier, then follows then localized string
		"GameTitle.png"="GameTitle_CHN.png" etc
	 */
			
	class SORA_API SoraLocalizer {
		friend class SoraSingleton<SoraLocalizer>;
		
	public:
		static SoraLocalizer* Instance();
		static void Destroy();
		/*
		 add a locale configuration file
		 if the ident of the conf already exist, then the old one would be replaced
		 @param confPath, path of the configuration file
		 */
		bool addLocaleConf(const SoraWString& confPath);
		
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
		
		
		/*
		 add current locale identifier to the resource file
		 @param resourceName, the resource name to localize
		 for example, myResource.png would be localized as myResource_CHN.png if current locale is CHN
		 */
		SoraWString localizeResourceName(SoraWString& resourceName);
		
	private:
		static SoraLocalizer* mInstance;

		typedef hash_map<stringId, SoraWString> LocaleStringMap;		
		typedef map<SoraString, LocaleStringMap > LocaleConfMap;
		LocaleConfMap localeConfs;
		LocaleConfMap::iterator currentLocaleMap;
		
		SoraString currentLocale;
		SoraWString currentLocaleW;
		
	protected:
		inline bool readToken(llexer* lexer, Token tokenExpected);
		inline SoraString readLocaleIdent(llexer* lexer);
		inline bool readLocaleString(llexer* lexer, LocaleStringMap& strMap);
		inline bool readLocaleResource(llexer* lexer, LocaleStringMap& strMap);
		
		void reportError(llexer* lexer);
		
		inline const wchar_t* strToWideStr(const char*);
		
		SoraLocalizer() {}
		~SoraLocalizer();
	};
	
	static SoraLocalizer* SORA_LOCALIZER = SoraLocalizer::Instance();
	
#define GET_LOCAL_STR(ident) SORA_LOCALIZER->getStr(ident)
#define GET_LOCAL_RESOURCE(ident) SORA_LOCALIZER->getStr(ident)

#define LOCALIZE_RESOURCE_NAME(ident) SORA_LOCALIZER->localizeResourceName(ident)

} // namespace sora

#endif