#ifndef XLOG_H
#define XLOG_H

#include <iostream>

using std::endl;
using std::cout;

#include <string>

using std::string;

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

void logMssg(const char* format, ...);

void logMssg(const string& sStr);

void exitWithMssgBox(int iExitCode, const char* format, ...);

#endif