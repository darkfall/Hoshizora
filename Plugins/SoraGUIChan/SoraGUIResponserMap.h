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
			return _resMap[str2id(id)];
		}

		void registerLuaResponser(ulong32 h, const SoraString& path) {
			_scrMap[h] = path;
		}
		SoraString getLuaResponser(ulong32 h) {
			return _scrMap[h];
		}

	private:
		typedef std::map<stringId, SoraGUIResponser*> ResponserMap;
		typedef std::map<ulong32, SoraString> LuaScriptMap;
		ResponserMap _resMap;
		LuaScriptMap _scrMap;
	};
} // namespace sora

#endif