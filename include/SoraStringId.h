/*
 *  SoraStringId.h
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
#include "util/SoraHash.h"

namespace sora {
	
	typedef uint64 stringId;
    typedef stringId SoraStringId;

	class SoraStringManager: public SoraSingleton<SoraStringManager> {
		friend class SoraSingleton<SoraStringManager>;
		
	public:
        // explicit add strings, no hash implied, be care
        inline void addString(const std::string& str, SoraStringId sid) {
            strings[sid] = s2ws(str);
        }
        
        inline void addString(const std::wstring& str, SoraStringId sid) {
            strings[sid] = str;
        }
        
		inline SoraStringId getStringId(const std::string& str) {
			SoraStringId sid = crc32(str);
			if(strings.find(sid) == strings.end()) {
				strings[sid] = s2ws(str);
			}
			return sid;
		}
		
		static SoraStringId getStringIdNoCache(const std::string& str) {
			return crc32(str);
		}
		
		inline SoraStringId getStringId(const std::wstring& str) {
			SoraStringId sid = crc32(str);

			if(strings.find(sid) == strings.end()) {
				strings[sid] = str;
			}
			return sid;
		}
		
		static SoraStringId getStringIdNoCache(const std::wstring& str) {
			return crc32(str);
		}
		
		inline std::wstring getStringByIdW(SoraStringId sid) {
			if(sid == 0) return NULL;
			std::wstring str = strings[sid];
			if(str.size() != 0)
				return str;
			return std::wstring();;
		}
		
		inline std::string getStringById(SoraStringId sid) {
			if(sid == 0) return NULL;
			std::string str = ws2s(strings[sid]);
			if(str.size() != 0)
				return str;
			return std::string();
		}
		
	private:
		typedef hash_map<SoraStringId, std::wstring> STR_MAP;
		STR_MAP strings;
	};
	
    #define SORA_STR_MANAGER SoraStringManager::Instance()
	#define str2id(str) SORA_STR_MANAGER->getStringId(str)
	#define id2strw(id) SORA_STR_MANAGER->getStringByIdW(id)
	#define id2str(id) SORA_STR_MANAGER->getStringById(id)
	#define str2idnc(str) SORA_STR_MANAGER->getStringIdNoCache(str)
    
    static SoraStringId GetUniqueStringId(const std::string& name) {
        return SoraStringManager::Instance()->getStringId(name);
    }
    
    static SoraStringId GetUniqueStringId(const std::wstring& name) {
        return SoraStringManager::Instance()->getStringId(name);
    }
    
    static SoraString GetStringByUniqueId(SoraStringId sid) {
        return SoraStringManager::Instance()->getStringById(sid);
    }
    
    static SoraWString GetStringByUniqueIdW(SoraStringId sid) {
        return SoraStringManager::Instance()->getStringByIdW(sid);
    }
} // namespace sora

#endif