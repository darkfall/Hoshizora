/*
 *  SoraGlobalValues.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGlobalValues.h"

namespace sora {

	bool SoraGlobalValues::getBoolValue(stringId name, bool defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal;
				case VALUE_INT: return itVal->second.data.intVal>0?true:false;
				default: break;
			}
		}
		return defaultVal;
	}
	
	int32 SoraGlobalValues::getIntValue(stringId name, int32 defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal?1:0;
				case VALUE_INT: return itVal->second.data.intVal;
				case VALUE_FLOAT: return (int32)itVal->second.data.floatVal;
				default: break;
			}
		}
		return defaultVal;
	}
	
	float32	SoraGlobalValues::getFloatValue(stringId name, float32 defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal?1.f:0.f;
				case VALUE_INT: return (float32)itVal->second.data.intVal;
				case VALUE_FLOAT: return itVal->second.data.floatVal;
				default: break;
			}
		}
		return defaultVal;
	}
	
	std::string	SoraGlobalValues::getStringValue(stringId name, const std::string& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2str(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	std::wstring SoraGlobalValues::getWStringValue(stringId name, const std::wstring& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2strw(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	
	void SoraGlobalValues::setBoolValue(stringId name, bool val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_BOOL;
		myData.data.boolVal = val;
		values[name] = myData;
	}
	
	void SoraGlobalValues::setIntValue(stringId name, int32 val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_INT;
		myData.data.intVal = val;
		values[name] = myData;
	}
	
	void SoraGlobalValues::setFloatValue(stringId name, float32 val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_FLOAT;
		myData.data.floatVal = val;
		values[name] = myData;
	}
	
	void SoraGlobalValues::setStringValue(stringId name, const std::string& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = str2id(val);
		values[name] = myData;
	}
	
	void SoraGlobalValues::setWStringValue(stringId name, const std::wstring& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = str2id(val);
		values[name] = myData;
	}
	
	
	// slower but more convinient
	bool SoraGlobalValues::getBoolValue_Str(const std::string& name, bool defaultVal) {
		stringId sname = str2id(name);
		return getBoolValue(sname, defaultVal);
	}
	
	int32 SoraGlobalValues::getIntValue_Str(const std::string& name, int32 defaultVal) {
		stringId sname = str2id(name);
		return getIntValue(sname, defaultVal);
	}
	
	float32	SoraGlobalValues::getFloatValue_Str(const std::string& name, float32 defaultVal) {
		stringId sname = str2id(name);
		return getFloatValue(sname, defaultVal);
	}
	
	std::string	SoraGlobalValues::getStringValue_Str(const std::string& name, const std::string& defaultVal) {
		stringId sname = str2id(name);
		return getStringValue(sname, defaultVal);
	}
	
	std::wstring SoraGlobalValues::getWStringValue_Str(const std::string& name, const std::wstring& defaultVal) {
		stringId sname = str2id(name);
		return getWStringValue(sname, defaultVal);
	}
	
	
	void SoraGlobalValues::setBoolValue_Str(const std::string& name, bool val) {
		setBoolValue(str2id(name), val);
	}
	
	void SoraGlobalValues::setIntValue_Str(const std::string& name, int32 val) {
		setIntValue(str2id(name), val);
	}
	
	void SoraGlobalValues::setFloatValue_Str(const std::string& name, float32 val) {
		setFloatValue(str2id(name), val);
	}
	
	void SoraGlobalValues::setStringValue_Str(const std::string& name, const std::string& val) {
		setStringValue(str2id(name), val);
	}
	
	void SoraGlobalValues::setWStringValue_Str(const std::string& name, const std::wstring& val) {
		setWStringValue(str2id(name), val);
	}
	
	

} // namespace sora