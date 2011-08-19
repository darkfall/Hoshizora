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
#include "SoraStringConv.h"
#include "hash.h"

namespace sora {
	
	typedef uint64 stringId;
    typedef stringId SoraStringId;

	class SoraStringManager: public SoraSingleton<SoraStringManager> {
		friend class SoraSingleton<SoraStringManager>;
		
	public:
        // explicit add strings, no hash implied, be care
        inline void addString(const std::string& str, stringId sid) {
            strings[sid] = s2ws(str);
        }
        
        inline void addString(const std::wstring& str, stringId sid) {
            strings[sid] = str;
        }
        
		inline stringId getStringId(const std::string& str) {
			stringId sid = crc32(str);
			if(strings.find(sid) == strings.end()) {
				strings[sid] = s2ws(str);
			}
			return sid;
		}
		
		static stringId getStringIdNoCache(const std::string& str) {
			return crc32(str);
		}
		
		inline stringId getStringId(const std::wstring& str) {
			stringId sid = crc32(str);

			if(strings.find(sid) == strings.end()) {
				strings[sid] = str;
			}
			return sid;
		}
		
		static stringId getStringIdNoCache(const std::wstring& str) {
			return crc32(str);
		}
		
		inline std::wstring getStringByIdW(stringId sid) {
			if(sid == 0) return NULL;
			std::wstring str = strings[sid];
			if(str.size() != 0)
				return str;
			return std::wstring();;
		}
		
		inline std::string getStringById(stringId sid) {
			if(sid == 0) return NULL;
			std::string str = ws2s(strings[sid]);
			if(str.size() != 0)
				return str;
			return std::string();
		}
		
	private:
		typedef hash_map<stringId, std::wstring> STR_MAP;
		STR_MAP strings;
	};
	
    #define SORA_STR_MANAGER SoraStringManager::Instance()
	#define str2id(str) SORA_STR_MANAGER->getStringId(str)
	#define id2strw(id) SORA_STR_MANAGER->getStringByIdW(id)
	#define id2str(id) SORA_STR_MANAGER->getStringById(id)
	#define str2idnc(str) SORA_STR_MANAGER->getStringIdNoCache(str)
    
    static stringId GetUniqueStringId(const std::string& name) {
        return SORA_STR_MANAGER->getStringId(name);
    }
    
    static stringId GetUniqueStringId(const std::wstring& name) {
        return SORA_STR_MANAGER->getStringId(name);
    }
    
    static SoraString GetStringByUniqueId(stringId sid) {
        return SORA_STR_MANAGER->getStringById(sid);
    }
    
    static SoraWString GetStringByUniqueIdW(stringId sid) {
        return SORA_STR_MANAGER->getStringByIdW(sid);
    }
} // namespace sora

#endif