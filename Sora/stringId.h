/*
 *  stringId.h
 *  Sora
 *
 *  Created by GriffinBu on 1/21/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_STRING_ID_H_
#define SORA_STRING_ID_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "hash.h"
#include "SoraStringConv.h"

namespace sora {
	
	typedef ulong64 stringId;

	class SoraStringManager: public SoraSingleton<SoraStringManager> {
		friend class SoraSingleton<SoraStringManager>;
		
	public:
        // explicit add strings, no hash implied, be care
        inline void addString(const SoraString& str, stringId sid) {
            strings[sid] = s2ws(str);
        }
        
        inline void addString(const SoraWString& str, stringId sid) {
            strings[sid] = str;
        }
        
		inline stringId getStringId(const SoraString& str) {
			stringId sid = crc32(str);
			if(strings.find(sid) == strings.end()) {
				strings[sid] = s2ws(str);
			}
			return sid;
		}
		
		static stringId getStringIdNoCache(const SoraString& str) {
			return crc32(str);
		}
		
		inline stringId getStringId(const SoraWString& str) {
			stringId sid = crc32(str);

			if(strings.find(sid) == strings.end()) {
				strings[sid] = str;
			}
			return sid;
		}
		
		static stringId getStringIdNoCache(const SoraWString& str) {
			return crc32(str);
		}
		
		inline SoraWString getStringByIdW(stringId sid) {
			if(sid == 0) return NULL;
			std::wstring str = strings[sid];
			if(str.size() != 0)
				return str;
			return L"???\0";
		}
		
		inline SoraString getStringById(stringId sid) {
			if(sid == 0) return NULL;
			std::string str = ws2s(strings[sid]);
			if(str.size() != 0)
				return str;
			return "???\0";
		}
		
	private:
		typedef hash_map<stringId, SoraWString> STR_MAP;
		STR_MAP strings;
	};
	
    #define SORA_STR_MANAGER SoraStringManager::Instance()
	#define str2id(str) SORA_STR_MANAGER->getStringId(str)
	#define id2strw(id) SORA_STR_MANAGER->getStringByIdW(id)
	#define id2str(id) SORA_STR_MANAGER->getStringById(id)
	#define str2idnc(str) SORA_STR_MANAGER->getStringIdNoCache(str)
} // namespace sora

#endif