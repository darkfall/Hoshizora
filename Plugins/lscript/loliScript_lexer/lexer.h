/*
 *  lexer.h
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */
#ifndef _LEXER_H_
#define _LEXER_H_

#include "lex_defines.h"
#include "lex_errors.h"
#include "lex_stringProcess.h"

#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>

#include "lex_operators.h"

using std::string;

const size_t MAX_LEXEME_SIZE = 1024;

#ifndef TOKEN
typedef int Token;
#endif

#define LEX_STATE_START					0
#define LEX_STATE_INT					1
#define LEX_STATE_FLOAT					2
#define LEX_STATE_IDENT					5

#define LEX_STATE_OP					6
#define LEX_STATE_DELIM					7

#define LEX_STATE_STRING				8
#define LEX_STATE_STRING_ESCAPE			9
#define LEX_STATE_STRING_CLOSE_QUOTE	10


#define TOKEN_TYPE_END_OF_STREAM			0

#define TOKEN_TYPE_INT						1
#define TOKEN_TYPE_FLOAT					2
		
#define TOKEN_TYPE_IDENT					3

#define TOKEN_TYPE_RSRVD_VAR				4
#define TOKEN_TYPE_RSRVD_TRUE				5
#define TOKEN_TYPE_RSRVD_FALSE				6
#define TOKEN_TYPE_RSRVD_IF					7
#define TOKEN_TYPE_RSRVD_ELSE				8
#define TOKEN_TYPE_RSRVD_BREAK				9
#define TOKEN_TYPE_RSRVD_CONTINUE			10
#define TOKEN_TYPE_RSRVD_FOR				11
#define TOKEN_TYPE_RSRVD_WHILE				12
#define TOKEN_TYPE_RSRVD_FUNC				13
#define TOKEN_TYPE_RSRVD_RETURN				14

#define TOKEN_TYPE_OP						15

#define TOKEN_TYPE_DELIM_COMMA				16	// ,
#define TOKEN_TYPE_DELIM_OPEN_PAREN			17	// (
#define TOKEN_TYPE_DELIM_CLOSE_PAREN		18	// )
#define TOKEN_TYPE_DELIM_OPEN_BRACE			19	// [
#define TOKEN_TYPE_DELIM_CLOSE_BRACE		20	// ]
#define TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE	21	// {
#define TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  22	// }
#define TOKEN_TYPE_DELIM_SEMICOLON			23	// ;

#define TOKEN_TYPE_STRING					24

class LSH_lexer {
public:
	LSH_lexer();
	~LSH_lexer();
	
	void initLexer	();
	
	int loadFile	(const char*);
	int lexFile		();
	
	
	Token	getNextToken();
	
	char*   getCurrLexeme();
	int		getCurrOp();
	
private:
	inline char	getNextChar();
		
	long    lSourceSize;
	char*	pstrSource;
	char	pstrCurrLexeme[MAX_LEXEME_SIZE];
	
	int		iCurrLexemeStart;
	int		iCurrLexemeEnd;
	int		iCurrOp;
};


#endif