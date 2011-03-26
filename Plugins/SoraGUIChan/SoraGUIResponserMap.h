#ifndef SORA_GUI_RESPONSER_MAP_H
#define SORA_GUI_RESPONSER_MAP_H

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include "SoraGUIResponser.h"

#include <map>

namespace sora {
    
	class SoraGUIResponserMap: public SoraSingleton<SoraGUIResponserMap> {
		friend class SoraSingleton<SoraGUIResponserMap>;

	public:
		~SoraGUIResponserMap() {
			_resMap.clear();
		}

		void registerResponser(const SoraString& id, SoraGUIResponser* responser) {
			_resMap.insert(std::make_pair<stringId, SoraGUIResponser*>(str2id(id), responser));
		}
		SoraGUIResponser* getResponser(const SoraString& id) {
            ResponserMap::iterator itResponser = _resMap.find(str2id(id));
            if(itResponser != _resMap.end())
                return itResponser->second;
			return 0;
		}

        // in xml, use responser = "@handleType=src" to indicate this gui uses a external responser
		void registerExternalResponser(const SoraString& handleType, SoraGUIResponserExtern* responser) {
			_extMap.insert(std::make_pair<stringId, SoraGUIResponserExtern*>(str2id(handleType), responser));
		}
        SoraGUIResponserExtern* getExternalResponser(const SoraString& handleType) {
            ExternalResponserMap::iterator itResponser = _extMap.find(str2id(handleType));
            if(itResponser != _extMap.end())
                return itResponser->second;
			return 0;
        }
        
        stringId getHandleSrcFromWidgetHandle(ulong32 handle) {
            HandleSrcMap::iterator itHandleSrc = widgetSrc.find(handle);
            if(itHandleSrc != widgetSrc.end())
                return itHandleSrc->second;
            return 0;
        }
        
        void registerHandleSrc(ulong32 widget, const SoraString& src) {
            widgetSrc[widget] = str2id(src);
        }
        
	private:
        typedef hash_map<ulong32, stringId> HandleSrcMap;
        HandleSrcMap widgetSrc;
        
		typedef std::map<stringId, SoraGUIResponser*> ResponserMap;
		ResponserMap _resMap;
    
        typedef std::map<stringId, SoraGUIResponserExtern*> ExternalResponserMap;
		ExternalResponserMap _extMap;
	};
} // namespace sora

#endif