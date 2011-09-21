#ifndef SORA_TEXTURE_MAP_H
#define SORA_TEXTURE_MAP_H

#include "SoraPlatform.h"
#include "SoraTexture.h"
#include "SoraSingleton.h"
#include "SoraException.h"
#include "SoraEnvValues.h"
#include "SoraStringId.h"
#include "SoraCore.h"
#include <map>

/* hash functions */
#include "util/SoraHash.h"

namespace sora {
	
	/*
	 texture map poll for faster texture access
	 */
	class SORA_API SoraTextureMap: public SoraSingleton<SoraTextureMap> {
		friend class SoraSingleton<SoraTextureMap>;

	public:
		~SoraTextureMap() {
			for(TEX_MAP::iterator p = texMap.begin(); p != texMap.end(); ++p) {
                SoraCore::Instance()->releaseTexture(p->second);
			}
			texMap.clear();
			texMapRv.clear();
		}
		
		void clear() {
			for(TEX_MAP::iterator p = texMap.begin(); p != texMap.end(); ++p) {
				if(texRefs[p->first] == 0) {
					SoraCore::Instance()->releaseTexture(p->second);
#ifdef _DEBUG
					log_mssg(vamssg("Free Texture: %lu, tex=%s\n", p->second, (id2str(p->second)).c_str()));
#endif
					texMapRv.erase(texMapRv.find(p->second));
					texMap.erase(p);
				}
			}
		}

		void add(const StringType& key, SoraTextureHandle tex) {			
			SoraStringId uiKey = key.uniqueId();
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
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", (int32)texMap.size());
		}
		
		void remove(SoraTextureHandle tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {					
				texRefs.erase(texRefs.find(p->second));
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", (int32)texMap.size());
		}
		
		void del(SoraTextureHandle tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs.erase(texRefs.find(p->second));
				texMap.erase(texMap.find(p->second));
				texMapRv.erase(p);
			}
			
			SET_ENV_INT("CORE_TEXMAP_SIZE", (int32)texMap.size());
		}
		
		void addRf(const StringType& key) {
			SoraStringId uiKey = key.uniqueId();
			texRefs[uiKey]++;
		}
		
		void addRf(SoraTextureHandle tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs[p->second]++;
			}
		}
		
		void decRf(SoraTextureHandle tex) {
			TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
				texRefs[p->second]--;
				
				if(texRefs[p->second] == 0) {
					SORA->releaseTexture(p->first);
					remove(tex);
				}
			
			}
		}
		
		SoraTextureHandle get(const StringType& key) {
			SoraStringId uiKey = key.uniqueId();
			TEX_MAP::iterator p = texMap.find(uiKey);
			if(p != texMap.end()) {
				//SoraTexture* tex = (SoraTexture*)p->second;
				texRefs[uiKey]++;
				return p->second;
			}
			return 0;
		}
        
        bool exist(SoraTextureHandle tex) {
            TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {	
                return true;
            }
            return false;
        }
        
        SoraStringId getTextureName(SoraTextureHandle tex) {
            TEXMAP_RV::iterator p = texMapRv.find(tex);
			if(p != texMapRv.end()) {
                return p->second;
            }
            return 0;
        }
		
	private:
		typedef sora_hash_map<SoraStringId, SoraTextureHandle> TEX_MAP;
		TEX_MAP texMap;
		typedef sora_hash_map<SoraTextureHandle, SoraStringId> TEXMAP_RV;
		TEXMAP_RV texMapRv;
		
		typedef sora_hash_map<SoraStringId, int32> TEX_USE_COUNT;
		TEX_USE_COUNT texRefs;
	};
};

#endif