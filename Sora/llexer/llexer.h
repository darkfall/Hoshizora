/*
 *  lexer.h
 *  lScript_lexer
 *
 *  Created by GriffinBu on 10年1月7日.
 *  Copyright 2010 RobertBu(Project L). All rights reserved.
 *
 */
#ifndef _LLEXER_H_
#define _LLEXER_H_

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>

#define	LLEXER_VER_MAJOR  1
#define	LLEXER_VER_MINOR  3

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef WIN32
#define stricmp strcasecmp
#endif

typedef int Token;

/* Token Types */
typedef enum {
	TOKEN_TYPE_INVALID_INPUT = -1, 	// invalid input
	TOKEN_TYPE_END_OF_STREAM = 0,	// end of a file
	TOKEN_TYPE_INT			 = 1,	// int number
	TOKEN_TYPE_FLOAT		 = 2,	// float number
	TOKEN_TYPE_IDENT		 = 3,	// a ident
	TOKEN_TYPE_STRING		 = 4,	// a string
	TOKEN_TYPE_OP			 = 15,	// operators, see lex_operators
	
	// delimeters
	TOKEN_TYPE_DELIM_COMMA				= 16,	// ,
	TOKEN_TYPE_DELIM_OPEN_PAREN			= 17,	// (
	TOKEN_TYPE_DELIM_CLOSE_PAREN		= 18,	// )
	TOKEN_TYPE_DELIM_OPEN_BRACE			= 19,	// [
	TOKEN_TYPE_DELIM_CLOSE_BRACE		= 20,	// ]
	TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE	= 21,	// {
	TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE	= 22,	// }
	TOKEN_TYPE_DELIM_SEMICOLON			= 23,	// ;
	TOKEN_TYPE_DELIM_PERIOD				= 24,	// .
	TOKEN_TYPE_DELIM_COLON				= 25,	// :
} LLEXER_TOKEN_TYPE;

// token subtypes
// int token special, hex & binary & octal
typedef enum {
	TOKEN_SUBTYPE_NULL			= 100,
	TOKEN_SUBTYPE_INT_HEX		= 101,
	TOKEN_SUBTYPE_INT_OCTAL		= 102,
} LLEXER_TOKEN_SUBTYPE;

/* Error Codes */
typedef enum {
    LEX_NO_ERROR		= 0,
	LEX_ERROR_FILEIO	= 1,
	LEX_INVALID_INPUT	= 2
} LLEXER_ERROR_CODE;

class llexer_state {
public:
	int	iCurrOp;

	size_t	iCurrLexemeStart;
	size_t	iCurrLexemeIndex;
	size_t	iCurrLine;
	
	Token tkTokenSubType;
	
	std::string strCurrLexeme;
	
	llexer_state(): iCurrOp(-1), iCurrLine(-1), iCurrLexemeIndex(0), iCurrLexemeStart(0), tkTokenSubType(TOKEN_SUBTYPE_NULL) {}
};

#include <map>
class llexer_operators {
public:
	typedef std::map<std::string, int> OPERATOR_MAP;
	typedef std::map<std::string, int>::iterator OPERATOR_MAP_IT;
	OPERATOR_MAP operators;
	
	void add(const std::string& str, int id) {
		operators[str] = id;
	}
	int check(const std::string& str) {
		OPERATOR_MAP_IT it = operators.begin();
		if((it = operators.find(str)) != operators.end()) {
			return it->second;
		}
		return -1;
	}
	int checkChar(char cChar) {
		OPERATOR_MAP_IT it = operators.begin();
		while(it != operators.end()) {
			if(it->first[0] == cChar) return 1;
			++it;
		}
		return 0;
	}
};

class llexer {
public:
	llexer();
	~llexer();
	/*
	 reset the lexer to default state
	*/
	void reset();
    
	/*
	 load a file to lex
	 @param, path of the file
	 @retval, if succeed, return LEX_NO_ERROR
	 */
	int loadFile(const char*);
	/*
	 load a file from memory
	 @param ptr, the pointer to the memory block that contains the file data
	 @param size, the size of the memory block
	 @retval, if succeed, return LEX_NO_ERROR
	 */
	int loadFileMem(void* ptr, unsigned long size);
	
	/*
	 get next token, return TOKEN_TYPE_INVALID_INPUT if there a invalid input met
	 @retval, next token
	 */
	Token getNextToken();
	/*
	 get current token's subtype, see TOKEN_SUBTPYE_*
	 if getNextToken() = TOKEN_TYPE_INT, then sub type can be 
		TOKEN_SUBTYPE_INT_OCTAL		- Octal Int 
		TOKEN_SUBTYPE_INT_HEX		- Hex Int
		TOKEN_SUBTYPE_NULL			- Demical Int
	 @retval, current token's subtype
	 */
	Token getCurrTokenSubtype();
	/*
	 to next token, when LEXER_INVALID_INPUT error happened, use this function to skip to next token, 
		otherwise the function behaves the same as getNextToken()
	 @retval, next token
	 */
	Token toNextToken();
	
	/*
	 get the lexeme of current token
	 @retval, pointer to the lexeme
	 */
	const char* getCurrLexeme();
	
	/*
	 get current operator, only available when getNextToken() == TOKEN_TYPE_OP
	 @retval, current operator
	 */
	int	getCurrOp();
	
	// information APIs
	/*
	 get the line count of the source file
	 @retval, line count of the source file
	*/
	size_t getSourceLineCount();
	/*
	 get the size of the source file
	 @retval, the size of the source file
	 */
	size_t getSourceSize();
	/*
	 get current char index, use when LEX_INVALID_INPUT happens to get the invalid input char index
	 @retval, current char index in the line
	 */
	size_t getCurrCharIndex();
	/*
	 get current line index, use when LEX_INVALID_INPUT happens to get the current line number of the source
	 @retval, current line index in the source file
	 */
	size_t getCurrLineIndex();
	/*
	 get current line of the source file
	 @retval, current line of the source file
	 */
	const char*	getCurrLine();
	/*
	 get invalid char, use when LEX_INVALID_INPUT happens to get the invalid char, returns 0 if there's no LEX_INVALID_INPUT error happened
	 @retval, invalid char
	 */
	char getCurrInvalidChar();
	
	/*
	 rewind the token stream to previous state
	 @retval, succeed or not
	 */
	int	rewindTokenStream();
	
	/*
	 add a operator ident to parser
	 @param op, the string of the operator, eg "++"
	 @param id, the id of the operator, see getNextToken() and toNextToken();
	 */
	void addOperator(const std::string& op, int id);
    	
private:
	llexer(const llexer&);
	llexer& operator=(const llexer&);
	
	inline char	getNextChar();
	inline void rewindChar();
	inline char getLookAheadChar();
	Token errorInvalidChar(char cCurrChar);
		
	char strInvalidChar;
	unsigned long lSourceSize;
	
	typedef std::vector<std::string> SOURCE_CONT;
	SOURCE_CONT contSource;
	
	llexer_state currState;
	typedef std::vector<llexer_state> LEXER_STATE_CONT;
	LEXER_STATE_CONT contLexerState;
	
	llexer_operators operators;
    
    unsigned short usErrorCode;
};

typedef llexer SoraLexer;

#endif