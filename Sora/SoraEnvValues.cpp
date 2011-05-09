/*
 *  SoraEnvValues.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraEnvValues.h"

namespace sora {

	bool SoraEnvValues::getValue(stringId name, bool defaultVal) {
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
	
	int32 SoraEnvValues::getValue(stringId name, int32 defaultVal) {
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
	
	float32	SoraEnvValues::getValue(stringId name, float32 defaultVal) {
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
	
	std::string	SoraEnvValues::getValue(stringId name, const std::string& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2str(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	std::wstring SoraEnvValues::getValue(stringId name, const std::wstring& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2strw(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	
	void SoraEnvValues::setValue(stringId name, bool val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_BOOL;
		myData.data.boolVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(stringId name, int32 val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_INT;
		myData.data.intVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(stringId name, float32 val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_FLOAT;
		myData.data.floatVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(stringId name, const std::string& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = str2id(val);
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(stringId name, const std::wstring& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = str2id(val);
		values[name] = myData;
	}
	
	bool SoraEnvValues::getBool(const std::string& name, bool defaultVal) {
		stringId sname = str2id(name);
		return getValue(sname, defaultVal);
	}
	
	int32 SoraEnvValues::getInt(const std::string& name, int32 defaultVal) {
		stringId sname = str2id(name);
		return getValue(sname, defaultVal);
	}
	
	float32	SoraEnvValues::getFloat(const std::string& name, float32 defaultVal) {
		stringId sname = str2id(name);
		return getValue(sname, defaultVal);
	}
	
	std::string	SoraEnvValues::getString(const std::string& name, const std::string& defaultVal) {
		stringId sname = str2id(name);
		return getValue(sname, defaultVal);
	}
	
	std::wstring SoraEnvValues::getWString(const std::string& name, const std::wstring& defaultVal) {
		stringId sname = str2id(name);
		return getValue(sname, defaultVal);
	}
	
	
	void SoraEnvValues::setBool(const std::string& name, bool val) {
		setValue(str2id(name), val);
	}
	
	void SoraEnvValues::setInt(const std::string& name, int32 val) {
		setValue(str2id(name), val);
	}
	
	void SoraEnvValues::setFloat(const std::string& name, float32 val) {
		setValue(str2id(name), val);
	}
	
	void SoraEnvValues::setString(const std::string& name, const std::string& val) {
		setValue(str2id(name), val);
	}
	
	void SoraEnvValues::setWString(const std::string& name, const std::wstring& val) {
		setValue(str2id(name), val);
	}
	
	

} // namespace sora