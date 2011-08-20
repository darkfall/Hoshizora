/*
 *  StringConv.h
 *  KuiIP
 *
 *  Created by griffin clare on 5/6/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_STRING_CONV_H
#define SORA_STRING_CONV_H

#include <iostream>
#include <string>
#include <vector>

#include <sstream>
#include <iomanip>

#include "SoraPlatform.h"

namespace sora {
	//extern "C" {
	//INT -> STRING
	SORA_API std::string int_to_str(int32 n);

	//FLOAT -> STRING
	SORA_API std::string fp_to_str(float32 x);

	SORA_API std::string ws2s(const std::wstring& ws);
	SORA_API std::wstring s2ws(const std::string& s);
    
	template<typename T>
	std::string anyToString(const T& t, int precision) {
		std::ostringstream str;
		str << std::fixed << std::setprecision(precision) << t;
		return str.str();		
	}
	template<typename T>
	std::wstring anyToWString(const T& t, int precision) {
		std::wostringstream str;
		str << std::fixed << std::setprecision(precision) << t;
		return str.str();
	}
	
	SORA_API std::string ws2sfast(const std::wstring& ws);
	SORA_API std::wstring s2wsfast(const std::string& s);	

	SORA_API void deliStr(std::vector<std::string>& c, const std::string& str, char deli);
    
    class SORA_API SoraStringConverter {
    public:
        static std::string wstringToString(const std::wstring& ws);
        static std::wstring stringToWString(const std::string& s);
    };
    
    SORA_API std::string tolowerstring(const std::string& s);
    SORA_API std::string toupperstring(const std::string& s);


} // namespace sora

#endif