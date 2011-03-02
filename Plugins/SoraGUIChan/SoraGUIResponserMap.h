#ifndef SORA_GUI_RESPONSER_MAP
#define SORA_GUI_RESPONSER_MAP

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
			for(ResponserMap::iterator p = _resMap.begin(); p != _resMap.end(); ++p) {
				if(p->second) delete p->second;
			}
			_resMap.clear();
		}

		void registerResponser(const SoraString& id, SoraGUIResponser* responser) {
			_resMap[id] = responser;
		}
		SoraGUIResponser* getResponser(const SoraString& id) {
			return _resMap[id];
		}

		void registerLuaResponser(ulong32 h, const SoraString& path) {
			_scrMap[h] = path;
		}
		SoraString getLuaResponser(ulong32 h) {
			return _scrMap[h];
		}

	private:
		typedef std::map<SoraString, SoraGUIResponser*> ResponserMap;
		typedef std::map<ulong32, SoraString> LuaScriptMap;
		ResponserMap _resMap;
		LuaScriptMap _scrMap;
	};
} // namespace sora

#endif