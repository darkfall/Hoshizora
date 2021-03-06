﻿/*
 *  compiler_error.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月9日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "compiler_error.h"

#include "lsLog.h"
void Compiler_ExitOnError(const char* pstrErrorMssg) {
	cout<<"Compiler Fatal Error: "<<pstrErrorMssg<<endl;
}

void Compiler_ExitOnCharExpectedError(char cChar) {
	char* pstrErrorMssg = (char*)malloc(strlen("Compiler: ' ' expected"));
	sprintf(pstrErrorMssg, "'%c' expected\n", cChar);
	Compiler_ExitOnError(pstrErrorMssg);
}


