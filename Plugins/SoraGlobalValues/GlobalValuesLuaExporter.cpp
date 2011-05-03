/*
 *  GlobalValuesExporter.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "GlobalValuesLuaExport.h"

namespace sora {

	void GlobalValuesExporter::exportValues(LuaPlus::LuaState* state) {
		{
			LuaPlus::LuaObject global = state->GetGlobals();
			
			SoraGlobalValues::VALUE_ITERATOR itVal = SoraGlobalValues::Instance()->values.begin();
			while(itVal != SoraGlobalValues::Instance()->values.end()) {
				switch(itVal->second.VALUE_TYPE) {
					case SoraGlobalValues::VALUE_INT:
						global.SetInteger(id2str(itVal->first).c_str(), itVal->second.data.intVal);
						break;
					case SoraGlobalValues::VALUE_BOOL:
						global.SetBoolean(id2str(itVal->first).c_str(), itVal->second.data.boolVal);
						break;
					case SoraGlobalValues::VALUE_FLOAT:
						global.SetNumber(id2str(itVal->first).c_str(), itVal->second.data.floatVal);
						break;
					case SoraGlobalValues::VALUE_STRING:
						std::string str = id2str(itVal->second.data.stringVal);
						global.SetString(id2str(itVal->first).c_str(), str.c_str(), str.size());
						break;
				}
				++itVal;
			}
		}
	}
	
	void exportGlobalValuesManager(LuaPlus::LuaState* state) {
		gge::LuaClass<SoraGlobalValues>(state, "GlobalValues", SoraGlobalValues::Instance())
		.def("getBoolValue", &SoraGlobalValues::getBoolValue_Str)
		.def("getIntValue",  &SoraGlobalValues::getIntValue_Str)
		.def("getFloatValue",  &SoraGlobalValues::getFloatValue_Str)
		.def("getStringValue",  &SoraGlobalValues::getStringValue_Str)
		.def("setBoolValue",  &SoraGlobalValues::setBoolValue_Str)
		.def("setIntValue",  &SoraGlobalValues::setIntValue_Str)
		.def("setFloatValue",  &SoraGlobalValues::setFloatValue)
		.def("setStringValue",  &SoraGlobalValues::setStringValue);
	}
	
}