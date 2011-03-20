#ifndef SORA_LUA_OBJECT_H_
#define SORA_LUA_OBJECT_H_

#include "SoraObject.h"

#include "SoraStringConv.h"
#include "SoraTypeInfo.h"
#include "SoraAutoPtr.h"

#include "LuaPlus/LuaPlus.h"

#include <map>

namespace sora {

	using namespace LuaPlus;
	using std::map;

	static map<ulong32, int32> LuaStateRefMap;

	class SoraLuaObject: public SoraObject {	 
	public:												 
		SoraLuaObject();
		SoraLuaObject(const SoraWString& scriptPath);
		SoraLuaObject(LuaState* state);
		virtual ~SoraLuaObject();

		int32 doScript(const SoraWString& scriptPath);
		int32 doString(const SoraString& str);
		int32 doBuffer(const char* buff, size_t size, const char* name);

		int32 loadString(const SoraString& str);
		int32 loadBuffer(const char* buff, size_t size, const char* name);
		int32 loadScript(const SoraWString& scriptPath);

		LuaObject createThread();
		
		LuaStackObject createTable(int narr=0, int nrec=0);
		LuaStackObject createUserData(size_t size);

		LuaObject getGlobals();
		LuaObject get(const SoraString& name);
		LuaObject operator[](const SoraString& name);

		// simple wrappers to help get data easily
		int32 getInteger(const SoraString& name);
		float32 getFloat(const SoraString& name);
		double32 getDouble(const SoraString& name);
		SoraString getString(const SoraString& name);
	//	SoraWString getWString(const SoraString& name);
		lua_CFunction getCFunction(const SoraString& name);
		void* getUserData(const SoraString& name);
		bool getBoolean(const SoraString& name);

		LuaState* getState();
	private:
		LuaState* luaState;
		
		LuaObject envObject;
	};

} // namespace sora

#endif