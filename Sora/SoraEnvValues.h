/*
 *  SoraEnvValues.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ENV_VALUES_H_
#define SORA_ENV_VALUES_H_

#include "SoraPlatform.h"
#include "stringId.h"

#include "SoraSingleton.h"
#include <string>

namespace sora {
	
	class SoraEnvValues: public SoraSingleton<SoraEnvValues> {
		friend class SoraSingleton<SoraEnvValues>;
		friend class EnvValuesExporter;
		
	protected:
		SoraEnvValues() {}
		~SoraEnvValues() {}
		
	public:
		bool			getValue		(stringId name, bool defaultVal);
		int32			getValue		(stringId name, int32 defaultVal);
		float32			getValue		(stringId name, float32 defaultVal);
		std::string		getValue		(stringId name, const std::string& defaultVal);
		std::wstring	getValue		(stringId name, const std::wstring& defaultVal);
		void*			getValue		(stringId name);
		
		void setValue	(stringId name, bool val);
		void setValue	(stringId name, int32 val);
		void setValue	(stringId name, float32 val);
		void setValue	(stringId name, const std::string& val);
		void setValue	(stringId name, const std::wstring& val);
		void setValue	(stringId name, void* data);
		
		// for script export
		// because there's no polyphormism in scripts
		// and it's hard in scripts to use string id
		bool			getBool		(const std::string& name, bool defaultVal);
		int32			getInt		(const std::string& name, int32 defaultVal);
		float32			getFloat	(const std::string& name, float32 defaultVal);
		std::string		getString	(const std::string& name, const std::string& defaultVal);
		std::wstring	getWString	(const std::string& name, const std::wstring& defaultVal);
		void*			getData		(const std::string& name);
		
		void setBool		(const std::string& name, bool val);
		void setInt			(const std::string& name, int32 val);
		void setFloat		(const std::string& name, float32 val);
		void setString		(const std::string& name, const std::string& val);
		void setWString		(const std::string& name, const std::wstring& val);
		void setData		(const std::string& name, void* data);
		
		void removeData		(const std::string& name);
		void removeData		(stringId name);

	private:
		enum {
			VALUE_INT = 1,
			VALUE_BOOL = 2,
			VALUE_FLOAT = 3,
			VALUE_STRING = 4,
			VALUE_USERDATA = 5,
		};
		
		struct VALUE_STORE {
			union {
				bool boolVal;
				int32 intVal;
				float32 floatVal;
				stringId stringVal;
			} data;
			
			void* userData;
			uint8 VALUE_TYPE;
		};
		
		typedef hash_map<stringId, VALUE_STORE> GLOBAL_VALUE_MAP;
		typedef GLOBAL_VALUE_MAP::iterator VALUE_ITERATOR;
		GLOBAL_VALUE_MAP values;
	};
	
	#define SET_ENV(name, val)		SoraEnvValues::Instance()->setValue(name, val) 
	#define GET_ENV(name, default)	SoraEnvValues::Instance()->getValue(name, default)
	
	#define SET_ENV_INT(name, val)		SoraEnvValues::Instance()->setInt(name, val)
	#define SET_ENV_FLOAT(name, val)	SoraEnvValues::Instance()->setFloat(name, val)
	#define SET_ENV_BOOL(name, val)		SoraEnvValues::Instance()->setBool(name, val)
	#define SET_ENV_STRING(name, val)	SoraEnvValues::Instance()->setString(name, val)
	#define SET_ENV_WSTRING(name, val)	SoraEnvValues::Instance()->setWString(name, val)
	#define SET_ENV_DATA(name, data)	SoraEnvValues::Instance()->setData(name, data)
	
	#define GET_ENV_INT(name, val)		SoraEnvValues::Instance()->getInt(name, val)
	#define GET_ENV_FLOAT(name, val)	SoraEnvValues::Instance()->getFloat(name, val)
	#define GET_ENV_BOOL(name, val)		SoraEnvValues::Instance()->getBool(name, val)
	#define GET_ENV_STRING(name, val)	SoraEnvValues::Instance()->getString(name, val)
	#define GET_ENV_WSTRING(name, val)	SoraEnvValues::Instance()->getWString(name, val)
	#define GET_ENV_DATA(name)			SoraEnvValues::Instance()->getData(name)
	
	#define REMOVE_ENV_DATA(name)	SoraEnvValues::Instance()->removeData(name)
} // namespace sora


#endif // SORA_GLOBAL_VALUES_H_