/*
 *  lex_error.h
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _LEX_ERROR_H_
#define _LEX_ERROR_H_

#include <cstdio>
#include <cstring>

#include <string>

using namespace std;

void	Lex_ExitOnInvalidInputError(char cInput, int iLineIndex, const string& sScriptLine);

#endif