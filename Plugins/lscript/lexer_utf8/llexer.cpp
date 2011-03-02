/*
 *  lexer.cpp
 *  lScript_lexer
 *
 *  Created by GriffinBu on 10年1月7日.
 *  Copyright 2010 RobertBu(Project L). All rights reserved.
 *
 */

#include "llexer.h"

#define _DEBUG

#define LEX_STATE_START					0
#define LEX_STATE_INT					1
#define LEX_STATE_FLOAT					2
#define LEX_STATE_IDENT					5

#define LEX_STATE_OP					6
#define LEX_STATE_DELIM					7

#define LEX_STATE_STRING				8
#define LEX_STATE_STRING_ESCAPE			9
#define LEX_STATE_STRING_CLOSE_QUOTE	10

//determines if a character is numeric
inline bool isCharNumericLex(wchar_t cChar) {
	if(cChar >= L'0' && 
	   cChar <= L'9')
		return true;
	return false;
}

inline bool isCharNumericSpecial(wchar_t cChar) {
	if(// hex
	   cChar == L'x')
		return true;
	return false;
}

//determines if a character is whitespace
inline bool isCharWhitespaceLex(wchar_t cChar) {
	if(cChar == L' ' || 
	   cChar == L'\t' || 
	   cChar == L'\n' ||
	   cChar == L'\r')
		return true;
	return false;
}

//determines if a character is ASCII character
inline bool isCharASCIIChar(char cChar) {
	// ascii head 0
	return ((cChar&0x80) == 0);
}

//determines if a charatcter could be part of a valid identifier
inline bool isCharIdentLex(wchar_t cChar) {
	if((cChar >= L'0' && cChar <= L'9') ||
	   (cChar >= L'A' && cChar <= L'Z') ||
	   (cChar >= L'a' && cChar <= L'z') ||
	   (cChar == L'_') ||
	   (!isCharASCIIChar(cChar)))
		return true;
	return false;
}

//determines if a character is a delimiter
inline bool isCharDelimiterLex(wchar_t cChar) {
	static std::wstring m_delimiters = L",(){};[].:";
	return (m_delimiters.find(cChar) != std::wstring::npos);
}

inline std::string ws2s(const std::wstring& ws) {
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_CTYPE, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_CTYPE, curLocale.c_str());
    return result;
}

std::string ws2s2(const std::wstring& ws) {
	std::string buffer(ws.length(), ' ');
	std::copy(ws.begin(), ws.end(), buffer.begin());
	return buffer;
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
	currState.wstrCurrLexeme.clear();
	currState.strCurrLexeme.clear();
	
    usErrorCode = LEX_NO_ERROR;
	strInvalidChar = 0;
	
	currToken = TOKEN_TYPE_END_OF_STREAM;
	
	contSource.clear();
	contLexerState.clear();
}

int llexer::loadFile(const wchar_t* pstrFileName) {
	FILE* pSourceFile;
	if(!(pSourceFile = fopen(ws2s(pstrFileName).c_str(), "rb"))) {
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

#include <iostream>

int llexer::loadFileMem(void* ptr, unsigned long size) {
	if(ptr && size != 0) {
		const char* pstrSource = (const char*)ptr;
		unsigned long c = 0;
		
		char cCurrChar;
		std::wstring sCurrLine;
		//size = wcslen(pstrSource);
		while(c < size) {
			cCurrChar = *pstrSource++;

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
		currState.wstrCurrLexeme.clear();
		currState.strCurrLexeme.clear();
		
		usErrorCode = LEX_NO_ERROR;
		strInvalidChar = 0;
		
		return LEX_NO_ERROR;
	}
	usErrorCode = LEX_ERROR_FILEIO;
	return LEX_ERROR_FILEIO;
}

Token llexer::errorInvalidChar(wchar_t cCurrChar) {
	strInvalidChar = cCurrChar;
	usErrorCode = LEX_INVALID_INPUT;
	return TOKEN_TYPE_INVALID_INPUT;
}

wchar_t llexer::getNextChar() {
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

wchar_t llexer::getLookAheadChar() {
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
		wchar_t cCurrChar = getNextChar();
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

Token llexer::getCurrToken() const {
	return currToken;
}

Token llexer::getNextToken() {
	if(usErrorCode != LEX_NO_ERROR) return TOKEN_TYPE_INVALID_INPUT;
	
	if(currState.iCurrLine >= contSource.size()) 
		return TOKEN_TYPE_END_OF_STREAM;
	
	currState.tkTokenSubType = TOKEN_SUBTYPE_NULL;
	currState.strCurrLexeme.clear();
	currState.wstrCurrLexeme.clear();
	
	if(currState.iCurrLine != -1)
		contLexerState.push_back(currState);
	
	int iCurrState = LEX_STATE_START;
	
	int iCurrOpStateIndex = -1;
	
	wchar_t cCurrChar;
	bool iAddCurrChar;
	bool iLexemeDone = false;
	
	while(true) {
		cCurrChar = getNextChar();
		if(cCurrChar == L'\0')
			break;
		
		iAddCurrChar = true;
		
		switch (iCurrState) {
			case LEX_STATE_START:
				if(isCharWhitespaceLex(cCurrChar)) {
					iAddCurrChar = false;
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
					iAddCurrChar = false;
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
					iAddCurrChar = false;
					iLexemeDone = true;
				}
				else {
					return errorInvalidChar(cCurrChar);
				}				
				break;
				
			case LEX_STATE_FLOAT:
				if(isCharNumericLex(cCurrChar))
					iCurrState = LEX_STATE_FLOAT;
				else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
					iAddCurrChar = false;
					iLexemeDone = true;
				} else if(cCurrChar == 'f') {
					iAddCurrChar = false;
				}
				else {
					return errorInvalidChar(cCurrChar);
				}				
				break;
				
			case LEX_STATE_IDENT:
				if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
					iAddCurrChar = false;
					iLexemeDone = true;
				} else if(isCharIdentLex(cCurrChar)) {
					iCurrState = LEX_STATE_IDENT;
				} else {
					return errorInvalidChar(cCurrChar);
				}
 				break;
				
			case LEX_STATE_OP:
				if((iCurrOpStateIndex = operators.check((currState.wstrCurrLexeme+cCurrChar))) != -1) {
					currState.iCurrOp = iCurrOpStateIndex;
				} else {
					currState.iCurrOp = operators.check(currState.wstrCurrLexeme);
					if(currState.iCurrOp == -1)
						return errorInvalidChar(cCurrChar);

					iAddCurrChar = false;
					iLexemeDone = true;
				}
				break;
				
			case LEX_STATE_DELIM:
				iAddCurrChar = false;
				iLexemeDone = true;
				break;
				
			case LEX_STATE_STRING:
				if(cCurrChar == '"') {
					iAddCurrChar = false;
					iCurrState = LEX_STATE_STRING_CLOSE_QUOTE;
				} else if(cCurrChar == '\\') {
					iAddCurrChar = true;
					iCurrState = LEX_STATE_STRING_ESCAPE;
				}
				break;
				
			case LEX_STATE_STRING_ESCAPE:
				iCurrState = LEX_STATE_STRING;
				break;
				
			case LEX_STATE_STRING_CLOSE_QUOTE:
				iAddCurrChar = false;
				iLexemeDone = true;
				break;
		}
		if(iAddCurrChar) {
			currState.wstrCurrLexeme += cCurrChar;
			currState.strCurrLexeme = ws2s2(currState.wstrCurrLexeme);
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
			switch (currState.wstrCurrLexeme[0]) {
				case L',': tokenType = TOKEN_TYPE_DELIM_COMMA; break;
				case L'(': tokenType = TOKEN_TYPE_DELIM_OPEN_PAREN; break;
				case L')': tokenType = TOKEN_TYPE_DELIM_CLOSE_PAREN; break;
				case L'[': tokenType = TOKEN_TYPE_DELIM_OPEN_BRACE; break;
				case L']': tokenType = TOKEN_TYPE_DELIM_CLOSE_BRACE; break;
				case L'{': tokenType = TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE; break;
				case L'}': tokenType = TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE; break;
				case L';': tokenType = TOKEN_TYPE_DELIM_SEMICOLON; break;
				case L'.': tokenType = TOKEN_TYPE_DELIM_PERIOD; break;
				case L':': tokenType = TOKEN_TYPE_DELIM_COLON; break;
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
		
	currToken = tokenType;
	return tokenType;
}

bool llexer::rewindTokenStream() {
	if(contLexerState.size() > 0) {
		currState = contLexerState.back();
		contLexerState.pop_back();
		return true;
	}
	return false;
}

const wchar_t* llexer::getCurrLexemeW() const {
	return currState.wstrCurrLexeme.c_str();
}

const char* llexer::getCurrLexeme() const {
	return currState.strCurrLexeme.c_str();
}

int llexer::getCurrOp() const {
	return currState.iCurrOp;
}

wchar_t llexer::getCurrInvalidChar() const {
	return strInvalidChar;
}

Token llexer::getCurrTokenSubtype() const {
	return currState.tkTokenSubType;
}

size_t llexer::getCurrCharIndex() const {
	return currState.iCurrLexemeIndex;
}

size_t llexer::getCurrLineIndex() const {
	return currState.iCurrLine;
}

size_t llexer::getSourceLineCount() const {
	return contSource.size();
}

size_t llexer::getSourceSize() const {
	return lSourceSize;
}

const wchar_t*	llexer::getCurrLine() const {
	if(currState.iCurrLine < contSource.size())
		return contSource[currState.iCurrLine].c_str();
	return 0;
}

void llexer::addOperator(const std::wstring& op, int ident) {
	operators.add(op, ident);
}