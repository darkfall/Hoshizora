/*
 *  lex_stringProcess.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 09年12月22日.
 *  Copyright 2009 ClareGriffin(ring0dev). All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  
 *  Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  
 *  Neither the name of this project's author nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */
#include "lex_stringProcess.h"
#include <string>

//determines if a character is numeric
int isCharNumericLex(char cChar) {
	if(cChar >= '0' && cChar <= '9')
		return TRUE;
	return FALSE;
}

//determines if a character is whitespace
int isCharWhitespaceLex(char cChar) {
	if(cChar == ' ' || cChar == '\t' || cChar == '\n')
		return TRUE;
	return FALSE;
}

//determines if a charatcter could be part of a valid identifier
int isCharIdentLex(char cChar) {
	if((cChar >= '0' && cChar <= '9') ||
	   (cChar >= 'A' && cChar <= 'Z') ||
	   (cChar >= 'a' && cChar <= 'z') ||
	   (cChar == '_') )
		return TRUE;
	return FALSE;
}

//determines if a character is a delimiter
int isCharDelimiterLex(char cChar) {
	if(cChar == ',' || cChar == '(' || cChar == ')' ||
	   cChar == '[' || cChar == ']' || cChar == ';' ||
	   cChar == '{' || cChar == '}' || cChar == '#' ||
	   cChar == '.')
		return TRUE;
	return FALSE;
}