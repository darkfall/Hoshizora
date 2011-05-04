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
        SoraLuaExport::export_sprites(luaState);
		SoraLuaExport::export_font(luaState);

		luaopen_pluto(luaState->GetCState());

		//LuaStateRefMap[(ulong32)luaState] = 1;
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
        setType(OBJ_LUA);
	}

	SoraLuaObject::SoraLuaObject(const SoraWString& scriptPath) {
		luaState = LuaState::Create(true);
		if(!luaState)
			throw SORA_EXCEPTION("error creating LuaState");
		
		SoraLuaExport::export_constants(luaState);
		SoraLuaExport::export_soracore(luaState);
        SoraLuaExport::export_sprites(luaState);
		SoraLuaExport::export_font(luaState);
		
		luaopen_pluto(luaState->GetCState());

		//LuaStateRefMap[(ulong32)luaState] = 1;
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
        setType(OBJ_LUA);

        doScript(scriptPath);
	}

	SoraLuaObject::SoraLuaObject(LuaState* state) {
		luaState = state;
		//LuaStateRefMap[(ulong32)luaState] = 1;
		if(!luaState)
			throw SORA_EXCEPTION("error creating LuaState");
		setType(OBJ_LUA);
		SoraLuaObjectManager::Instance()->registerLuaObject(this);
	}

	SoraLuaObject::~SoraLuaObject() {
	//	LuaStateRefMap[(ulong32)luaState]--;
	//	if(LuaStateRefMap[(ulong32)luaState] == 0)
			LuaState::Destroy(luaState);
		SoraLuaObjectManager::Instance()->unregisterLuaObject(this);
	}

	int32 SoraLuaObject::doScript(const SoraWString& scriptPath) {
		ulong32 size;
		void* pdata = SORA->getResourceFile(scriptPath, size);
		if(pdata) {
			return doString((const char*)pdata);
			SORA->freeResourceFile(pdata);
		}
		return 0;
	}

	int32 SoraLuaObject::doString(const SoraString& str) {
		return luaState->DoString(str.c_str());
	}

	int32 SoraLuaObject::doBuffer(const char* buff, size_t size, const char* name) {
		return luaState->DoBuffer(buff, size, name);
	}

	int32 SoraLuaObject::loadScript(const SoraWString& scriptPath) {
		ulong32 size;
		void* pdata = SORA->getResourceFile(scriptPath, size);
		if(pdata) {
			return loadString((const char*)pdata);
			SORA->freeResourceFile(pdata);
		}
		return 0;
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

	LuaState* SoraLuaObject::getState() const {
		return luaState;
	}
	
    void SoraLuaObject::callFuncVoid(const SoraString& funcName) {
        LuaObject obj = get(funcName);
        if(obj.IsFunction()) {
            LuaFunctionVoid func = obj;
            func();
        }
    }
	
    uint32 SoraLuaObject::update(float32 dt) {
        LuaObject obj = get("update");
        if(obj.IsFunction()) {
            LuaFunction<void> func = obj;
            func(dt);
        }
        return 0;
    }
   
    void SoraLuaObject::render() {
        callFuncVoid("render");
    }
} // namespace sora