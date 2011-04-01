/*
 *  StringConv.h
 *  KuiIP
 *
 *  Created by griffin clare on 5/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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
	std::string int_to_str(int32 n);

	//FLOAT -> STRING
	std::string fp_to_str(float32 x);

	//宽字符串->字符串
	std::string ws2s(const std::wstring& ws);
	//字符串->宽字符串
	std::wstring s2ws(const std::string& s);
	
	
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
	
	// 純英文字符串限定
	std::string ws2sfast(const std::wstring& ws);
	std::wstring s2wsfast(const std::string& s);	

	void deliStr(std::vector<std::string>& c, const std::string& str, char deli);
	//}

} // namespace sora

#endif