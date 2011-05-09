/*
 *  GlobalValuesExporter.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "EnvVarExport.h"

namespace sora {

	void EnvValuesExporter::exportValues(LuaPlus::LuaState* state) {
		{
			LuaPlus::LuaObject global = state->GetGlobals();
			
			SoraEnvValues::VALUE_ITERATOR itVal = SoraEnvValues::Instance()->values.begin();
			while(itVal != SoraEnvValues::Instance()->values.end()) {
				switch(itVal->second.VALUE_TYPE) {
					case SoraEnvValues::VALUE_INT:
						global.SetInteger(id2str(itVal->first).c_str(), itVal->second.data.intVal);
						break;
					case SoraEnvValues::VALUE_BOOL:
						global.SetBoolean(id2str(itVal->first).c_str(), itVal->second.data.boolVal);
						break;
					case SoraEnvValues::VALUE_FLOAT:
						global.SetNumber(id2str(itVal->first).c_str(), itVal->second.data.floatVal);
						break;
					case SoraEnvValues::VALUE_STRING:
						std::string str = id2str(itVal->second.data.stringVal);
						global.SetString(id2str(itVal->first).c_str(), str.c_str(), str.size());
						break;
				}
				++itVal;
			}
		}
	}
	
	void exportEnvValuesManager(LuaPlus::LuaState* state) {
		gge::LuaClass<SoraEnvValues>(state, "ENV_VAR", SoraEnvValues::Instance())
			.def("getBool",		&SoraEnvValues::getBool)
			.def("getInt",		&SoraEnvValues::getInt)
			.def("getFloat",	&SoraEnvValues::getFloat)
			.def("getString",	&SoraEnvValues::getString)
			.def("setBool",		&SoraEnvValues::setBool)
			.def("setInt",		&SoraEnvValues::setInt)
			.def("setFloat",	&SoraEnvValues::setFloat)
			.def("setString",	&SoraEnvValues::setString);
	}
	
}