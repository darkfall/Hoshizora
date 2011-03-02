/*
 *  compiler_error.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月9日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _COMPILER_ERROR_H_
#define _COMPILER_ERROR_H_

#include "compiler_global.h"

void	Compiler_ExitOnError(const char* pstrErrorMssg);
void	Compiler_ExitOnCharExpectedError(char cChar);

#endif