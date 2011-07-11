#ifndef SORA_TEXTURE_MAP_H
#define SORA_TEXTURE_MAP_H

#include "SoraTexture.h"
#include "SoraSingleton.h"
#include "SoraException.h"
#include "SoraCore.h"
#include "SoraPlatform.h"
#include "SoraEnvValues.h"

#include "stringId.h"
#include <map>

/* hash functions */
#include "hash.h"

namespace sora {
	
	/*
	 texture map poll for faster texture access
	 */
	class SORA_API SoraTextureMap: public SoraSingleton<SoraTextureMap> {
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
				if(texRefs[p->first] == 0) {
					SoraCore::Instance()->releaseTexture(p->second);
#ifdef _DEBUG
					DebugPtr->log(vamssg("Free Texture: %lu, tex=%s\n", p->second, (id2str(p->second)).c_str()));
#endif
					texMapRv.erase(texMapRv.find(p->second));
					texMap.erase(p);
				}
			}
		}

		void add(const SoraWString& key, HSORATEXTURE tex) {			
			stringId uiKey = str2id(key);
			TEX_MAP::iterator pos = texMap.find(uiKey);
			if(pos == texMap.end()) {
				texMap[uiKey] = tex;
				texMapRv[tex] = uiKey;
				texRefs[uiKey] = 0;
			}
			else {
				del(pos->second);
				texMap[uiKey] = tex;
				texMapRv[tex] = uiKey;
				texRefs[uiKey] = 0;
			}
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", texMap.size());
		}
		
		void remove(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {					
				texRefs.erase(texRefs.find(p->second));
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", texMap.size());
		}
		
		void del(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs.erase(texRefs.find(p->second));
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", texMap.size());
		}
		
		void addRf(const SoraWString& key) {
			stringId uiKey = str2id(key);
			texRefs[uiKey]++;
		}
		
		void addRf(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs[p->second]++;
			}
		}
		
		void decRf(HSORATEXTURE tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs[p->second]--;
				
				if(texRefs[p->second] == 0) {
					SORA->releaseTexture(p->first);
					remove(tex);
				}
			
			}
		}
		
		HSORATEXTURE get(const SoraWString& key) {
			stringId uiKey = str2id(key);
			TEX_MAP::iterator p = texMap.find(uiKey);
			if(p != texMap.end()) {
				//SoraTexture* tex = (SoraTexture*)p->second;
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