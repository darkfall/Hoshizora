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

#ifdef WIN32
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)

#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <iostream>
#include <string>

#include <vector>

using std::string;
using std::wstring;

//宽字符串->字符串
string ws2s(const wstring& ws);
//字符串->宽字符串
wstring s2ws(const string& s);

#endif