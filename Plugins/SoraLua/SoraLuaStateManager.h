#ifndef SORA_LUA_STATE_MANAGER_H
#define SORA_LUA_STATE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraLuaObject.h"

#include <map>

namespace sora {

	using std::map;
	using LuaPlus::LuaState;

	class SoraLuaObjectManager: public SoraSingleton<SoraLuaObjectManager> {
		friend class SoraSingleton<SoraLuaObjectManager>;

	public:
		ulong32 registerLuaObject(SoraLuaObject* obj);
		void unregisterLuaObject(SoraLuaObject* obj);
		SoraLuaObject* getLuaObject(ulong32 h);

		void sendMessage(const util::String& message, const util::String& params, ulong32 receiver=0);

	private:
		void doSendMessage(const util::String& message, const util::String& params, SoraLuaObject* obj);

		typedef map<ulong32, SoraLuaObject*> LUA_OBJECT_MAP;

		LUA_OBJECT_MAP luaObjectMap;
	};


} // namespace sora

#endif