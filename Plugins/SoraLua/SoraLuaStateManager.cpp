#include "SoraLuaStateManager.h"

namespace sora {

	ulong32 SoraLuaObjectManager::registerLuaObject(SoraLuaObject* obj) {
		luaObjectMap[(ulong32)obj] = obj;
		return (ulong32)obj;
	}

	void SoraLuaObjectManager::unregisterLuaObject(SoraLuaObject* obj) {
		LUA_OBJECT_MAP::iterator p = luaObjectMap.find((ulong32)obj);
		if(p != luaObjectMap.end()) {
			luaObjectMap.erase(p);
		}
	}

	SoraLuaObject* SoraLuaObjectManager::getLuaObject(ulong32 h) {
		LUA_OBJECT_MAP::iterator p = luaObjectMap.find(h);
		if(p != luaObjectMap.end())
			return p->second;
		return 0;
	}

	void SoraLuaObjectManager::sendMessage(const util::String& mssg, const util::String& params, ulong32 receiver) {
		LUA_OBJECT_MAP::iterator itObj = luaObjectMap.begin();
		
		while(itObj != luaObjectMap.end()) {
			if(receiver != 0) {
				if(receiver == (ulong32)itObj->second) {
					doSendMessage(mssg, params, itObj->second);
				}
			} else
				doSendMessage(mssg, params, itObj->second);

			++itObj;
		}
	}

	void SoraLuaObjectManager::doSendMessage(const util::String& mssg, const util::String& params, SoraLuaObject* obj) {
		LuaPlus::LuaObject luaObj = obj->get(mssg);
		if(luaObj.IsFunction()) {
			LuaPlus::LuaFunction<void> mssgFunc = luaObj;
			mssgFunc(params);
		}
	}

} // namespace sora