#include "SoraLuaObject.h"

#include "LuaPlus/src/lua.h"

extern "C" {
#include "pluto.h"
//#pragma comment(lib, "Libs/pluto.lib")
}

#include "SoraLuaStateManager.h"
#include "SoraLuaExport.h"

namespace sora {
	SoraLuaObject::SoraLuaObject() {
		luaState = LuaState::Create(true);
		if(!luaState)
			throw SORA_EXCEPTION("error creating LuaState");

		SoraLuaExport::export_constants(luaState);
		SoraLuaExport::export_soracore(luaState);
		SoraLuaExport::export_sorafiles(luaState);
		SoraLuaExport::export_soraobjects(luaState);

		luaopen_pluto(luaState->GetCState());

		type = OBJ_LUA;
		LuaStateRefMap[(ulong32)luaState].inc();
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
	}

	SoraLuaObject::SoraLuaObject(const SoraWString& scriptPath) {
		luaState = LuaState::Create(true);
		if(!luaState)
			throw SORA_EXCEPTION("error creating LuaState");
		
		SoraLuaExport::export_constants(luaState);
		SoraLuaExport::export_soracore(luaState);
		SoraLuaExport::export_sorafiles(luaState);
		SoraLuaExport::export_soraobjects(luaState);

		luaopen_pluto(luaState->GetCState());

		type = OBJ_LUA;
		LuaStateRefMap[(ulong32)luaState].inc();
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
	}

	SoraLuaObject::SoraLuaObject(LuaState* state) {
		luaState = state;
		LuaStateRefMap[(ulong32)luaState].inc();
		if(!luaState)
			throw SORA_EXCEPTION("error creating LuaState");
		type = OBJ_LUA;
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
	}

	SoraLuaObject::~SoraLuaObject() {
		LuaStateRefMap[(ulong32)luaState].dec();
		if(LuaStateRefMap[(ulong32)luaState] == 0)
			LuaState::Destroy(luaState);
		SoraLuaObjectManager::Instance()->unregisterLuaObject(this);
	}

	int32 SoraLuaObject::doScript(const SoraWString& scriptPath) {
		return luaState->DoFile(ws2s(scriptPath).c_str());
	}

	int32 SoraLuaObject::doString(const SoraString& str) {
		return luaState->DoString(str.c_str());
	}

	int32 SoraLuaObject::doBuffer(const char* buff, size_t size, const char* name) {
		return luaState->DoBuffer(buff, size, name);
	}

	int32 SoraLuaObject::loadScript(const SoraWString& scriptPath) {
		return luaState->LoadFile(ws2s(scriptPath).c_str());
	}

	int32 SoraLuaObject::loadString(const SoraString& str) {
		return luaState->LoadString(str.c_str());
	}

	int32 SoraLuaObject::loadBuffer(const char* buffer, size_t size, const char* name) {
		return luaState->LoadBuffer(buffer, size, name);
	}

	LuaObject SoraLuaObject::createThread() {
		return luaState->CreateThread(luaState);
	}

	LuaStackObject SoraLuaObject::createTable(int narr, int nrec) {
		return luaState->CreateTable(narr, nrec);
	}

	LuaStackObject SoraLuaObject::createUserData(size_t size) {
		return luaState->NewUserData(size);
	}

	LuaObject SoraLuaObject::getGlobals() {
		return luaState->GetGlobals();
	}

	LuaObject SoraLuaObject::get(const SoraString& name) {
		return luaState->GetGlobal(name.c_str());
	}

	LuaObject SoraLuaObject::operator[](const SoraString& name) {
		return luaState->GetGlobal(name.c_str());
	}

	int32 SoraLuaObject::getInteger(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetInteger();
	}

	float32 SoraLuaObject::getFloat(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetFloat();
	}

	double32 SoraLuaObject::getDouble(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetDouble();
	}

	SoraString SoraLuaObject::getString(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetString();
	}

	lua_CFunction SoraLuaObject::getCFunction(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetCFunction();
	}

	void* SoraLuaObject::getUserData(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetUserData();
	}

	bool SoraLuaObject::getBoolean(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetBoolean();
	}

	LuaState* SoraLuaObject::getState() {
		return luaState;
	}
} // namespace sora