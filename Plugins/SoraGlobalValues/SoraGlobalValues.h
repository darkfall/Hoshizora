/*
 *  SoraGlobalValues.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GLOBAL_VALUES_H_
#define SORA_GLOBAL_VALUES_H_


#include "SoraPlatform.h"
#include "stringId.h"

#include "SoraSingleton.h"
#include <string>

namespace sora {
	
	class SoraGlobalValues: public SoraSingleton<SoraGlobalValues> {
		friend class SoraSingleton<SoraGlobalValues>;
		friend class GlobalValuesExporter;
		
	protected:
		SoraGlobalValues() {}
		~SoraGlobalValues() {}
		
	public:
		bool			getBoolValue	(stringId name, bool defaultVal);
		int32			getIntValue		(stringId name, int32 defaultVal);
		float32			getFloatValue	(stringId name, float32 defaultVal);
		std::string		getStringValue	(stringId name, const std::string& defaultVal);
		std::wstring	getWStringValue	(stringId name, const std::wstring& defaultVal);
		
		void setBoolValue	(stringId name, bool val);
		void setIntValue	(stringId name, int32 val);
		void setFloatValue	(stringId name, float32 val);
		void setStringValue	(stringId name, const std::string& val);
		void setWStringValue(stringId name, const std::wstring& val);
		
		// slower but more convinient
		bool			getBoolValue_Str	(const std::string& name, bool defaultVal);
		int32			getIntValue_Str		(const std::string& name, int32 defaultVal);
		float32			getFloatValue_Str	(const std::string& name, float32 defaultVal);
		std::string		getStringValue_Str	(const std::string& name, const std::string& defaultVal);
		std::wstring	getWStringValue_Str	(const std::string& name, const std::wstring& defaultVal);
		
		void setBoolValue_Str	(const std::string& name, bool val);
		void setIntValue_Str	(const std::string& name, int32 val);
		void setFloatValue_Str	(const std::string& name, float32 val);
		void setStringValue_Str	(const std::string& name, const std::string& val);
		void setWStringValue_Str(const std::string& name, const std::wstring& val);

	private:
		enum {
			VALUE_INT = 1,
			VALUE_BOOL = 2,
			VALUE_FLOAT = 3,
			VALUE_STRING = 4,
		};
		
		struct VALUE_STORE {
			union {
				bool boolVal;
				int32 intVal;
				float32 floatVal;
				stringId stringVal;
			} data;
			
			uint8 VALUE_TYPE;
		};
		
		typedef hash_map<stringId, VALUE_STORE> GLOBAL_VALUE_MAP;
		typedef GLOBAL_VALUE_MAP::iterator VALUE_ITERATOR;
		GLOBAL_VALUE_MAP values;
	};
	
} // namespace sora


#endif // SORA_GLOBAL_VALUES_H_