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
#include <vector>

#include "lex_operators.h"

using std::string;
using std::vector;

const size_t LEXER_MAX_LEXEME_SIZE = 1024;

#ifndef TOKEN
typedef int Token;
#endif

#define LEX_STATE_UNKNOWN				-1

#define LEX_STATE_START					0
#define LEX_STATE_INT					1
#define LEX_STATE_FLOAT					2
#define LEX_STATE_IDENT					5

#define LEX_STATE_OP					6
#define LEX_STATE_DELIM					7

#define LEX_STATE_STRING				8
#define LEX_STATE_STRING_ESCAPE			9
#define LEX_STATE_STRING_CLOSE_QUOTE	10


#define TOKEN_TYPE_END_OF_STREAM			-1

#define TOKEN_TYPE_INVALID					0

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
#define TOKEN_TYPE_RSRVD_HOST				15

#define TOKEN_TYPE_RSRVD_STRUCT				31

#define TOKEN_TYPE_RSRVD_DEFINE				28
#define TOKEN_TYPE_RSRVD_INCLUDE			29

#define TOKEN_TYPE_OP						16

#define TOKEN_TYPE_DELIM_COMMA				17	// ,
#define TOKEN_TYPE_DELIM_OPEN_PAREN			18	// (
#define TOKEN_TYPE_DELIM_CLOSE_PAREN		19	// )
#define TOKEN_TYPE_DELIM_OPEN_BRACE			20	// [
#define TOKEN_TYPE_DELIM_CLOSE_BRACE		21	// ]
#define TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE	22	// {
#define TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  23	// }
#define TOKEN_TYPE_DELIM_SEMICOLON			24	// ;
#define TOKEN_TYPE_DELIM_POUND				25  // #

#define TOKEN_TYPE_DELIM_POINT				30  // .

#define TOKEN_TYPE_STRING					26

#define TOKEN_TYPE_RSRVD_PRINT				27

typedef struct _LSH_LexerState {
	int iCurrLineIndex;
	
	vector<string>::iterator	itCurrLine;
	
	Token	tCurrToken;
	char	pstrCurrLexeme[LEXER_MAX_LEXEME_SIZE];
	int		iCurrLexemeStart;
	int		iCurrLexemeEnd;
	int		iCurrOp;
	
	_LSH_LexerState():tCurrToken(-1), iCurrLexemeEnd(0), iCurrLexemeStart(0), iCurrOp(-1), iCurrLineIndex(-1) {}

} LSH_LexerState;

class LSH_lexer {
public:
	friend class LSH_Parser;
	
	LSH_lexer(vector<string>::iterator _itSourceHead, vector<string>::iterator _itSourceEnd);
	LSH_lexer();
	~LSH_lexer();
	
	void	resetLexer(vector<string>::iterator _itSourceHead, vector<string>::iterator _itSourceEnd);
	
	Token	getNextToken();
	void	rewindTokenStream();
	
	Token	getCurrToken() const				{return lsCurrLexerState.tCurrToken; }
	char*   getCurrLexeme()						{return lsCurrLexerState.pstrCurrLexeme; }
	void	copyCurrLexeme(char* pstrBuffer);
	int		getCurrOp() const					{return lsCurrLexerState.iCurrOp; }
	
	string	getCurrSourceLine()					{return *lsCurrLexerState.itCurrLine; }
	int		getCurrSourceLineIndex() const		{return lsCurrLexerState.iCurrLineIndex; }
	int		getLexemeStartIndex() const			{return lsCurrLexerState.iCurrLexemeStart; }
	
private:
	char	getNextChar();
	char	getLookAheadChar();
	
	void copyLexerState(LSH_LexerState& ls1, const LSH_LexerState& ls2);
		
	vector<string>::iterator itSourceEnd;
	
	LSH_LexerState lsCurrLexerState;
	LSH_LexerState lsPrevLexerState;
};


#endif