/*
 *  lexer.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "lexer.h"

#include <iostream>

using std::cout;
using std::endl;

LSH_lexer::LSH_lexer() {
	pstrSource = NULL;
	initLexer();
}

LSH_lexer::~LSH_lexer() {
	if(pstrSource)
		free(pstrSource);
}

void LSH_lexer::initLexer() {
	iCurrLexemeStart = iCurrLexemeEnd = iCurrOp = 0;
	
	if(pstrSource != NULL)
		free(pstrSource);
	lSourceSize = 0;
	pstrSource = NULL;
}

char LSH_lexer::getNextChar() {
	return pstrSource[iCurrLexemeEnd++];
}

int LSH_lexer::loadFile(const char* pstrFileName) {
	FILE* pSourceFile;
	if(!(pSourceFile = fopen(pstrFileName, "rb"))) {
		printf("File I/O error.\n");
		return FALSE;
	}
	
	fseek(pSourceFile, 0, SEEK_END);
	lSourceSize = ftell(pSourceFile);
	fseek(pSourceFile, 0, SEEK_SET);
	pstrSource = (char*)malloc(lSourceSize+1);
	
	char cCurrChar;
	for(int iCurrCharIndex = 0; iCurrCharIndex < lSourceSize; ++iCurrCharIndex) {
		cCurrChar = fgetc(pSourceFile);
		//unix/linux line sperator 10
		//win 13 || 10
		if(cCurrChar == 13) {
			fgetc(pSourceFile);
			--lSourceSize;
			pstrSource[iCurrCharIndex] = '\n';
		} else {
			pstrSource[iCurrCharIndex] = cCurrChar;
		}
	}
	pstrSource[lSourceSize] = '\0';
	
	printf("%s\n", pstrSource);
	
	fclose(pSourceFile);
	
	return TRUE;
}

Token LSH_lexer::getNextToken() {
	iCurrLexemeStart = iCurrLexemeEnd;
	if(iCurrLexemeStart >= lSourceSize)
		return TOKEN_TYPE_END_OF_STREAM;
	
	int iCurrState = LEX_STATE_START;
	int iLexemeDone = FALSE;
	
	int iCurrOpCharIndex = 0;
	int iCurrOpStateIndex = 0;
	opState opCurrOpState;
	
	char cCurrChar;
	
	int iNextLexemeCharIndex = 0;
	
	int iAddCurrChar;
	
	while(TRUE) {
		cCurrChar = getNextChar();
		if(cCurrChar == '\0')
			break;
		
		iAddCurrChar = TRUE;
		
		switch (iCurrState) {
			case LEX_STATE_START:
				if(isCharWhitespaceLex(cCurrChar)) {
					++iCurrLexemeStart;
					iAddCurrChar = FALSE;
				} else if(isCharNumericLex(cCurrChar)) {
					iCurrState = LEX_STATE_INT;
				} else if(cCurrChar == '.') {
					iCurrState = LEX_STATE_FLOAT;
				} else if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else if(isCharDelimiterLex(cCurrChar)) {
					iCurrState = LEX_STATE_DELIM;
				} else if(isCharOpChar(cCurrChar, 0)) {
					iCurrOpStateIndex = getOpStateIndex(cCurrChar, 0, 0, 0);
					if(iCurrOpStateIndex == -1)
						ExitOnInvalidInputError(cCurrChar);
					
					opCurrOpState = getOpState(0, iCurrOpStateIndex);
					iCurrOpCharIndex = 1;
					
					iCurrOp = opCurrOpState.iIndex;
					
					iCurrState = LEX_STATE_OP;
				} else if(cCurrChar == '"') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_STRING;
				}
				else
					ExitOnInvalidInputError(cCurrChar);
				break;
				
			case LEX_STATE_INT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_INT;
				else if(cCurrChar == '.')
					iCurrState = LEX_STATE_FLOAT;
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || isCharOpChar(cCurrChar, 0)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else
					ExitOnInvalidInputError(cCurrChar);
				break;
				
			case LEX_STATE_FLOAT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_FLOAT;
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || isCharOpChar(cCurrChar, 0)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else
					ExitOnInvalidInputError(cCurrChar);
				break;
				
			case LEX_STATE_IDENT:
				if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || isCharOpChar(cCurrChar, 0)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else
					ExitOnInvalidInputError(cCurrChar);
				break;
				
			case LEX_STATE_OP:
				if(opCurrOpState.iSubStateCount == 0) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
					break;
				}
				
				if(isCharOpChar(cCurrChar, iCurrOpCharIndex)) {
					iCurrOpStateIndex = getOpStateIndex(cCurrChar, iCurrOpCharIndex, opCurrOpState.iSubStateIndex, opCurrOpState.iSubStateCount);
					if(iCurrOpCharIndex == -1)
						ExitOnInvalidInputError(cCurrChar);
					
					opCurrOpState = getOpState(iCurrOpCharIndex, iCurrOpStateIndex);
					
					++iCurrOpCharIndex;
					iCurrOp = opCurrOpState.iIndex;
				} else {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				}
				break;
				
			case LEX_STATE_DELIM:
				iAddCurrChar = FALSE;
				iLexemeDone = TRUE;
				break;
				
			case LEX_STATE_STRING:
				if(cCurrChar == '"') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_STRING_CLOSE_QUOTE;
				} else if(cCurrChar == '\\') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_STRING_ESCAPE;
				}
				break;
				
			case LEX_STATE_STRING_ESCAPE:
				iCurrState = LEX_STATE_STRING;
				break;
				
			case LEX_STATE_STRING_CLOSE_QUOTE:
				iAddCurrChar = FALSE;
				iLexemeDone = TRUE;
				break;
				
			default:
				break;
		}
		if(iAddCurrChar) {
			pstrCurrLexeme[iNextLexemeCharIndex] = cCurrChar;
			++iNextLexemeCharIndex;
		}
		
		if(iLexemeDone)
			break;
	}
		
	pstrCurrLexeme[iNextLexemeCharIndex] = '\0';
	--iCurrLexemeEnd;

	Token tokenType;
	switch (iCurrState) {
		case LEX_STATE_INT:
			tokenType = TOKEN_TYPE_INT;
			break;
				
		case LEX_STATE_FLOAT:
			tokenType = TOKEN_TYPE_FLOAT;
			break;
			
		case LEX_STATE_IDENT:
			tokenType = TOKEN_TYPE_IDENT;
			if(strcasecmp(pstrCurrLexeme, "var") == 0)
				tokenType = TOKEN_TYPE_RSRVD_VAR;
			else if(strcasecmp(pstrCurrLexeme, "true") == 0)
				tokenType = TOKEN_TYPE_RSRVD_TRUE;
			else if(strcasecmp(pstrCurrLexeme, "false") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FALSE;
			else if(strcasecmp(pstrCurrLexeme, "if") == 0)
				tokenType = TOKEN_TYPE_RSRVD_IF;
			else if(strcasecmp(pstrCurrLexeme, "else") == 0)
				tokenType = TOKEN_TYPE_RSRVD_ELSE;
			else if(strcasecmp(pstrCurrLexeme, "break") == 0)
				tokenType = TOKEN_TYPE_RSRVD_BREAK;
			else if(strcasecmp(pstrCurrLexeme, "continue") == 0)
				tokenType = TOKEN_TYPE_RSRVD_CONTINUE;
			else if(strcasecmp(pstrCurrLexeme, "for") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FOR;
			else if(strcasecmp(pstrCurrLexeme, "while") == 0)
				tokenType = TOKEN_TYPE_RSRVD_WHILE;
			else if(strcasecmp(pstrCurrLexeme, "func") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FUNC;
			else if(strcasecmp(pstrCurrLexeme, "return") == 0)
				tokenType = TOKEN_TYPE_RSRVD_RETURN;
			break;
				
		case LEX_STATE_DELIM:
			switch (pstrCurrLexeme[0]) {
				case ',':
					tokenType = TOKEN_TYPE_DELIM_COMMA;
					break;
					
				case '(':
					tokenType = TOKEN_TYPE_DELIM_OPEN_PAREN;
					break;
					
				case ')':
					tokenType = TOKEN_TYPE_DELIM_CLOSE_PAREN;
					break;
					
				case '[':
					tokenType = TOKEN_TYPE_DELIM_OPEN_BRACE;
					break;
					
				case ']':
					tokenType = TOKEN_TYPE_DELIM_CLOSE_BRACE;
					break;
					
				case '{':
					tokenType = TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE;
					break;
					
				case '}':
					tokenType = TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE;
					break;
					
				case ';':
					tokenType = TOKEN_TYPE_DELIM_SEMICOLON;
					break;
					
				default:
					break;
			}
			break;
			
		case LEX_STATE_OP:
			tokenType = TOKEN_TYPE_OP;
			break;
			
		case LEX_STATE_STRING_CLOSE_QUOTE:
			tokenType = TOKEN_TYPE_STRING;
			break;
			
		default:
			tokenType = TOKEN_TYPE_END_OF_STREAM;
			break;
	}
	return tokenType;
}

char* LSH_lexer::getCurrLexeme() {
	return pstrCurrLexeme;
}

int LSH_lexer::getCurrOp() {
	return iCurrOp;
}
