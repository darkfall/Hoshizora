/*
 *  lexer.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */
#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

#include "lexer.h"

#include <iostream>

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

using std::cout;
using std::endl;

LSH_lexer::LSH_lexer(vector<string>::iterator _itSourceHead, vector<string>::iterator _itSourceEnd) {
	resetLexer(_itSourceHead, _itSourceEnd);
}

LSH_lexer::LSH_lexer() {
}

LSH_lexer::~LSH_lexer() {
	return;
}

void LSH_lexer::resetLexer(vector<string>::iterator _itSourceHead, vector<string>::iterator _itSourceEnd) {
	lsCurrLexerState.iCurrLexemeEnd = lsCurrLexerState.iCurrLexemeStart = 0;
	lsCurrLexerState.iCurrLineIndex = 0;
	lsCurrLexerState.iCurrOp = 0;
	lsCurrLexerState.itCurrLine = _itSourceHead;
	itSourceEnd = _itSourceEnd;
}

void LSH_lexer::rewindTokenStream() {
	copyLexerState(lsCurrLexerState, lsPrevLexerState);
}

void LSH_lexer::copyCurrLexeme(char* pstrBuffer) {
	strcpy(pstrBuffer, lsCurrLexerState.pstrCurrLexeme);
}

char LSH_lexer::getLookAheadChar() {
	LSH_LexerState prevLexerState;
	copyLexerState(prevLexerState, lsCurrLexerState);
	
	char cCurrChar;
	while (TRUE) {
		cCurrChar = getNextChar();
		if(!isCharWhitespaceLex(cCurrChar))
			break;
	}
	
	copyLexerState(lsCurrLexerState, prevLexerState);
	return cCurrChar;
}

char LSH_lexer::getNextChar() {
	if(lsCurrLexerState.itCurrLine == itSourceEnd)
		return '\0';
	
	if(lsCurrLexerState.iCurrLexemeEnd >= lsCurrLexerState.itCurrLine->size()) {
		++lsCurrLexerState.itCurrLine;
		if(lsCurrLexerState.itCurrLine == itSourceEnd)
			return '\0';
			
		++lsCurrLexerState.iCurrLineIndex;
		lsCurrLexerState.iCurrLexemeStart = 0;
		lsCurrLexerState.iCurrLexemeEnd = 0;
	}
	   
	return (*lsCurrLexerState.itCurrLine)[lsCurrLexerState.iCurrLexemeEnd++];
}

void LSH_lexer::copyLexerState(LSH_LexerState& dest, const LSH_LexerState& source) {
	dest.iCurrLineIndex		= source.iCurrLineIndex;
	dest.itCurrLine			= source.itCurrLine;
	dest.tCurrToken			= source.tCurrToken;
	strcpy(dest.pstrCurrLexeme, source.pstrCurrLexeme);
	dest.iCurrLexemeStart	= source.iCurrLexemeStart;
	dest.iCurrLexemeEnd		= source.iCurrLexemeEnd;
	dest.iCurrOp			= source.iCurrOp;
}

Token LSH_lexer::getNextToken() {
	copyLexerState(lsPrevLexerState, lsCurrLexerState);
	
	lsCurrLexerState.iCurrLexemeStart = lsCurrLexerState.iCurrLexemeEnd;
	
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
			case LEX_STATE_UNKNOWN:
				iLexemeDone = TRUE;
				break;
				
			case LEX_STATE_START:
				if(isCharWhitespaceLex(cCurrChar)) {
					++lsCurrLexerState.iCurrLexemeStart;
					iAddCurrChar = FALSE;
				} else if(isCharNumericLex(cCurrChar)) {
					iCurrState = LEX_STATE_INT;
				} else if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else if(isCharDelimiterLex(cCurrChar)) {
					iCurrState = LEX_STATE_DELIM;
				} else if(isCharOpChar(cCurrChar, 0)) {
					iCurrOpStateIndex = getOpStateIndex(cCurrChar, 0, 0, 0);
					if(iCurrOpStateIndex == -1)
						return TOKEN_TYPE_INVALID;
					
					opCurrOpState = getOpState(0, iCurrOpStateIndex);
					iCurrOpCharIndex = 1;
					
					lsCurrLexerState.iCurrOp = opCurrOpState.iIndex;
					
					iCurrState = LEX_STATE_OP;
				} else if(cCurrChar == '"') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_STRING;
				}
				else
					Lex_ExitOnInvalidInputError(cCurrChar, lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
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
					Lex_ExitOnInvalidInputError(cCurrChar, lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
				break;
				
			case LEX_STATE_FLOAT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_FLOAT;
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || isCharOpChar(cCurrChar, 0)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else
					Lex_ExitOnInvalidInputError(cCurrChar, lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
				break;
				
			case LEX_STATE_IDENT:
				if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || isCharOpChar(cCurrChar, 0)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else
					Lex_ExitOnInvalidInputError(cCurrChar, lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
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
						Lex_ExitOnInvalidInputError(cCurrChar, lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
					
					opCurrOpState = getOpState(iCurrOpCharIndex, iCurrOpStateIndex);
					
					++iCurrOpCharIndex;
					lsCurrLexerState.iCurrOp = opCurrOpState.iIndex;
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
				} else if(cCurrChar == '\n') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_UNKNOWN;
				} else if(cCurrChar == '\\') {
					iAddCurrChar = TRUE;
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
			lsCurrLexerState.pstrCurrLexeme[iNextLexemeCharIndex] = cCurrChar;
			++iNextLexemeCharIndex;
		}
		
		if(iLexemeDone)
			break;
	}
		
	lsCurrLexerState.pstrCurrLexeme[iNextLexemeCharIndex] = '\0';
	--lsCurrLexerState.iCurrLexemeEnd;

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
			if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "var") == 0)
				tokenType = TOKEN_TYPE_RSRVD_VAR;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "true") == 0)
				tokenType = TOKEN_TYPE_RSRVD_TRUE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "false") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FALSE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "if") == 0)
				tokenType = TOKEN_TYPE_RSRVD_IF;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "else") == 0)
				tokenType = TOKEN_TYPE_RSRVD_ELSE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "break") == 0)
				tokenType = TOKEN_TYPE_RSRVD_BREAK;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "continue") == 0)
				tokenType = TOKEN_TYPE_RSRVD_CONTINUE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "for") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FOR;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "while") == 0)
				tokenType = TOKEN_TYPE_RSRVD_WHILE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "func") == 0)
				tokenType = TOKEN_TYPE_RSRVD_FUNC;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "return") == 0)
				tokenType = TOKEN_TYPE_RSRVD_RETURN;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "host") == 0)
				tokenType = TOKEN_TYPE_RSRVD_HOST;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "print") == 0)
				tokenType = TOKEN_TYPE_RSRVD_PRINT;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "define") == 0)
				tokenType = TOKEN_TYPE_RSRVD_DEFINE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "include") == 0)
				tokenType = TOKEN_TYPE_RSRVD_INCLUDE;
			else if(strcasecmp(lsCurrLexerState.pstrCurrLexeme, "struct") == 0)
				tokenType = TOKEN_TYPE_RSRVD_STRUCT;
			break;
				
		case LEX_STATE_DELIM:
			switch (lsCurrLexerState.pstrCurrLexeme[0]) {
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
					
				case '#':
					if(lsCurrLexerState.iCurrLexemeStart != 0)
						Lex_ExitOnInvalidInputError('#', lsCurrLexerState.iCurrLineIndex, *lsCurrLexerState.itCurrLine);
					tokenType = TOKEN_TYPE_DELIM_POUND;
					break;
					
				case '.':
					tokenType = TOKEN_TYPE_DELIM_POINT;
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
	lsCurrLexerState.tCurrToken = tokenType;
	return tokenType;
}