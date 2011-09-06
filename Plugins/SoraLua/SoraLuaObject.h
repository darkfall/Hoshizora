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

	class SoraLuaObject: public SoraObject {	 
	public:												 
		SoraLuaObject(bool heavy=true);
		SoraLuaObject(const util::String& scriptPath, bool heavy=true);
		SoraLuaObject(LuaPlus::LuaState* state, bool heavy=true);
		virtual ~SoraLuaObject();
        
        uint32 update(float dt);
        void render();

		int32 doScript(const util::String& scriptPath);
		int32 doString(const util::String& str);
		int32 doBuffer(const char* buff, size_t size, const char* name);

		int32 loadString(const util::String& str);
		int32 loadBuffer(const char* buff, size_t size, const char* name);
		int32 loadScript(const util::String& scriptPath);

		LuaPlus::LuaObject createThread();
		
		LuaPlus::LuaStackObject createTable(int narr=0, int nrec=0);
		LuaPlus::LuaStackObject createUserData(size_t size);

		LuaPlus::LuaObject getGlobals();
		LuaPlus::LuaObject get(const util::String& name);
		LuaPlus::LuaObject operator[](const util::String& name);

		// simple wrappers to help get data easily
		int32 getInteger(const util::String& name);
		float getFloat(const util::String& name);
		double getDouble(const util::String& name);
		util::String getString(const util::String& name);
	//	util::String getWString(const util::String& name);
		lua_CFunction getCFunction(const util::String& name);
		void* getUserData(const util::String& name);
		bool getBoolean(const util::String& name);
		
        void callFuncVoid(const util::String& funcName);
	
		LuaPlus::LuaState* getState() const;
		
	private:
		LuaPlus::LuaState* luaState;
	};

} // namespace sora

#endif