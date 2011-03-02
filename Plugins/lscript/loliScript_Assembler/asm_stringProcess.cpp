/*
 *  stringProcess.cpp
 *  loliScript_Assembler
 *
 *  Created by griffin clare on 09年12月22日.
 *  Copyright 2009 ring0dev. All rights reserved.
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

#include "asm_stringProcess.h"

//determines if a character is numeric
int isCharNumeric(char cChar) {
	if(cChar >= '0' && cChar <= '9')
		return TRUE;
	return FALSE;
}

//determines if a character is whitespace
int isCharWhitespace(char cChar) {
	if(cChar == ' ' || cChar == '\t' || cChar == '\r' || cChar == '\n')
		return TRUE;
	return FALSE;
}

//determines if a charatcter could be part of a valid identifier
int isCharIdent(char cChar) {
	if((cChar >= '0' && cChar <= '9') ||
	   (cChar >= 'A' && cChar <= 'Z') ||
	   (cChar >= 'a' && cChar <= 'z') ||
	   (cChar == '_') )
		return TRUE;
	return FALSE;
}

//determines if a character is a delimiter
int isCharDelimiter(char cChar) {
	if(cChar == ':' || cChar == ',' || cChar == '"' ||
	   cChar == '[' || cChar == ']' ||
	   cChar == '{' || cChar == '}' ||
	   isCharWhitespace(cChar) || (int)cChar == 10 )
		return TRUE;
	return FALSE;
}

int isStringInt(const char* pstrString) {
	if(!pstrString)
		return FALSE;
	
	size_t strLength = strlen(pstrString);
	if(strLength == 0)
		return FALSE;
	
	size_t iCurrCharIndex;
	for(iCurrCharIndex = 0; iCurrCharIndex < strLength; ++iCurrCharIndex) {
		if(!isCharNumeric(pstrString[iCurrCharIndex]) &&
		   !(pstrString[iCurrCharIndex] == '-') &&
		   ! (isCharWhitespace(pstrString[iCurrCharIndex])) )
			return FALSE;
		
		
		if(iCurrCharIndex != 0)
			if(pstrString[iCurrCharIndex] == '-')
				return FALSE;
	}
	
	return TRUE;
}

int isStringFloat(const char* pstrString) {
	if(!pstrString)
		return FALSE;
	
	size_t strLength = strlen(pstrString);
	if(strLength == 0)
		return FALSE;
	
	size_t	iCurrCharIndex;
	int		iRadixPointFound = FALSE;
	
	for(iCurrCharIndex = 0; iCurrCharIndex < strLength; ++iCurrCharIndex) {
		if(!isCharNumeric(pstrString[iCurrCharIndex]) &&
		   !(pstrString[iCurrCharIndex] == '-') &&
		   !(pstrString[iCurrCharIndex] == '.') &&
		   ! (isCharWhitespace(pstrString[iCurrCharIndex])) )
			return FALSE;
		
		//find radix point, if there are more than one, then illegal
		if(pstrString[iCurrCharIndex] == '.')
			if(iRadixPointFound)
				return FALSE;
			else 
				iRadixPointFound = TRUE;
		
		if(iCurrCharIndex != 0)
			if(pstrString[iCurrCharIndex] == '-')
				return FALSE;
	}
	
	return iRadixPointFound;
}

int isStringWhitespacec(const char* pstrString) {
	if(!pstrString)
		return FALSE;
	
	size_t strLength = strlen(pstrString);
	if(strLength == 0)
		return FALSE;
	
	size_t iCurrCharIndex;
	for(iCurrCharIndex = 0; iCurrCharIndex < strLength; ++iCurrCharIndex) {
		if(!isCharWhitespace(pstrString[iCurrCharIndex]) && (int)pstrString[iCurrCharIndex] != 10 )
			return FALSE;
	}
	
	return TRUE;
}

int isStringIdent(const char* pstrString) {
	if(!pstrString)
		return FALSE;
	
	size_t strLength = strlen(pstrString);
	if(strLength == 0)
		return FALSE;
	
	if(pstrString[0] >= '0' && pstrString[0] <= '9')
		return FALSE;
	
	size_t iCurrCharIndex;
	for(iCurrCharIndex = 0; iCurrCharIndex < strLength; ++iCurrCharIndex) {
		if(!isCharIdent(pstrString[iCurrCharIndex]))
			return FALSE;
	}
	
	return TRUE;
}


void StripComments(char* pstrSourceLine) {
	size_t	iCurrCharIndex;
	int		iInString = 0;
	
	int		strLength = strlen(pstrSourceLine);
	if(strLength <= 0) {
		return;
	}
	
	for(iCurrCharIndex = 0;
		iCurrCharIndex < strLength-1;
		++iCurrCharIndex) {
		if(pstrSourceLine[iCurrCharIndex] == '"') {
			if(iInString)
				iInString = 0;
			else 
				iInString = 1;
		}
		
		if(pstrSourceLine[iCurrCharIndex] == ';') {
			if(!iInString) {
				pstrSourceLine[iCurrCharIndex] = '\n';
				pstrSourceLine[iCurrCharIndex+1] = '\0';
				break;
			}
		}
	}
}

void TrimWhitespace(char* pstrString) {
	size_t	iStringLength = strlen(pstrString);
	size_t	iPadLength;
	size_t	iCurrCharIndex;
	
	if(iStringLength > 1) {
		for(iCurrCharIndex = 0;
			iCurrCharIndex < iStringLength;
			++iCurrCharIndex) {
			if(!isCharWhitespace(pstrString[iCurrCharIndex]) )
				break;
		}
		
		iPadLength = iCurrCharIndex;
		if(iPadLength) {
			for(iCurrCharIndex = iPadLength;
				iCurrCharIndex < iStringLength;
				++iCurrCharIndex) {
				pstrString[iCurrCharIndex-iPadLength] = pstrString[iCurrCharIndex];
			}
			
			for(iCurrCharIndex = iStringLength-iPadLength;
				iCurrCharIndex < iStringLength;
				++iCurrCharIndex) {
				pstrString[iCurrCharIndex] = ' ';
			}
		}
		
		for(iCurrCharIndex = iStringLength-1;
			iCurrCharIndex > 0;
			--iCurrCharIndex) {
			if(!isCharWhitespace(pstrString[iCurrCharIndex])) {
				pstrString[iCurrCharIndex+1] = '\0';
				break;
			}
		}
	}
}

char* strupr(char* a) {
	char *ret = a;
	while(*a != '\0') {
		if((*a) >= 97 && (*a) <= 122)
			*a = (*a)-32;
		++a;
    }
	return ret;
}

void strupr(std::string& str) {
	size_t size = 0;
	while(size < str.size()) {
		if(islower(str[size]))
		   toupper(str[size]);
	}
}


