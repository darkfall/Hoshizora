#include <cstdio>
#include <cstring>
#include <cmath>

#include "llexer.h"

#define OP_EQUAL 1
#define OP_ADD	 2
#define OP_SUB	 3
#define OP_MUL	 4
#define OP_DIV	 5

std::string ws2s(const std::wstring& ws)
{
#ifdef WIN32
	return WChar2Ansi(ws);
#endif
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

std::wstring s2ws(const std::string& s)
{
#ifdef WIN32
	return Ansi2WChar(s, s.size());
#endif
    setlocale(LC_ALL, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}



std::wstring s2ws2(const std::string& s) {
	std::wstring buffer(s.length(), L' ');
	std::copy(s.begin(), s.end(), buffer.begin());
	return buffer;
}

// init operators
void initOp(llexer* lexer) {
	lexer->addOperator(L"=", 10);
	lexer->addOperator(L"==", OP_EQUAL);
	lexer->addOperator(L"+", OP_ADD);
	lexer->addOperator(L"-", OP_SUB);
	lexer->addOperator(L"*", OP_MUL);
	lexer->addOperator(L"/", OP_DIV);
}

void exitOnTokenError(Token token) {
	printf("Unexpected token: %d\n", token);
	exit(0);
}

void operationsTest(const wchar_t* file) {
	llexer* lexer = new llexer;
	
	initOp(lexer);
	
	if(lexer->loadFile(file) == LEX_NO_ERROR) {
		printf("***********************************\n");
		printf("File: %s, SourceLine: %d, SourceSize: %d\n", file, (int)lexer->getSourceLineCount(), (int)lexer->getSourceSize());
		printf("***********************************\n");
		
		Token token = lexer->getNextToken();
		
		int state = 0;
		int prevOp = -1;
		float num = 0.f;
		
		while (token != TOKEN_TYPE_END_OF_STREAM) {
			if(token != TOKEN_TYPE_INVALID_INPUT) {
				//printf("Token: %d, lexeme:%s, op:%d\n", token, lexer->getCurrLexeme(), prevOp);
				switch(state) {
					case 0:
					case 1:
						if(token != TOKEN_TYPE_INT && token != TOKEN_TYPE_FLOAT)
							exitOnTokenError(token);
						
					/*	if(state == 0) {
							if(token == TOKEN_TYPE_INT) num = atoi(lexer->getCurrLexeme());
							else if(token == TOKEN_TYPE_FLOAT) num = atof(lexer->getCurrLexeme());
						} else {
							switch (prevOp) {
								case OP_ADD:
									if(token == TOKEN_TYPE_INT) num += atoi(lexer->getCurrLexeme());
									else if(token == TOKEN_TYPE_FLOAT) num += atof(lexer->getCurrLexeme());
									break;
									
								case OP_SUB:
									if(token == TOKEN_TYPE_INT) num -= atoi(lexer->getCurrLexeme());
									else if(token == TOKEN_TYPE_FLOAT) num -= atof(lexer->getCurrLexeme());
									break;
									
								case OP_MUL:
									if(token == TOKEN_TYPE_INT) num *= atoi(lexer->getCurrLexeme());
									else if(token == TOKEN_TYPE_FLOAT) num *= atof(lexer->getCurrLexeme());
									break;
									
								case OP_DIV:
									if(token == TOKEN_TYPE_INT) num /= atoi(lexer->getCurrLexeme());
									else if(token == TOKEN_TYPE_FLOAT) num /= atof(lexer->getCurrLexeme());
									break;
							}
						}
					*/	
						state = 2;
						break;
					case 2:
						if(token != TOKEN_TYPE_OP)
							exitOnTokenError(token);
						
						prevOp = lexer->getCurrOp();
						state = 1;
						break;
				}
			}
			else {
				printf("Unexpected token: '%c'\nline=%s\n", lexer->getCurrInvalidChar(), lexer->getCurrLine());
				for(size_t i=0; i<lexer->getCurrCharIndex()+4; ++i) printf(" ");
				printf("^\n");
				
				token = lexer->toNextToken();
				continue;
			}
			token = lexer->getNextToken();
		}
		
		printf("result = %f\n", num);
	} else {
		printf("error loading file %s\n", file);
	}
	
	delete lexer;
}

void utf8IdentTest(const wchar_t* file) {
	llexer* lexer = new llexer;
	initOp(lexer);
	
	if(lexer->loadFile(file) == LEX_NO_ERROR) {
		printf("***********************************\n");
		printf("File: %s, SourceLine: %d, SourceSize: %d\n", file, (int)lexer->getSourceLineCount(), (int)lexer->getSourceSize());
		printf("***********************************\n");
		
		Token t = lexer->getNextToken();
		while(t != TOKEN_TYPE_END_OF_STREAM) {
			if(t != TOKEN_TYPE_INVALID_INPUT) {
				printf("Token: %d, lexeme: %s\n", t, lexer->getCurrLexeme());
				t = lexer->getNextToken();
				continue;
			}
			t = lexer->toNextToken();
		}
	}
	delete lexer;
}

int main (int argc, char * const argv[]) {
	printf("llexer ver%d.%d\n", LLEXER_VER_MAJOR, LLEXER_VER_MINOR);
	if(argc > 1) {
		//operationsTest(argv[1]);
		utf8IdentTest(s2ws(argv[1]).c_str());
	} else {
		printf("no file input\n");
	}
	
    return 0;
}

typedef enum {
	/* Operator Types */
	OP_TYPE_ADD	= 0,	// +
	OP_TYPE_SUB	= 1,	// -
	OP_TYPE_MUL	= 2,	// *
	OP_TYPE_DIV	= 3,	// /
	OP_TYPE_MOD	= 4,	// %
	OP_TYPE_EXP	= 5,	// ^
	
	OP_TYPE_INC	= 15,	// ++
	OP_TYPE_DEC	= 17,	// --
	
	OP_TYPE_ASSIGN_ADD	= 14,		// +=
	OP_TYPE_ASSIGN_SUB	= 16,		// -=
	OP_TYPE_ASSIGN_MUL	= 18,		// *=
	OP_TYPE_ASSIGN_DIV	= 19,		// /=
	OP_TYPE_ASSIGN_MOD	= 20,		// %=
	OP_TYPE_ASSIGN_EXP	= 21,		// ^=
	
	OP_TYPE_BITWISE_AND	= 6,		// &
	OP_TYPE_BITWISE_OR	= 7,		// |
	OP_TYPE_BITWISE_XOR	= 8,		// #
	OP_TYPE_BITWISE_NOT	= 9,		// ~
	OP_TYPE_BITWISE_SHIFT_LEFT	= 30,	// <<
	OP_TYPE_BITWISE_SHIFT_RIGHT = 32,	// >>
	
	OP_TYPE_ASSIGN_AND			= 22,		// &=
	OP_TYPE_ASSIGN_OR			= 24,		// |=
	OP_TYPE_ASSIGN_XOR			= 26,		// #=
	OP_TYPE_ASSIGN_SHIFT_LEFT	= 33,		// <<=
	OP_TYPE_ASSIGN_SHIFT_RIGHT	= 34,		// >>=
	
	OP_TYPE_LOGICAL_AND	= 23,		// &&
	OP_TYPE_LOGICAL_OR	= 25,		// ||
	OP_TYPE_LOGICAL_NOT	= 10,		// !
	
	OP_TYPE_EQUAL			= 28,		// ==
	OP_TYPE_NOT_EQUAL		= 27,		// !=
	OP_TYPE_LESS			= 12,		// <
	OP_TYPE_GREATER			= 13,		// >
	OP_TYPE_LESS_EQUAL		= 29,		// <=
	OP_TYPE_GREATER_EQUAL	= 31,		// >=
	
	OP_TYPE_POINTER	= 35,		// ->
} LLEXER_OP_TYPE;

