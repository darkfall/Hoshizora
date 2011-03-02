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
	std::string anyToString(const T& t, int precision=3);
	template<typename T>
	std::wstring anyToWString(const T& t, int precision=3);
	
	
	// 純英文字符串限定
	std::string ws2s2(const std::wstring& ws);
	std::wstring s2ws2(const std::string& s);	

	void deliStr(std::vector<std::string>& c, const std::string& str, char deli);
	//}

} // namespace sora

#endif