/*
 *  lexer.cpp
 *  lScript_lexer
 *
 *  Created by GriffinBu on 10年1月7日.
 *  Copyright 2010 RobertBu(Project L). All rights reserved.
 *
 */

#include "llexer.h"

#define LEX_STATE_START					0
#define LEX_STATE_INT					1
#define LEX_STATE_FLOAT					2
#define LEX_STATE_IDENT					5

#define LEX_STATE_OP					6
#define LEX_STATE_DELIM					7

#define LEX_STATE_STRING				8
#define LEX_STATE_STRING_ESCAPE			9
#define LEX_STATE_STRING_CLOSE_QUOTE	10

#if !defined(__APPLE__)
#include <malloc.h>
#endif

//determines if a character is numeric
inline int isCharNumericLex(char cChar) {
	if(cChar >= '0' &&
	   cChar <= '9')
		return TRUE;
	return FALSE;
}

inline int isCharNumericSpecial(char cChar) {
	if(
	   // hex
	   cChar == 'x'
	   )
		return TRUE;
	return FALSE;
}

//determines if a character is whitespace
inline int isCharWhitespaceLex(char cChar) {
	if(cChar == ' ' ||
	   cChar == '\t' ||
	   cChar == '\n' ||
	   cChar == '\r')
		return TRUE;
	return FALSE;
}

//determines if a character is ASCII character
inline int isCharASCIIChar(char cChar) {
	// ascii head 0
	return ((cChar&0x80) == 0);
}

//determines if a charatcter could be part of a valid identifier
inline int isCharIdentLex(char cChar) {
	if((cChar >= '0' && cChar <= '9') ||
	   (cChar >= 'A' && cChar <= 'Z') ||
	   (cChar >= 'a' && cChar <= 'z') ||
	   (cChar == '_') ||
	   (!isCharASCIIChar(cChar)))
		return TRUE;
	return FALSE;
}

//determines if a character is a delimiter
inline int isCharDelimiterLex(char cChar) {
	if(cChar == ',' ||
	   cChar == '(' ||
	   cChar == ')' ||
	   cChar == '[' ||
	   cChar == ']' ||
	   cChar == ';' ||
	   cChar == '{' ||
	   cChar == '}' ||
	   cChar == '.' ||
	   cChar == ':')
		return TRUE;
	return FALSE;
}


llexer::llexer() {
    reset();
}

llexer::~llexer() {

}

void llexer::reset() {
    // reset lexer
    currState.iCurrLexemeIndex = currState.iCurrOp = currState.iCurrLine = 0;
	currState.tkTokenSubType = TOKEN_SUBTYPE_NULL;
	currState.strCurrLexeme.clear();

    usErrorCode = LEX_NO_ERROR;
	strInvalidChar = 0;

	contSource.clear();
	contLexerState.clear();
}

int llexer::loadFile(const char* pstrFileName) {
	FILE* pSourceFile;
	if(!(pSourceFile = fopen(pstrFileName, "rb"))) {
		usErrorCode = LEX_ERROR_FILEIO;
		return LEX_ERROR_FILEIO;
	}

	fseek(pSourceFile, 0, SEEK_END);
	lSourceSize = ftell(pSourceFile);
	fseek(pSourceFile, 0, SEEK_SET);

	void* pstrSource = malloc(lSourceSize+1);
	fread(pstrSource, lSourceSize, 1, pSourceFile);
	fclose(pSourceFile);

	if(!pstrSource) {
		usErrorCode = LEX_ERROR_FILEIO;
		return LEX_ERROR_FILEIO;
	}

	return loadFileMem(pstrSource, lSourceSize);
}

int llexer::loadFileMem(void* ptr, unsigned long size) {
	if(ptr && size != 0) {
		const char* pstrSource = (const char*)ptr;
		unsigned long c = 0;

		char cCurrChar;
		std::string sCurrLine;
		while(c < size) {
			cCurrChar = pstrSource[c];

			// ASCII character
			if(isCharASCIIChar(cCurrChar)) {
				if(cCurrChar == '\\')
					cCurrChar = '/';

				//*nix LF, windows LF+CR
				//ignore CR
				if(cCurrChar == '\r') {
					++c;
					continue;
				}
				else if(cCurrChar == '\n') {
					contSource.push_back(sCurrLine);
					sCurrLine.clear();
				} else {
					sCurrLine += cCurrChar;
				}
				++c;
			} else {
				// just add to source
				sCurrLine += cCurrChar;
				++c;
			}
		}
		contSource.push_back(sCurrLine);

		currState.iCurrLexemeIndex = currState.iCurrOp = currState.iCurrLine = 0;
		currState.tkTokenSubType = TOKEN_SUBTYPE_NULL;
		currState.strCurrLexeme.clear();

		usErrorCode = LEX_NO_ERROR;
		strInvalidChar = 0;

		return LEX_NO_ERROR;
	}
	usErrorCode = LEX_ERROR_FILEIO;
	return LEX_ERROR_FILEIO;
}

Token llexer::errorInvalidChar(char cCurrChar) {
	strInvalidChar = cCurrChar;
	usErrorCode = LEX_INVALID_INPUT;
	return TOKEN_TYPE_INVALID_INPUT;
}

char llexer::getNextChar() {
	if(currState.iCurrLexemeIndex < contSource[currState.iCurrLine].size()) {
		return contSource[currState.iCurrLine][currState.iCurrLexemeIndex++];
	} else {
		//	printf("line %d/%d, char %d/%d, %s :", iCurrLine, contSource.size(), iCurrLexemeIndex, contSource[iCurrLine].size(), contSource[iCurrLine].c_str());
		currState.iCurrLexemeIndex = 0;
		++currState.iCurrLine;
		return '\n';
	}
}

void llexer::rewindChar() {
	if(currState.iCurrLexemeIndex > 0) {
		--currState.iCurrLexemeIndex;
	}
}

char llexer::getLookAheadChar() {
	if(currState.iCurrLexemeIndex < contSource[currState.iCurrLine].size()-1) {
		return contSource[currState.iCurrLine][currState.iCurrLexemeIndex+1];
	} else {
		return 0;
	}
}

Token llexer::toNextToken() {
	if(usErrorCode == LEX_NO_ERROR) return getNextToken();
	else if(usErrorCode == LEX_INVALID_INPUT) {
		rewindChar();
		char cCurrChar = getNextChar();
		while(!(isCharWhitespaceLex(cCurrChar) ||
				isCharDelimiterLex(cCurrChar) ||
				isCharIdentLex(cCurrChar) ||
				operators.checkChar(cCurrChar))) {
			cCurrChar = getNextChar();
		}
		usErrorCode = LEX_NO_ERROR;
		rewindChar();
		return getNextToken();
	}
	return TOKEN_TYPE_END_OF_STREAM;
}

Token llexer::getNextToken() {
	if(usErrorCode != LEX_NO_ERROR) return TOKEN_TYPE_INVALID_INPUT;

	if(currState.iCurrLine >= contSource.size())
		return TOKEN_TYPE_END_OF_STREAM;

	currState.tkTokenSubType = TOKEN_SUBTYPE_NULL;
	currState.strCurrLexeme.clear();

	if(currState.iCurrLine != -1)
		contLexerState.push_back(currState);

	int iCurrState = LEX_STATE_START;
	int iLexemeDone = FALSE;

	int iCurrOpStateIndex = -1;

	char cCurrChar;
	int iAddCurrChar;

	while(TRUE) {
		if(currState.iCurrLine >= contSource.size())
			return TOKEN_TYPE_END_OF_STREAM;
		
		cCurrChar = getNextChar();
		if(cCurrChar == '\0')
			break;
		if(cCurrChar == '\n')
			continue;

		iAddCurrChar = TRUE;

		switch (iCurrState) {
			case LEX_STATE_START:
				if(isCharWhitespaceLex(cCurrChar)) {
					iAddCurrChar = FALSE;
				} else if(isCharNumericLex(cCurrChar)) {
					// octal number begins with '0'
					if(cCurrChar == '0')
						currState.tkTokenSubType = TOKEN_SUBTYPE_INT_OCTAL;

					iCurrState = LEX_STATE_INT;
				} else if(cCurrChar == '.') {
					if(isCharNumericLex( getLookAheadChar() ))
						iCurrState = LEX_STATE_FLOAT;
					else
						iCurrState = LEX_STATE_DELIM;
				} else if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else if(operators.checkChar(cCurrChar)) {
					currState.iCurrOp = iCurrOpStateIndex;

					iCurrState = LEX_STATE_OP;
				} else if(isCharDelimiterLex(cCurrChar)) {
					iCurrState = LEX_STATE_DELIM;
				} else if(cCurrChar == '"') {
					iAddCurrChar = FALSE;
					iCurrState = LEX_STATE_STRING;
				}
				else {
					return errorInvalidChar(cCurrChar);
				}
				break;

			case LEX_STATE_INT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_INT;
				else if(cCurrChar == '.') {
					// no special for float
					if(currState.tkTokenSubType != TOKEN_SUBTYPE_NULL) {
						return errorInvalidChar(cCurrChar);
					}
					iCurrState = LEX_STATE_FLOAT;
				}
				else if(isCharNumericSpecial(cCurrChar)) {
					// determine which kind of number
					switch (cCurrChar) {
						case 'x': currState.tkTokenSubType = TOKEN_SUBTYPE_INT_HEX; break;
					}
					iCurrState = LEX_STATE_INT;
				}
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				}
				else {
					return errorInvalidChar(cCurrChar);
				}
				break;

			case LEX_STATE_FLOAT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_FLOAT;
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else if(cCurrChar == 'f') {
					iAddCurrChar = FALSE;
				}
				else {
					return errorInvalidChar(cCurrChar);
				}
				break;

			case LEX_STATE_IDENT:
				if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
					iAddCurrChar = FALSE;
					iLexemeDone = TRUE;
				} else if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else {
					return errorInvalidChar(cCurrChar);
				}
 				break;

			case LEX_STATE_OP:
				if((iCurrOpStateIndex = operators.check((currState.strCurrLexeme+cCurrChar))) != -1) {
					currState.iCurrOp = iCurrOpStateIndex;
				} else {
					currState.iCurrOp = operators.check(currState.strCurrLexeme);
					if(currState.iCurrOp == -1)
						return errorInvalidChar(cCurrChar);

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
		}
		if(iAddCurrChar) {
			currState.strCurrLexeme += cCurrChar;
		}

		if(iLexemeDone)
			break;
	}

	if(currState.iCurrLexemeIndex > 0)
		--currState.iCurrLexemeIndex;

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
			break;

		case LEX_STATE_DELIM:
			switch (currState.strCurrLexeme[0]) {
				case ',': tokenType = TOKEN_TYPE_DELIM_COMMA; break;
				case '(': tokenType = TOKEN_TYPE_DELIM_OPEN_PAREN; break;
				case ')': tokenType = TOKEN_TYPE_DELIM_CLOSE_PAREN; break;
				case '[': tokenType = TOKEN_TYPE_DELIM_OPEN_BRACE; break;
				case ']': tokenType = TOKEN_TYPE_DELIM_CLOSE_BRACE; break;
				case '{': tokenType = TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE; break;
				case '}': tokenType = TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE; break;
				case ';': tokenType = TOKEN_TYPE_DELIM_SEMICOLON; break;
				case '.': tokenType = TOKEN_TYPE_DELIM_PERIOD; break;
				case ':': tokenType = TOKEN_TYPE_DELIM_COLON; break;
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

int llexer::rewindTokenStream() {
	if(contLexerState.size() > 0) {
		currState = contLexerState.back();
		contLexerState.pop_back();
		return TRUE;
	}
	return FALSE;
}

const char* llexer::getCurrLexeme() {
	return currState.strCurrLexeme.c_str();
}

int llexer::getCurrOp() {
	return currState.iCurrOp;
}

char llexer::getCurrInvalidChar() {
	return strInvalidChar;
}

Token llexer::getCurrTokenSubtype() {
	return currState.tkTokenSubType;
}

size_t llexer::getCurrCharIndex() {
	return currState.iCurrLexemeIndex;
}

size_t llexer::getCurrLineIndex() {
	return currState.iCurrLine;
}

size_t llexer::getSourceLineCount() {
	return contSource.size();
}

size_t llexer::getSourceSize() {
	return lSourceSize;
}

const char*	llexer::getCurrLine() {
	if(currState.iCurrLine < contSource.size())
		return contSource[currState.iCurrLine].c_str();
	return 0;
}

void llexer::addOperator(const std::string& op, int ident) {
	operators.add(op, ident);
}
