/*
 *  StringConv.h
 *  KuiIP
 *
 *  Created by griffin clare on 5/6/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GCN_STRING_CONV_H
#define GCN_STRING_CONV_H

#include <iostream>
#include <string>

#include <vector>

namespace gcn {
	//宽字符串->字符串
	std::string ws2s(const std::wstring& ws);
	//字符串->宽字符串
	std::wstring s2ws(const std::string& s);
}

#endif // GCN_STRING_CONV_H_