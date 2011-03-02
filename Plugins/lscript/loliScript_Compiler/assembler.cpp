/*
 *  loadSourceFile.c
 *  loliScript_Assembler
 *
 *  Created by griffin clare on 09年12月21日.
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
// #include "stdafx.h"

#include "assembler.h"

#include <iostream>

#include <cstring>

using namespace std;

#define OP_FLAG_TYPE_INT			1		//integer
#define OP_FLAG_TYPE_FLOAT			2		//float
#define OP_FLAG_TYPE_STRING			4		//string
#define OP_FLAG_TYPE_MEM_REF		8		//memory reference
#define OP_FLAG_TYPE_LINE_LABEL		16		//label identifier	 ( jmp labelName )
#define OP_FLAG_TYPE_FUNC_NAME		32		//function call ( call funcName ) 
#define OP_FLAG_TYPE_HOST_API_CALL	64		//Host API call	( callHost hostAPIName )
#define OP_FLAG_TYPE_REG			128		//_RegVal register;

#define OP_TYPE_INT							0
#define OP_TYPE_FLOAT						1
#define OP_TYPE_STRING_INDEX				2
#define	OP_TYPE_ABS_STACK_INDEX				3
#define OP_TYPE_REL_STACK_INDEX				4
#define OP_TYPE_INSTR_INDEX					5
#define OP_TYPE_FUNC_INDEX					6
#define OP_TYPE_HOST_API_CALL_INDEX			7
#define OP_TYPE_REG							8

#define INSTR_MOV               0

#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

#define INSTR_PUSH              26
#define INSTR_POP               27

#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

#define INSTR_PAUSE             31
#define INSTR_EXIT              32

#define INSTR_PRINT				33

#define INSTR_SIN				34
#define INSTR_COS				35
#define INSTR_TAN				36
#define INSTR_LN				37
#define INSTR_LG				38
#define INSTR_EXPL				39
#define INSTR_REC				40
#define INSTR_SH				41
#define INSTR_CH				42
#define INSTR_TH				43
#define INSTR_SQRT				44

#define INSTR_RANDOMINT			48
#define INSTR_RANDOMFLOAT		49

#define INSTR_PUSHPARAM			45
#define INSTR_PUSHHOSTPARAM		46

#define INSTR_PARAM				47

#define TOKEN_TYPE_INT						0
#define TOKEN_TYPE_FLOAT					1
#define TOKEN_TYPE_STRING					2
#define TOKEN_TYPE_QUOTE					3
#define TOKEN_TYPE_IDENT					4
#define TOKEN_TYPE_COLON					5
#define TOKEN_TYPE_OPEN_BRACKET				6
#define TOKEN_TYPE_CLOSE_BRACKET			7
#define TOKEN_TYPE_COMMA					8
#define TOKEN_TYPE_OPEN_BRACE				9
#define TOKEN_TYPE_CLOSE_BRACE				10
#define TOKEN_TYPE_NEWLINE					11
#define TOKEN_TYPE_INSTR					12
#define TOKEN_TYPE_SETSTACKSIZE				13
#define TOKEN_TYPE_VAR						14
#define TOKEN_TYPE_FUNC						15
#define TOKEN_TYPE_PARAM					16
#define TOKEN_TYPE_REG_RETVAL				17
#define TOKEN_TYPE_INVALID					18
#define TOKEN_TYPE_SETPRIORITY				19

#define END_OF_TOKEN_STREAM					20

#define LEX_STATE_END_STRING				0
#define LEX_STATE_NO_STRING					1
#define LEX_STATE_IN_STRING					2

#include "asm_stringProcess.h"

#include "asm_error.h"

#include "lsLog.h"
LSL_Assembler::LSL_Assembler() {
	initInstrTable();
	
	iIsSetStackSizeFound = FALSE;
	iIsPriorityFound = FALSE;
	
	iSourceCodeSize = iInstrStreamSize = iCurrInstrIndex = 0;
	
	pInstrStream = NULL;
	ppstrSourceCode = NULL;
	pSourceFile = NULL;
}

LSL_Assembler::~LSL_Assembler() {
	if(pInstrStream)
		free(pInstrStream);
	for(int i=0; i<iSourceCodeSize; ++i)
		free(ppstrSourceCode[i]);
	free(ppstrSourceCode);
}

Token LSL_Assembler::GetNextToken() {
	lexer.iIndex0 = lexer.iIndex1;
	
	if(lexer.iIndex0 >= strlen(ppstrSourceCode[lexer.iCurrSourceLine]))
		if(!SkipToNextLine())
			return END_OF_TOKEN_STREAM;
	
	//string lexemes
	
	if(lexer.iCurrLexState == LEX_STATE_END_STRING)
		lexer.iCurrLexState = LEX_STATE_NO_STRING;
	
	if(lexer.iCurrLexState != LEX_STATE_IN_STRING) {
		while (TRUE) {
			if(!isCharWhitespace(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex0]))
				break;
			++lexer.iIndex0;
		}
	}
	
	lexer.iIndex1 = lexer.iIndex0;
	
	while(TRUE) {
		if(lexer.iCurrLexState == LEX_STATE_IN_STRING) {
			
			//when reach the end
			//if there is no ending double-quote of a string lexeme
			//return an invalid token
			
			if(lexer.iIndex1 >= strlen(ppstrSourceCode[lexer.iCurrSourceLine])) {
				lexer.CurrToken = TOKEN_TYPE_INVALID;
				return lexer.CurrToken;
			}
			
			
			//skip escape sequence if current character is a backslash
			
			if(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1] == '\\') {
				if(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1+1] == 'n') {
					ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1+1] = '\n';
				}
				lexer.iIndex1 += 2;
				continue;
			}
		
			//end of a string
			
			if(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1] == '"') {
				break;
			}
		
			++lexer.iIndex1;
		} else {
			if(lexer.iIndex1 >= strlen(ppstrSourceCode[lexer.iCurrSourceLine]))
				break;
			
			if(isCharDelimiter(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1]) ||
			   isCharWhitespace(ppstrSourceCode[lexer.iCurrSourceLine][lexer.iIndex1])) {
				break;
			}
			
			++lexer.iIndex1;
		}
	}
	
	if(lexer.iIndex1 - lexer.iIndex0 == 0)
		++lexer.iIndex1;
	
	size_t iCurrDestIndex = 0;
	for(size_t iCurrSourceIndex = lexer.iIndex0; iCurrSourceIndex < lexer.iIndex1; ++iCurrSourceIndex) {
		
		//if is in a string, check the escape sequence and copy the character following the backslash
		
		if(lexer.iCurrLexState == LEX_STATE_IN_STRING) 
			if(ppstrSourceCode[lexer.iCurrSourceLine][iCurrSourceIndex] == '\\')
				++iCurrSourceIndex;
		
		lexer.pstrCurrLexeme[iCurrDestIndex] = ppstrSourceCode[lexer.iCurrSourceLine][iCurrSourceIndex];
		
		++iCurrDestIndex;
	}
	
	lexer.pstrCurrLexeme[iCurrDestIndex] = '\0';
	
	//if it's not a string, then convert to uppercase
	
	if(lexer.iCurrLexState != LEX_STATE_IN_STRING)
		strupr(lexer.pstrCurrLexeme);
	
	lexer.CurrToken = TOKEN_TYPE_INVALID;
	
	if(strlen(lexer.pstrCurrLexeme) > 1 ||
	   lexer.pstrCurrLexeme[0] != '"') {
		if(lexer.iCurrLexState == LEX_STATE_IN_STRING) {
			lexer.CurrToken = TOKEN_TYPE_STRING;
			return TOKEN_TYPE_STRING;
		}
	}
	
	//single character tokens
	if(strlen(lexer.pstrCurrLexeme) == 1) {
		switch (lexer.pstrCurrLexeme[0]) {
			
				//if is a double-quote, then update the string state
				
			case '"':
				switch (lexer.iCurrLexState) {
					case LEX_STATE_NO_STRING:
						lexer.iCurrLexState = LEX_STATE_IN_STRING;
						break;
						
					case LEX_STATE_IN_STRING:
						lexer.iCurrLexState = LEX_STATE_END_STRING;
						break;
				}
				
				lexer.CurrToken = TOKEN_TYPE_QUOTE;
				break;
				
			case ',':
				lexer.CurrToken = TOKEN_TYPE_COMMA;
				break;
				
			case ':':
				lexer.CurrToken = TOKEN_TYPE_COLON;
				break;
				
			case '[':
				lexer.CurrToken = TOKEN_TYPE_OPEN_BRACKET;
				break;
				
			case ']':
				lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACKET;
				break;
				
			case '{':
				lexer.CurrToken = TOKEN_TYPE_OPEN_BRACE;
				break;
				
			case '}':
				lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACE;
				break;
				
			case '\n':
				lexer.CurrToken = TOKEN_TYPE_NEWLINE;
				break;
		}
	}
	
	if(isStringInt(lexer.pstrCurrLexeme))
		lexer.CurrToken = TOKEN_TYPE_INT;
	
	if(isStringFloat(lexer.pstrCurrLexeme))
		lexer.CurrToken = TOKEN_TYPE_FLOAT;
	
	if(isStringIdent(lexer.pstrCurrLexeme))
		lexer.CurrToken = TOKEN_TYPE_IDENT;
	
	if(strcmp(lexer.pstrCurrLexeme, "VAR") == 0) 
		lexer.CurrToken = TOKEN_TYPE_VAR;
	
	if(strcmp(lexer.pstrCurrLexeme, "SETSTACKSIZE") == 0)
		lexer.CurrToken = TOKEN_TYPE_SETSTACKSIZE;
	
	if(strcmp(lexer.pstrCurrLexeme, "FUNC") == 0)
		lexer.CurrToken = TOKEN_TYPE_FUNC;
	
	if(strcmp(lexer.pstrCurrLexeme, "PARAM") == 0)
		lexer.CurrToken = TOKEN_TYPE_PARAM;
	
	if(strcmp(lexer.pstrCurrLexeme, "_RETVAL") == 0)
		lexer.CurrToken = TOKEN_TYPE_REG_RETVAL;
	
	if(strcmp(lexer.pstrCurrLexeme, "SETPRIORITY") == 0)
		lexer.CurrToken = TOKEN_TYPE_SETPRIORITY;
	
	LSL_InstrLookup instr;
	if(tInstrTable.getInstrByMnemonic(lexer.pstrCurrLexeme, &instr))
		lexer.CurrToken = TOKEN_TYPE_INSTR;
	
	return lexer.CurrToken;
}



int LSL_Assembler::SkipToNextLine() {
	++lexer.iCurrSourceLine;
		
	if(lexer.iCurrSourceLine >= iSourceCodeSize)
		return FALSE;
	
	lexer.iIndex0 = 0;
	lexer.iIndex1 = 0;
	
	lexer.iCurrLexState = LEX_STATE_NO_STRING;
	
	return TRUE;
}

void LSL_Assembler::ResetLexer() {
	lexer.iCurrSourceLine = 0;
	
	lexer.iIndex0 = 0;
	lexer.iIndex1 = 0;
	
	lexer.CurrToken = TOKEN_TYPE_INVALID;
	
	lexer.iCurrLexState = LEX_STATE_NO_STRING;
}

char* LSL_Assembler::GetCurrLexeme() {
	return lexer.pstrCurrLexeme;
}

//forward reference
char LSL_Assembler::GetLookAheadChar() {
	int iCurrSourceLine = lexer.iCurrSourceLine;
	size_t iIndex		= lexer.iIndex1;
	
	if(lexer.iCurrLexState != LEX_STATE_IN_STRING) {
		while(TRUE) {
			if(iIndex >= strlen(ppstrSourceCode[iCurrSourceLine])) {
				iCurrSourceLine += 1;
				if(iCurrSourceLine >= iSourceCodeSize)
					return 0;
				
				iIndex = 0;
			}
		
			if(!isCharWhitespace(ppstrSourceCode[iCurrSourceLine][iIndex]))
				break;
			
			++iIndex;
		}
	}
	
	return ppstrSourceCode[iCurrSourceLine][iIndex];
}


void LSL_Assembler::ExitOnError(const char* pstrErrorMssg) {
	exitWithMssgBox(0, "Fatal Error: %s.\n", pstrErrorMssg);
}

void LSL_Assembler::ExitOnCodeError(const char* pstrErrorMssg) {
	logMssg("Error: %s.\n\n", pstrErrorMssg);
	logMssg("Line %d\n", lexer.iCurrSourceLine);
	
	char pstrSourceLine[ASM_MAX_SOURCE_LINE_SIZE];
	strcpy(pstrSourceLine, ppstrSourceCode[lexer.iCurrSourceLine]);
	
	for(size_t iCurrCharIndex = 0;
		iCurrCharIndex < strlen(pstrSourceLine);
		++iCurrCharIndex)
		if(pstrSourceLine[iCurrCharIndex] == '\t')
			pstrSourceLine[iCurrCharIndex] = ' ';

	logMssg("%s", pstrSourceLine);
	
	for(size_t iCurrSpace = 0;
		iCurrSpace < lexer.iIndex0;
		++iCurrSpace)
		logMssg(" ");
	logMssg("^\n");
	
	exitWithMssgBox(0, "Could not assemble script %s.\nScript Line: %s\n", pstrExecFilename.c_str(), pstrSourceLine);
}

void LSL_Assembler::ExitOnCharExpectedError(char cChar) {
	char* pstrErrorMssg = (char*)malloc(strlen("' ' expected"));
	sprintf(pstrErrorMssg, "'%c' expected", cChar);

	ExitOnCodeError(pstrErrorMssg);
}


//Parser

void LSL_Assembler::assembleSourceFile() {
	scriptHeader.iStackSize			= 0;
	scriptHeader.iIsMainFuncPresent	= FALSE;

	iInstrStreamSize				= 0;
	iIsSetStackSizeFound			= FALSE;
	scriptHeader.iGlobalDataSize	= 0;

	int			iIsFuncActive						= FALSE;
	LSL_FuncNode*	pCurrFunc;
	int			iCurrFuncIndex						= 0;
	char		pstrCurrFuncName[ASM_MAX_IDENT_SIZE];
	int			iCurrFuncParamCount					= 0;
	int			iCurrFuncLocalDataSize				= 0;

	LSL_InstrLookup CurrInstr;
	
	//* First Pass 
	
	ResetLexer();
	
	while(TRUE) {
		if(GetNextToken() == END_OF_TOKEN_STREAM)
			break;
	
		switch (lexer.CurrToken) {
			case TOKEN_TYPE_SETSTACKSIZE:
			{
				if(iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_LOCAL_SETSTACKSIZE);
			
				if(iIsSetStackSizeFound)
					ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);
			
				if (GetNextToken() != TOKEN_TYPE_INT) 
					ExitOnCodeError(ERROR_MSSG_INVALID_STACK_SIZE);
			
				scriptHeader.iStackSize = atoi(GetCurrLexeme());
				iIsSetStackSizeFound = TRUE;
				break;
			}
				
			case TOKEN_TYPE_SETPRIORITY: 
			{
				if(iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_LOCAL_PRIORITY);
				
				if(iIsPriorityFound)
					ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETPRIORITY);
				
				switch (GetNextToken()) {
					case TOKEN_TYPE_INT:
						scriptHeader.iUserPriority = atoi(GetCurrLexeme());
						scriptHeader.iPriorityType = PRIORITY_USER;
						break;
						
					case TOKEN_TYPE_IDENT: {
						char* pstrCurrPriorityIdent = lexer.pstrCurrLexeme;
						strupr(pstrCurrPriorityIdent);
				//		logMssg("\n set priority: %s\n", pstrCurrPriorityIdent);
						
						if(strcmp(pstrCurrPriorityIdent, PRIORITY_LOW_KEYWORD) == 0)
							scriptHeader.iPriorityType = PRIORITY_LOW;
						else if(strcmp(pstrCurrPriorityIdent, PRIORITY_MED_KEYWORD) == 0)
							scriptHeader.iPriorityType = PRIORITY_MED;
						else if(strcmp(pstrCurrPriorityIdent, PRIORITY_HIGH_KEYWORD) == 0)
							scriptHeader.iPriorityType = PRIORITY_HIGH;
						break;
					}
						
					default:
						ExitOnCodeError(ERROR_MSSG_INVALID_PRIORITY);
					
				}
				iIsPriorityFound = TRUE;
				break;
			}
			
			
			case TOKEN_TYPE_FUNC:
			{
				if(iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_NESTED_FUNC);
			
				if(GetNextToken() != TOKEN_TYPE_IDENT)
					ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
			
				char* pstrFuncName = GetCurrLexeme();
				int	  iEntryPoint  = iInstrStreamSize;
				int	  iFuncIndex   = tFuncTable.addFunc(pstrFuncName, iEntryPoint);
				
				if(iFuncIndex == -1)
					ExitOnCodeError(ERROR_MSSG_FUNC_REDEFINITION);
			
				if(strcmp(pstrFuncName, MAIN_FUNC_NAME) == 0) {
					scriptHeader.iIsMainFuncPresent	= TRUE;
					scriptHeader.iMainFuncIndex		= iFuncIndex;
				}
			
				iIsFuncActive = TRUE;
				strcpy(pstrCurrFuncName, pstrFuncName);
			
				iCurrFuncIndex			= iFuncIndex;
				iCurrFuncParamCount		= 0;
				iCurrFuncLocalDataSize	= 0;
				
#ifdef ASM_VERBOSE_MODE
				cout<<"Adding Func: "<<pstrFuncName<<" index;"<<iFuncIndex<<endl;
#endif
			
				while(GetNextToken() == TOKEN_TYPE_NEWLINE);
			
				if(lexer.CurrToken != TOKEN_TYPE_OPEN_BRACE)
					ExitOnCharExpectedError('{');
			
				// All functions are automatically appended with Ret, so increment the
				// required size of the instruction stream

				++iInstrStreamSize;
				break;
			}
			
			case TOKEN_TYPE_CLOSE_BRACE:
			{
				if(!iIsFuncActive)
					ExitOnCharExpectedError('}');
			
				tFuncTable.setFuncInfo(pstrCurrFuncName, iCurrFuncParamCount, iCurrFuncLocalDataSize);
			
				iIsFuncActive = FALSE;
				break;
			}
			
			case TOKEN_TYPE_VAR: 
			{
				if(GetNextToken() != TOKEN_TYPE_IDENT)
					ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
				
				char pstrIdent[ASM_MAX_IDENT_SIZE];
				strcpy(pstrIdent, GetCurrLexeme());
			
				int iSize = 1;
				int iStackIndex;
			
				if(GetLookAheadChar() == '[') {
					if(GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
						ExitOnCharExpectedError('[');
					
					if(GetNextToken() != TOKEN_TYPE_INT)
						ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
					
					iSize = atoi(GetCurrLexeme());
				
					if(iSize <= 0)
						ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
				
					if(GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
						ExitOnCharExpectedError(']');
				}
			
				if(iIsFuncActive)
					iStackIndex = -(iCurrFuncLocalDataSize + 2);
				else
					iStackIndex = scriptHeader.iGlobalDataSize;
			
				if(tSymbolTable.addSymbol(pstrIdent, iSize, iStackIndex, iCurrFuncIndex) == -1)
					ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);
			
				if(iIsFuncActive)
					iCurrFuncLocalDataSize += iSize;
				else
					scriptHeader.iGlobalDataSize += iSize;
			
				break;
			}
			
			case TOKEN_TYPE_PARAM:
			{
				if(!iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_GLOBAL_PARAM);
				
				if(strcmp(pstrCurrFuncName, MAIN_FUNC_NAME) == 0)
					ExitOnCodeError(ERROR_MSSG_MAIN_PARAM);
				
				if(GetNextToken() != TOKEN_TYPE_IDENT)
					ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
				
				++iCurrFuncParamCount;
				break;
			}
				
			case TOKEN_TYPE_IDENT:
			{
				if(GetLookAheadChar() != ':')
					ExitOnCodeError(ERROR_MSSG_INVALID_INSTR);
				
				if(!iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_GLOBAL_LINE_LABEL);
				
				char* pstrIdent = GetCurrLexeme();
				
				int iTargerIndex = iInstrStreamSize - 1;
				int iFuncIndex	 = iCurrFuncIndex;
				
				if(tLabelTable.addLabel(pstrIdent, iTargerIndex, iFuncIndex) == -1)
					ExitOnCodeError(ERROR_MSSG_LINE_LABEL_REDEFINITION);
				break;
			}
			
			case TOKEN_TYPE_INSTR: {
				if(!iIsFuncActive)
					ExitOnCodeError(ERROR_MSSG_GLOBAL_INSTR);
				++iInstrStreamSize;
				break;
			}
				
			
			default:
				if(lexer.CurrToken != TOKEN_TYPE_NEWLINE) {
					ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);
				}
				break;
		}
		if(!SkipToNextLine())
			break;
	}
	
#ifdef ASM_VERBOSE_MODE
	cout<<"Instr Stream Size: "<<iInstrStreamSize<<endl;
#endif
	
	pInstrStream = (LSL_Instr*)malloc(iInstrStreamSize*sizeof(LSL_Instr));
	int iCurrInstrIndex = 0;
	for(iCurrInstrIndex; iCurrInstrIndex < iInstrStreamSize; ++iCurrInstrIndex)
		pInstrStream[iCurrFuncIndex].pOpList = NULL;
	
	iCurrInstrIndex = 0;
	
	//* Second Pass 
	
	ResetLexer();
	
	while(TRUE) {
		if(GetNextToken() == END_OF_TOKEN_STREAM)
			break;
		
		switch (lexer.CurrToken) {
			case TOKEN_TYPE_FUNC:
			{
				GetNextToken();
				pCurrFunc = tFuncTable.getFuncByName(GetCurrLexeme());
				
				iIsFuncActive		= TRUE;
				iCurrFuncParamCount = 0;
				iCurrFuncIndex		= pCurrFunc->iIndex;
				
				while(GetNextToken() == TOKEN_TYPE_NEWLINE);
				
				break;
			}
				
			case TOKEN_TYPE_CLOSE_BRACE:
			{
				iIsFuncActive = FALSE;
				if(pCurrFunc->sFuncName.compare(MAIN_FUNC_NAME) == 0) {
					pInstrStream[iCurrInstrIndex].iOpcode					= INSTR_EXIT;
					pInstrStream[iCurrInstrIndex].iOpCount					= 1;
					pInstrStream[iCurrInstrIndex].pOpList					= new LSL_Op;
					pInstrStream[iCurrInstrIndex].pOpList[0].iType			= OP_TYPE_INT;
					pInstrStream[iCurrInstrIndex].pOpList[0].iIntLiteral	= 0;
				}
				else {
					pInstrStream[iCurrInstrIndex].iOpcode	= INSTR_RET;
					pInstrStream[iCurrInstrIndex].iOpCount	= 0;
					pInstrStream[iCurrInstrIndex].pOpList	= NULL;
				}
				
#ifdef ASM_VERBOSE_MODE
				cout<<"adding instr: "<<pInstrStream[iCurrInstrIndex].iOpcode<<" count:"<<pInstrStream[iCurrInstrIndex].iOpCount<<endl;
#endif
				++iCurrInstrIndex;
				break;
			}
				
			case TOKEN_TYPE_PARAM:
			{
				if(GetNextToken() != TOKEN_TYPE_IDENT)
					ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
				
				char* pstrIdent = GetCurrLexeme();
				int iStackIndex = -(pCurrFunc->iLocalDataSize+2+(iCurrFuncParamCount+1));
				
				if(tSymbolTable.addSymbol(pstrIdent, 1, iStackIndex, iCurrFuncIndex) == -1)
					ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);
				
				++iCurrFuncParamCount;
				break;
			}
				
			case TOKEN_TYPE_INSTR:
			{
				tInstrTable.getInstrByMnemonic(GetCurrLexeme(), &CurrInstr);
				
				pInstrStream[iCurrInstrIndex].iOpcode = CurrInstr.iOpcode;
				
				pInstrStream[iCurrInstrIndex].iOpCount = CurrInstr.iOpCount;
				
#ifdef ASM_VERBOSE_MODE
				cout<<"adding instr: "<<pInstrStream[iCurrInstrIndex].iOpcode<<" opcount:"<<pInstrStream[iCurrInstrIndex].iOpCount<<endl;
#endif
				
				LSL_Op* pOpList = (LSL_Op*)malloc(CurrInstr.iOpCount*sizeof(LSL_Op));
				
				for(int iCurrOpIndex = 0; iCurrOpIndex < CurrInstr.iOpCount; ++iCurrOpIndex) {
					LSL_OpTypes CurrOpTypes = CurrInstr.opList[iCurrOpIndex];
					
					Token initOpToken = GetNextToken();
					switch (initOpToken) {
						case TOKEN_TYPE_INT:
							if(CurrOpTypes & OP_FLAG_TYPE_INT) {
								pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
								pOpList[iCurrOpIndex].iIntLiteral = atoi(GetCurrLexeme());
							} else {
								ExitOnCodeError(ERROR_MSSG_INVALID_OP);
							}
							break;
							
						case TOKEN_TYPE_FLOAT:
							if(CurrOpTypes & OP_FLAG_TYPE_FLOAT) {
								pOpList[iCurrOpIndex].iType = OP_TYPE_FLOAT;
								pOpList[iCurrOpIndex].fFloatLiteral = static_cast<float>(atof(GetCurrLexeme()));
							} else {
								ExitOnCodeError(ERROR_MSSG_INVALID_OP);
							}
							break;
							
						case TOKEN_TYPE_QUOTE:
						{
							if(CurrOpTypes & OP_FLAG_TYPE_STRING) {
								GetNextToken();
								
								switch (lexer.CurrToken) {
									case TOKEN_TYPE_QUOTE:
									{
										pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
										pOpList[iCurrOpIndex].iIntLiteral = 0;
										break;
									}
										
									case TOKEN_TYPE_STRING:
									{
										char* pstrString = GetCurrLexeme();
										int iStringIndex = tStringTable.addString(pstrString);
										
										if(GetNextToken() != TOKEN_TYPE_QUOTE)
											ExitOnCharExpectedError('\\');
										
										pOpList[iCurrOpIndex].iType = OP_TYPE_STRING_INDEX;
										pOpList[iCurrOpIndex].iStringTableIndex = iStringIndex;
										break;
									}
										
									default:
										ExitOnCodeError(ERROR_MSSG_INVALID_STRING);
										break;
								}
							} else {
								ExitOnCodeError(ERROR_MSSG_INVALID_OP);
							}

							break;
						}
							
						case TOKEN_TYPE_REG_RETVAL:
						{
							if(CurrOpTypes & OP_FLAG_TYPE_REG) {
								pOpList[iCurrOpIndex].iReg = 0;
								pOpList[iCurrOpIndex].iType = OP_TYPE_REG;
							} else {
								ExitOnCodeError(ERROR_MSSG_INVALID_OP);
							}
							break;
						}
							
					// These operands can be
					//      - Variables/Array Indices
					//      - Line Labels
					//      - Function Names
					//      - Host API Calls
							
						case TOKEN_TYPE_IDENT:
						{
							if(CurrOpTypes & OP_FLAG_TYPE_MEM_REF) {
								char pstrIdent[ASM_MAX_IDENT_SIZE];
								strcpy(pstrIdent, GetCurrLexeme());
								
								if(!tSymbolTable.getSymbolByIdent(pstrIdent, iCurrFuncIndex))
									ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);
								
								int iBaseIndex = tSymbolTable.getStackIndexByIdent(pstrIdent, iCurrFuncIndex);
								
								if(GetLookAheadChar() != '[') {
									if(tSymbolTable.getSizeByIdent(pstrIdent, iCurrFuncIndex) > 1)
										ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED);
									
									pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
									pOpList[iCurrOpIndex].iStackIndex = iBaseIndex;
								} else {
									if(tSymbolTable.getSizeByIdent(pstrIdent, iCurrFuncIndex) == 1)
										ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY);
									
									if(GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
										ExitOnCharExpectedError('[');
									
									Token indexToken = GetNextToken();
									if(indexToken == TOKEN_TYPE_INT) {
										int iOffsetIndex = atoi(GetCurrLexeme());
										
										pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
										if(iBaseIndex > 0)
											pOpList[iCurrOpIndex].iStackIndex = iBaseIndex + iOffsetIndex;
										else
											pOpList[iCurrOpIndex].iStackIndex = iBaseIndex - iOffsetIndex;
									} else if(indexToken == TOKEN_TYPE_IDENT) {
										char* pstrIndexIdent = GetCurrLexeme();
										
										if(!tSymbolTable.getSymbolByIdent(pstrIndexIdent, iCurrFuncIndex))
											ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);
										if(tSymbolTable.getSizeByIdent(pstrIndexIdent, iCurrFuncIndex) > 1)
											ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
										
										int iOffsetIndex = tSymbolTable.getStackIndexByIdent(pstrIndexIdent, iCurrFuncIndex);
										
										pOpList[iCurrOpIndex].iType			= OP_TYPE_REL_STACK_INDEX;
										pOpList[iCurrOpIndex].iStackIndex	= iBaseIndex;
										pOpList[iCurrOpIndex].iOffsetIndex	= iOffsetIndex;
									} else {
										ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
									}
									
									if(GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET) 
										ExitOnCharExpectedError('[');

								}
							 }
							
							if(CurrOpTypes & OP_FLAG_TYPE_LINE_LABEL) {
								char* pstrLabelIdent = GetCurrLexeme();
								
								LSL_LabelNode* pLabel = tLabelTable.getLabelByIdent(pstrLabelIdent, iCurrFuncIndex);
								
								if(!pLabel)
									ExitOnCodeError(ERROR_MSSG_UNDEFINED_LINE_LABEL);
								
								pOpList[iCurrOpIndex].iType = OP_TYPE_INSTR_INDEX;
								pOpList[iCurrOpIndex].iInstrIndex = pLabel->iTargetIndex;
							}
							
							if(CurrOpTypes & OP_FLAG_TYPE_FUNC_NAME) {
								char* pstrFuncName = GetCurrLexeme();
								
								LSL_FuncNode* pFunc = tFuncTable.getFuncByName(pstrFuncName);
								
								if(!pFunc)
									ExitOnCodeError(ERROR_MSSG_UNDEFINED_FUNC);
								
								pOpList[iCurrOpIndex].iType = OP_TYPE_FUNC_INDEX;
								pOpList[iCurrOpIndex].iFuncIndex = pFunc->iIndex;
							}
							
							if(CurrOpTypes & OP_FLAG_TYPE_HOST_API_CALL) {
								char* pstrHostAPICall = GetCurrLexeme();
								int iIndex = tStringTable.addString(pstrHostAPICall);
								
								pOpList[iCurrOpIndex].iType = OP_TYPE_HOST_API_CALL_INDEX;
								pOpList[iCurrOpIndex].iHostAPICallIndex = iIndex;
							}
							break;
						}
							
						default:
							ExitOnCodeError(ERROR_MSSG_INVALID_OP);
							break;
					}
					
					if(iCurrOpIndex < CurrInstr.iOpCount-1)
						if(GetNextToken() != TOKEN_TYPE_COMMA)
							ExitOnCharExpectedError(',');
				}
				if(GetNextToken() != TOKEN_TYPE_NEWLINE) {
					ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);
				}
				
				pInstrStream[iCurrInstrIndex].pOpList = pOpList;
				++iCurrInstrIndex;
				break;
			}
		}
		if(!SkipToNextLine())
			break;
	}
}

void LSL_Assembler::buildAssembledFile(const char* pstrFileName) {
	FILE* pExecFile;
	if(!(pExecFile = fopen(pstrFileName, "wb")))
		ExitOnError("Could not open executable file for output");
	
	//header
	fwrite(LOLISCRIPT_HEADER_ID, 4, 1, pExecFile);
	
	//version
	char cVersionMajor = ASM_VERSION_MAJOR;
	char cVersionMinor = ASM_VERSION_MINOR;
	fwrite(&cVersionMajor, 1, 1, pExecFile);
	fwrite(&cVersionMinor, 1, 1, pExecFile);
	
	//stack
	fwrite(&scriptHeader.iStackSize, 4, 1, pExecFile);
	
	//global data
	fwrite(&scriptHeader.iGlobalDataSize, 4, 1, pExecFile);
	
	//priority
	char cPriorityType = scriptHeader.iPriorityType;
	fwrite(&cPriorityType, 1, 1, pExecFile);
	fwrite(&scriptHeader.iUserPriority, 4, 1, pExecFile);
	
	//_main()
	char cIsMainPresent = 0;
	if(scriptHeader.iIsMainFuncPresent)
		cIsMainPresent = 1;
	fwrite(&cIsMainPresent, 1, 1, pExecFile);
	
	fwrite(&scriptHeader.iMainFuncIndex, 4, 1, pExecFile);
	
	//instruction stream
	fwrite(&iInstrStreamSize, 4, 1, pExecFile);
	
	for(int iCurrInstrIndex = 0; iCurrInstrIndex < iInstrStreamSize; ++iCurrInstrIndex) {
		//operand
		short sOpcode = pInstrStream[iCurrInstrIndex].iOpcode;
		fwrite(&sOpcode, 2, 1, pExecFile);
		
		//operand count
		char iOpCount = pInstrStream[iCurrInstrIndex].iOpCount;
		fwrite(&iOpCount, 1, 1, pExecFile);
		
#ifdef ASM_VERBOSE_MODE
		cout<<"Writing Instruction, iOpcode: "<<pInstrStream[iCurrInstrIndex].iOpcode<<" iOpCount"<<pInstrStream[iCurrInstrIndex].iOpCount<<endl;
#endif
		
		for(int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
			LSL_Op CurrOp = pInstrStream[iCurrInstrIndex].pOpList[iCurrOpIndex];
			
			char cOpType = CurrOp.iType;
			fwrite(&cOpType, 1, 1, pExecFile);
	
#ifdef ASM_VERBOSE_MODE
			cout<<"			Writing OpType: "<<CurrOp.iType<<endl;
#endif
			
			switch (CurrOp.iType) {
				case OP_TYPE_INT:
					fwrite(&CurrOp.iIntLiteral, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_FLOAT:
					fwrite(&CurrOp.fFloatLiteral, sizeof(float), 1, pExecFile);
					break;
					
				case OP_TYPE_STRING_INDEX:
					fwrite(&CurrOp.iStringTableIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_INSTR_INDEX:
					fwrite(&CurrOp.iInstrIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_ABS_STACK_INDEX:
					fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_REL_STACK_INDEX:
					fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
					fwrite(&CurrOp.iOffsetIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_FUNC_INDEX:
					fwrite(&CurrOp.iFuncIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_HOST_API_CALL_INDEX:
					fwrite(&CurrOp.iHostAPICallIndex, sizeof(int), 1, pExecFile);
					break;
					
				case OP_TYPE_REG:
					fwrite(&CurrOp.iReg, sizeof(int), 1, pExecFile);
					break;
			}
		}
	}
	
	//string table
	
	list<string>::iterator pStringNode = tStringTable.stringTable.begin();
	
	int itStringTableSize = tStringTable.stringTable.size();
	fwrite(&itStringTableSize, 4, 1, pExecFile);
	
	char cParamCount;
	
	for(pStringNode; pStringNode != tStringTable.stringTable.end(); ++pStringNode) {
		string sCurrString			= *pStringNode;
		int	   iCurrStringLength	= sCurrString.size();
		
		fwrite(&iCurrStringLength, 4, 1, pExecFile);
		fwrite(sCurrString.c_str(), sCurrString.size(), 1, pExecFile);
		
#ifdef ASM_VERBOSE_MODE
		cout<<"Writing String: "<<sCurrString<<" Size:"<<sCurrString.size()<<endl;
#endif
	}
	
	//function table
	
	int itFuncTableSize = tFuncTable.funcTable.size();
	fwrite(&itFuncTableSize, 4, 1, pExecFile);
	
	list<LSL_FuncNode*>::iterator pFuncNode = tFuncTable.funcTable.begin();
	
	for(pFuncNode; pFuncNode != tFuncTable.funcTable.end(); ++pFuncNode) {
		LSL_FuncNode* pFunc = (*pFuncNode);
		
		fwrite(&pFunc->iEntryPoint, sizeof(int), 1, pExecFile);
		
		cParamCount = pFunc->iParamCount;
		fwrite(&cParamCount, 1, 1, pExecFile);
		
		fwrite(&pFunc->iLocalDataSize, sizeof(int), 1, pExecFile);
		
		char cFuncNameLength = pFunc->sFuncName.size();
		fwrite(&cFuncNameLength, 1, 1, pExecFile);
		fwrite(pFunc->sFuncName.c_str(), pFunc->sFuncName.size(), 1, pExecFile);
#ifdef ASM_VERBOSE_MODE
		cout<<"Writing Func Node: "<<pFunc->iEntryPoint<<" DataSize:"<<pFunc->iLocalDataSize<<endl;
#endif
	}
	
	//host API call table
	
	int iHostAPICallSize = tHostAPICallTable.stringTable.size();
	fwrite(&iHostAPICallSize, 4, 1, pExecFile);
	
	for(pStringNode = tHostAPICallTable.stringTable.begin();
		pStringNode != tHostAPICallTable.stringTable.end();
		++pStringNode) {
		
		char cCurrHostAPICallLength = pStringNode->size();
		
		fwrite(&cCurrHostAPICallLength, 1, 1, pExecFile);
		fwrite(pStringNode->c_str(), pStringNode->size(), 1, pExecFile);
#ifdef ASM_VERBOSE_MODE
		cout<<"Writing Host API Call Table: "<<*pStringNode<<"   size:"<<pStringNode->size()<<endl;
#endif
	}
	
	fclose(pExecFile);
		
}

#include <fstream>
#include <string>
using std::ifstream;
using std::string;

#include <iostream>
using std::cout;

bool LSL_Assembler::loadSourceFile(const char* g_pstrSourceFilename) {
	if (!(pSourceFile = fopen(g_pstrSourceFilename, "rb" )))
		return false;
	
	// Count the number of source lines
	
	while(!feof(pSourceFile))
		if(fgetc(pSourceFile) == '\n')
			++iSourceCodeSize;
	++iSourceCodeSize;
	
	fclose(pSourceFile);
	
	// Reopen the source file in ASCII mode
	
//	if(!(pSourceFile = fopen(g_pstrSourceFilename, "r")))
//		ExitOnError("Could not open source file");
		
	ifstream source(g_pstrSourceFilename);
	if(!source.is_open())
		return false;

	if (!(ppstrSourceCode = (char **)malloc(iSourceCodeSize * sizeof(char *))))
		return false;
	
	// Read the source code in from the file
	
/*	for(int iCurrLineIndex = 0; iCurrLineIndex < iSourceCodeSize; ++iCurrLineIndex) {
		if (!(ppstrSourceCode[iCurrLineIndex] = (char *)malloc(ASM_MAX_SOURCE_LINE_SIZE+1)))
			ExitOnError("Could not allocate space for source line");
		
		fgets(ppstrSourceCode[iCurrLineIndex], ASM_MAX_SOURCE_LINE_SIZE, pSourceFile);
		
		// Strip comments and trim whitespace
		
		StripComments(ppstrSourceCode[iCurrLineIndex]);
		TrimWhitespace(ppstrSourceCode[iCurrLineIndex]);
		
		int iNewLineIndex = strlen (ppstrSourceCode[iCurrLineIndex])-1;
		if((int)ppstrSourceCode[iCurrLineIndex][iNewLineIndex] != 10) {
			ppstrSourceCode[iCurrLineIndex][iNewLineIndex+1] = '\n';
			ppstrSourceCode[iCurrLineIndex][iNewLineIndex+2] = '\0';
		}
	}*/

	string buffer;
	int count = 0;
	while(getline(source, buffer)) {
		if(buffer.find(';') != string::npos) {
			buffer.erase(buffer.find(';'), buffer.size());
			buffer += '\n';
		}
		if(buffer.size() > 1 && !isStringWhitespace(buffer.c_str())) {
			while(isCharWhitespace(buffer[buffer.size()-1]))
				buffer.erase(buffer.end()-1);
			while(isCharWhitespace(buffer[0]))
				buffer.erase(buffer.begin());
			ppstrSourceCode[count] = (char*)malloc(buffer.size()+2);
			strcpy(ppstrSourceCode[count], buffer.c_str());
			ppstrSourceCode[count][buffer.size()] = '\n';
			ppstrSourceCode[count][buffer.size()+1] = '\0';
			++count;
		}
	}
	iSourceCodeSize = count;
	source.close();
	return true;
//	fclose(pSourceFile);
}

void LSL_Assembler::printAssembleStates(const char* pstrFileName) {
	int iVarCount = 0;
	int iArrayCount = 0;
	int iGlobalCount = 0;
	
	for(list<LSL_SymbolNode*>::iterator p = tSymbolTable.symbolTable.begin();
		p != tSymbolTable.symbolTable.end();
		++p) {
		LSL_SymbolNode* pCurrSymbol = *p;
		if(pCurrSymbol->iSize > 1)
			++ iArrayCount;
		else {
			++iVarCount;
		}
		
		if(pCurrSymbol->iStackIndex >= 0)
			++iGlobalCount;
	}
	
	logMssg("%s created successfully!\n\n", pstrFileName);
	logMssg("Source Lines Processed: %d\n", iSourceCodeSize);
	
	logMssg("            Stack Size: ");
	if(scriptHeader.iStackSize)
		logMssg("%d", scriptHeader.iStackSize);
	else
		logMssg("Default");
	
	logMssg("\n");
	logMssg("Instructions Assembled: %d\n", iInstrStreamSize);
	logMssg("             Variables: %d\n", iVarCount);
	logMssg("                Arrays: %d\n", iArrayCount);
	logMssg("               Globals: %d\n", iGlobalCount);
	logMssg("       String Literals: %d\n", (int)tStringTable.stringTable.size());
	logMssg("                Labels: %d\n", (int)tLabelTable.labelTable.size());
	logMssg("        Host API Calls: %d\n", (int)tHostAPICallTable.stringTable.size());
	logMssg("             Functions: %d\n", (int)tFuncTable.funcTable.size());
	
	logMssg("      _Main () Present: ");
	if(scriptHeader.iIsMainFuncPresent)
		logMssg("YES (Index %d)\n", scriptHeader.iMainFuncIndex);
	else
		logMssg("No\n");
	
}


void LSL_Assembler::initInstrTable() {
	int iInstrIndex;
	// ---- Main
			
	// Mov          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Mov", INSTR_MOV, 2);
	tInstrTable.setOpType(iInstrIndex, 0,  OP_FLAG_TYPE_MEM_REF |
										    OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,  OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// ---- Arithmetic
			
	// Add         Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Add", INSTR_ADD, 2);
	tInstrTable.setOpType(iInstrIndex, 0,  OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,  OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Sub          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Sub", INSTR_SUB, 2);
	tInstrTable.setOpType(iInstrIndex, 0,  OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,  OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Mul          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Mul", INSTR_MUL, 2);
	tInstrTable.setOpType(iInstrIndex, 0,  OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Div          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Div", INSTR_DIV, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Mod          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Mod", INSTR_MOD, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Exp          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Exp", INSTR_EXP, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Neg          Destination
			
	iInstrIndex = tInstrTable.addInstrLookup("Neg", INSTR_NEG, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Inc          Destination
			
	iInstrIndex = tInstrTable.addInstrLookup("Inc", INSTR_INC, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Dec          Destination
			
	iInstrIndex = tInstrTable.addInstrLookup("Dec", INSTR_DEC, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Sin          Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Sin", INSTR_SIN, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Cos          Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Cos", INSTR_COS, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Tan          Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Tan", INSTR_TAN, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Ln           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Ln", INSTR_LN, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Lg           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Lg", INSTR_LG, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Expl           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Expl", INSTR_EXPL, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Sh           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Sh", INSTR_SH, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Ch           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Ch", INSTR_CH, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Th           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Th", INSTR_TH, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Rec           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Rec", INSTR_REC, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Sqrt           Destination, Source
	
	iInstrIndex = tInstrTable.addInstrLookup("Sqrt", INSTR_SQRT, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );

			
	// ---- Bitwise
			
	// And          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("And", INSTR_AND, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Or           Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Or", INSTR_OR, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// XOr          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("XOr", INSTR_XOR, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Not          Destination
			
	iInstrIndex = tInstrTable.addInstrLookup("Not", INSTR_NOT, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// ShL          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("ShL", INSTR_SHL, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// ShR          Destination, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("ShR", INSTR_SHR, 2);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// ---- String Manipulation
			
	// Concat       String0, String1
			
	iInstrIndex = tInstrTable.addInstrLookup("Concat", INSTR_CONCAT, 2 );
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG |
											OP_FLAG_TYPE_STRING );
			
	// GetChar      Destination, Source, Index
			
	iInstrIndex = tInstrTable.addInstrLookup("GetChar", INSTR_GETCHAR, 3 );
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG |
											OP_FLAG_TYPE_STRING );
	tInstrTable.setOpType(iInstrIndex, 2,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG |
											OP_FLAG_TYPE_INT );
			
	// SetChar      Destination, Index, Source
			
	iInstrIndex = tInstrTable.addInstrLookup("SetChar", INSTR_SETCHAR, 3 );
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG |
											OP_FLAG_TYPE_INT );
	tInstrTable.setOpType(iInstrIndex, 2,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG |
											OP_FLAG_TYPE_STRING );
			
	// ---- Conditional Branching
			
	// Jmp          Label
			
	iInstrIndex = tInstrTable.addInstrLookup("Jmp", INSTR_JMP, 1);
	tInstrTable.setOpType(iInstrIndex, 0, OP_FLAG_TYPE_LINE_LABEL);
			
	// JE           Op0, Op1, Label
			
	iInstrIndex = tInstrTable.addInstrLookup("JE", INSTR_JE, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );
			
	// JNE          Op0, Op1, Label
	
	iInstrIndex = tInstrTable.addInstrLookup("JNE", INSTR_JNE, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );
			
	// JG           Op0, Op1, Label
	
	iInstrIndex = tInstrTable.addInstrLookup("JG", INSTR_JG, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );
			
	// JL           Op0, Op1, Label
	
	iInstrIndex = tInstrTable.addInstrLookup("JL", INSTR_JL, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );
			
	// JGE          Op0, Op1, Label
	
	iInstrIndex = tInstrTable.addInstrLookup("JGE", INSTR_JGE, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );
			
	// JLE           Op0, Op1, Label
	
	iInstrIndex = tInstrTable.addInstrLookup("JLE", INSTR_JLE, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
						   					OP_FLAG_TYPE_FLOAT |
						   					OP_FLAG_TYPE_STRING |
						   					OP_FLAG_TYPE_MEM_REF |
						   					OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

	// ---- The Stack Interface
			
	// Push          Source
			
	iInstrIndex = tInstrTable.addInstrLookup("Push", INSTR_PUSH, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Pop           Destination
			
	iInstrIndex = tInstrTable.addInstrLookup("Pop", INSTR_POP, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// ---- The Function Interface
			
	// Call          FunctionName
			
	iInstrIndex = tInstrTable.addInstrLookup("Call", INSTR_CALL, 1);
	tInstrTable.setOpType(iInstrIndex, 0, OP_FLAG_TYPE_FUNC_NAME );
			
	// Ret
			
	iInstrIndex = tInstrTable.addInstrLookup("Ret", INSTR_RET, 0);
			
	// CallHost      FunctionName
			
	iInstrIndex = tInstrTable.addInstrLookup("CallHost", INSTR_CALLHOST, 1);
	tInstrTable.setOpType(iInstrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL );
			
	// ---- Miscellaneous
			
	// Pause        Duration
			
	iInstrIndex = tInstrTable.addInstrLookup("Pause", INSTR_PAUSE, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
			
	// Exit         Code
			
	iInstrIndex = tInstrTable.addInstrLookup("Exit", INSTR_EXIT, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// Print        Code
	
	iInstrIndex = tInstrTable.addInstrLookup("Print", INSTR_PRINT, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// PushParam        Code
	
	iInstrIndex = tInstrTable.addInstrLookup("PushParam", INSTR_PUSHPARAM, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
				
	// PushHostParam        Code
	
	iInstrIndex = tInstrTable.addInstrLookup("PushHParam", INSTR_PUSHHOSTPARAM, 1);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_STRING |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// RandomInt	Number,		Start,    End
	iInstrIndex = tInstrTable.addInstrLookup("RandomInt", INSTR_RANDOMINT, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2,	OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	
	// RandomFloat	Number,		Start,    End
	iInstrIndex = tInstrTable.addInstrLookup("RandomFloat", INSTR_RANDOMFLOAT, 3);
	tInstrTable.setOpType(iInstrIndex, 0,	OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 1,	OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
	tInstrTable.setOpType(iInstrIndex, 2,	OP_FLAG_TYPE_FLOAT |
											OP_FLAG_TYPE_INT |
											OP_FLAG_TYPE_MEM_REF |
											OP_FLAG_TYPE_REG );
}