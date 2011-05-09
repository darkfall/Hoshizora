/*
 *  GlobalValuesLuaExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GLOBAL_VALUES_LUA_EXPORT_H_
#define GLOBAL_VALUES_LUA_EXPORT_H_

#include "SoraEnvValues.h"
#include "luaclass.h"

namespace sora {
	
	// this exports all value symbols in the manager to lua
	class EnvValuesExporter {
	public:
		static void exportValues(LuaPlus::LuaState* state);
	};
	
	void exportEnvValuesManager(LuaPlus::LuaState* state);
	
	
} // namespace sora

#endif // GLOBAL_VALUES_LUA_EXPORT_H_
