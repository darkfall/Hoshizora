/*
 *  SoraLocalizedString.h
 *  Sora
 *
 *  Created by GriffinBu on 1/14/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_LOCALIZED_STRING_H_
#define SORA_LOCALIZED_STRING_H_

#include "SoraPlatform.h"
#include "SoraStringConv.h"
#include "SoraLocalizer.h"

namespace sora {
	
	class SoraLocalizedString {
		friend class SoraLocalizer;
	public:
		SoraLocalizedString(): bInternalStr(false) {}
		SoraLocalizedString(const SoraString& ident): localeIdent(ident), localizedString(GET_LOCAL_STR(ident)), bInternalStr(false) {
			if(localizedString.empty())
				setString(s2ws(ident));
			else
				SORA_LOCALIZER->registerLocalizedString(this);
		}
		SoraLocalizedString(const SoraLocalizedString& rhs): localeIdent(rhs.localeIdent), localizedString(GET_LOCAL_STR(rhs.localeIdent)), bInternalStr(rhs.bInternalStr) {
			if(!localizedString.empty() && !bInternalStr)
				SORA_LOCALIZER->registerLocalizedString(this);
		}
		~SoraLocalizedString() {
			if(!localizedString.empty() && !bInternalStr)
				SORA_LOCALIZER->unregisterLocalizedString(this);
		}

		/*
		 set the locale of current string
		 if applied, then the content of the string would not change when app locale changed
		 @param localIdent, the identifier of the locale to set, depends on locale configuration files
		 */
		void setLocale(const SoraString& localeIdent) {
			this->localeIdent = localeIdent;
			localizedString = GET_LOCAL_STR(localeIdent);
			//SORA_LOCALIZER->registerLocalizedString(this);
		}
		
		/*
		 operator = 
		 @param str, the ident id of the str in locale configuration files
			etc, MENU_START=Start
			then the ident id is MENU_START
		 */
		SoraLocalizedString operator=(const char* pstr) {
			localeIdent = pstr;
			localizedString = GET_LOCAL_STR(localeIdent);
			if(!localizedString.empty())
				setString(s2ws(pstr));
			return *this;
		}
		
		SoraLocalizedString operator=(const SoraString& str) {
			localeIdent = str;
			localizedString = GET_LOCAL_STR(localeIdent);
			if(!localizedString.empty())
				setString(s2ws(str));
			return *this;
		}

		SoraLocalizedString operator=(const SoraLocalizedString& localStr) {
			localeIdent = localStr.localeIdent;
			localizedString = localStr.localizedString;
			bInternalStr = localStr.bInternalStr;
			
			if(!localizedString.empty() && !bInternalStr)
				SORA_LOCALIZER->registerLocalizedString(this);
#ifdef _DEBUG
			printf("%s\n", ws2s(localizedString).c_str());
#endif
			return *this;
		}
		
		const wchar_t* operator*() const {
			if(bInternalStr)
				return internalStr.c_str();
			return localizedString.c_str();
		}
		
		const wchar_t* c_str() const {
			if(bInternalStr)
				return internalStr.c_str();
			return localizedString.c_str();
		}
		
		SoraWString getStr() const {
			return localizedString;
		}
		
		void setString(const SoraWString& str) {
			internalStr = str;
			bInternalStr = true;
			
			//SORA_LOCALIZER->unregisterLocalizedString(this);
		}
		
	private:		
		SoraWString localizedString;
		SoraString localeIdent;
		
		SoraWString internalStr;
		bool bInternalStr;
	};
	
} // namespace sora

#endif