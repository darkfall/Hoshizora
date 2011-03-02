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

namespace sora {
	
	typedef ulong32 stringId;

	class SoraStringManager: public SoraSingleton<SoraStringManager> {
		friend class SoraSingleton<SoraStringManager>;
		
	public:
		inline stringId getStringId(const SoraString& str) {
			stringId sid = crc32(str);
			if(strings.find(sid) == strings.end()) {
#ifdef _DEBUG
				printf("mapping str, id=%lu, str=%s\n", sid, str.c_str());
#endif
				strings[sid] = s2ws(str);
			}
			return sid;
		}
		
		inline stringId getStringIdNoCache(const SoraString& str) {
			return crc32(str);
		}
		
		inline stringId getStringId(const SoraWString& str) {
			stringId sid = crc32(str);

			if(strings.find(sid) == strings.end()) {
				strings[sid] = str;
#ifdef _DEBUG
				printf("mapping str, id=%lu, str=%s\n", sid, ws2s(str).c_str());
#endif
			}
			return sid;
		}
		
		inline stringId getStringIdNoCache(const SoraWString& str) {
			return crc32(str);
		}
		
		inline const wchar_t* getStringByIdW(stringId sid) {
			if(sid == 0) return NULL;
			STR_MAP::iterator itStr = strings.find(sid);
			if(itStr != strings.end())
				return itStr->second.c_str();
			return NULL;
		}
		
		inline const char* getStringById(stringId sid) {
			if(sid == 0) return NULL;
			STR_MAP::iterator itStr = strings.find(sid);
			if(itStr != strings.end())
				return ws2s(itStr->second).c_str();
			return 0;
		}
		
	private:
		typedef hash_map<ulong32, SoraWString> STR_MAP;
		STR_MAP strings;
	};
	
	static SoraStringManager* SORA_STR_MANAGER = SoraStringManager::Instance();
	#define str2id(str) SORA_STR_MANAGER->getStringId(str)
	#define id2strw(id) SORA_STR_MANAGER->getStringByIdW(id)
	#define id2str(id) SORA_STR_MANAGER->getStringById(id)
	#define str2idnc(str) SORA_STR_MANAGER->getStringIdNoCache(str)
} // namespace sora

#endif