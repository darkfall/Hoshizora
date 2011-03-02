#include <cstdio>
#include <cstring>
#include <cmath>

#include "lexer.h"

#define OP_EQUAL 1
#define OP_ADD	 2
#define OP_SUB	 3
#define OP_MUL	 4
#define OP_DIV	 5

// init operators
void initOp(llexer* lexer) {
	lexer->addOperator("==", OP_EQUAL);
	lexer->addOperator("+", OP_ADD);
	lexer->addOperator("-", OP_SUB);
	lexer->addOperator("*", OP_MUL);
	lexer->addOperator("/", OP_DIV);
}

void exitOnTokenError(Token token) {
	printf("Unexpected token: %d\n", token);
	exit(0);
}

void operationsTest(const char* file) {
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
						
						if(state == 0) {
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

void utf8IdentTest(const char* file) {
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
		utf8IdentTest(argv[1]);
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

