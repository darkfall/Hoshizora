#ifndef SORA_LUA_OBJECT_H_
#define SORA_LUA_OBJECT_H_

#include "SoraObject.h"

#include "SoraStringConv.h"
#include "SoraTypeInfo.h"
#include "SoraAutoPtr.h"

#include "LuaPlus/LuaPlus.h"

#include <map>

#ifdef OS_WIN32     
    #ifdef LUA_PLUS_LIB
        #pragma comment(lib, "LuaPlus.lib")
    #endif
#endif

#include "SoraLuaFunctionHelper.h"

namespace sora {

#define OBJ_LUA 0x00000200

	class SoraLuaObject: public SoraObject {	 
	public:												 
		SoraLuaObject();
		SoraLuaObject(const SoraWString& scriptPath);
		SoraLuaObject(LuaPlus::LuaState* state);
		virtual ~SoraLuaObject();
        
        uint32 update(float32 dt);
        void render();

		int32 doScript(const SoraWString& scriptPath);
		int32 doString(const SoraString& str);
		int32 doBuffer(const char* buff, size_t size, const char* name);

		int32 loadString(const SoraString& str);
		int32 loadBuffer(const char* buff, size_t size, const char* name);
		int32 loadScript(const SoraWString& scriptPath);

		LuaPlus::LuaObject createThread();
		
		LuaPlus::LuaStackObject createTable(int narr=0, int nrec=0);
		LuaPlus::LuaStackObject createUserData(size_t size);

		LuaPlus::LuaObject getGlobals();
		LuaPlus::LuaObject get(const SoraString& name);
		LuaPlus::LuaObject operator[](const SoraString& name);

		// simple wrappers to help get data easily
		int32 getInteger(const SoraString& name);
		float32 getFloat(const SoraString& name);
		double getDouble(const SoraString& name);
		SoraString getString(const SoraString& name);
	//	SoraWString getWString(const SoraString& name);
		lua_CFunction getCFunction(const SoraString& name);
		void* getUserData(const SoraString& name);
		bool getBoolean(const SoraString& name);
		
        void callFuncVoid(const SoraString& funcName);
	
		LuaPlus::LuaState* getState() const;
		
	private:
		LuaPlus::LuaState* luaState;
		
		LuaPlus::LuaObject envObject;
	};

} // namespace sora

#endif