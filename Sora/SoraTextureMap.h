#ifndef SORA_TEXTURE_MAP_H
#define SORA_TEXTURE_MAP_H

#include "SoraTexture.h"
#include "SoraSingleton.h"
#include "SoraException.h"
#include "SoraCore.h"
#include "SoraPlatform.h"

#include "stringId.h"
#include <map>

/* hash functions */
#include "hash.h"

namespace sora {
	
	/*
	 texture map poll for faster texture access
	 */
	class SoraTextureMap: public SoraSingleton<SoraTextureMap> {
		friend class SoraSingleton<SoraTextureMap>;

	public:
		~SoraTextureMap() {
			for(TEX_MAP::iterator p = texMap.begin(); p != texMap.end(); ++p) {
				SORA->releaseTexture(p->second);
			}
			texMap.clear();
			texMapRv.clear();
		}
		
		void clear() {
			for(TEX_MAP::iterator p = texMap.begin(); p != texMap.end(); ++p) {
				//printf("tex rf: %d, %d\n", p->second, texRefs[p->first]);
				if(texRefs[p->first] == 0) {
					SoraCore::Instance()->releaseTexture(p->second);
#ifdef _DEBUG
					printf("Free Texture: %lu, tex=%s\n", p->second, (id2str(p->second)));
#endif
					texMapRv.erase(texMapRv.find(p->second));
					texMap.erase(p);
				}
			}
		}

		void add(const SoraWString& key, HSORATEXTURE tex) {
			stringId uiKey = str2id(key);
			TEX_MAP::iterator pos = texMap.find(uiKey);
			//wcscpy(((SoraTexture*)tex)->name, key.c_str());
			if(pos == texMap.end()) {
				texMap[uiKey] = tex;
				texMapRv[tex] = uiKey;
				texRefs[uiKey]++;
			}
			else {
				del(pos->second);
				texMap[uiKey] = tex;
				texMapRv[tex] = uiKey;
				texRefs[uiKey]++;
			}
		}
		
		void remove(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {					
				texRefs.erase(texRefs.find(p->second));
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
		}
		
		void del(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
		}
		
		void addRf(const SoraWString& key) {
			stringId uiKey = str2id(key);
			texRefs[uiKey]++;
		}
		
		void decRf(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs[p->second]--;
				if(texRefs[p->second] == 0) {
					SORA->releaseTexture(p->first);
				}
				printf("tex: %lu, ref: %d\n", tex, texRefs[p->second]);
				remove(tex);
			}
		}
		
		HSORATEXTURE get(const SoraWString& key) {
			stringId uiKey = str2id(key);
			TEX_MAP::iterator p = texMap.find(uiKey);
			if(p != texMap.end()) {
				texRefs[uiKey]++;
				return p->second;
			}
			return 0;
		}
        
        bool exist(HSORATEXTURE tex) {
            TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {	
                return true;
            }
            return false;
        }
        
        stringId getTextureName(HSORATEXTURE tex) {
            TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
                return p->second;
            }
            return 0;
        }
		
	private:
		typedef hash_map<stringId, HSORATEXTURE> TEX_MAP;
		TEX_MAP texMap;
		typedef hash_map<HSORATEXTURE, stringId> TEXMAP_RV;
		TEXMAP_RV texMapRv;
		
		typedef hash_map<stringId, int32> TEX_USE_COUNT;
		TEX_USE_COUNT texRefs;
	};
};

#endif