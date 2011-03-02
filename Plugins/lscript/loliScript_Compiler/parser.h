/*
 *  parser.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月10日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include "compiler_funcTable.h"
#include "compiler_stringTable.h"
#include "compiler_symbolTable.h"
#include "compiler_defineTable.h"
#include "compiler_structTable.h"

#include "lexer.h"
#include "compiler_global.h"

#include <stack>
#include <string>

using std::string;
using std::stack;

#define MAX_FUNC_DECLARE_PARAM_COUNT	32

#define PARSER_MAX_IDENT_SIZE			64

#define REG_CODE_RETVAL	0

typedef	struct _LSH_Expr {
	int iStackOffset;
} LSH_Parser_Expr;

typedef enum {
	LOOP_TYPE_NULL,
	LOOP_TYPE_WHILE,
	LOOP_TYPE_FOR,
	LOOP_TYPE_IF_IN_LOOP,
} LSH_LOOP_TYPE;

typedef enum {
	LOGICAL_EXPRESSION_IF,
	LOGICAL_EXPRESSION_WHILE,
	LOGICAL_EXPRESSION_FOR,
} LSH_LOGICAL_EXPRESSION_TYPE;

typedef enum {
	LOGICAL_OR,
	LOGICAL_AND
} LSH_LOGICAL_OPERATOR_TYPE;

typedef struct _LSH_Loop {
	int iStartTargetIndex;
	int iEndTargetIndex;
	
	LSH_LOOP_TYPE eType;
	
	_LSH_Loop(): iStartTargetIndex(0), iEndTargetIndex(0), eType(LOOP_TYPE_NULL) {}
} LSH_Parser_Loop;

typedef struct _LSH_LogicalExpressionTarget {
	int iStartTargetIndex;
	int iEndTargetIndex;
	
	LSH_LOGICAL_EXPRESSION_TYPE eType;
} LSH_LogicalExpressionTarget;

class LSH_Parser {
public:
	friend class LSH_Compiler;
	friend class LSH_lexer;
	
	LSH_Parser();
	~LSH_Parser();
	
	bool parseSourceCode();
	
private:	
	vector<string> vSourceCode;
	
	//runtime loop stack, for break/continue
	stack<LSH_Parser_Loop*> sLoopStack;
	stack<LSH_LogicalExpressionTarget> sLogicalLoopTargetStack;
	stack<LSH_LOGICAL_OPERATOR_TYPE> sLogicalOperatorStack;
		
	void initReservedFunctions();
	
	void readToken(Token reqToken);
	
	int isOpRelational	(int iOpType);
	int isOpLogical		(int iOpType);
	
	void parseStatement();
	void parseBlock();
	
	void parseVar();
	void parseHost();
	void parseFunc();
	
	void parseExpr();
	void parseSubExpr();
	void parseTerm();
	void parseFactor();
	
	void parseIf();
	void parseWhile();
	void parseFor();
	void parseBreak();
	void parseContinue();
	void parseReturn();
	
	void parseForwardOp();
	
	void parsePrint();
	
	void parseAssign();
	void parseFuncCall(int iIsReturn);
	
	void parsePound();
	void replaceSourceDefineSymbols(const string& sDefineName);
	
	void parseStruct();
	void parseStructAssign();
	void structElmntToArrayIndex(const string& sIdentName);
	
	
	void optimizeInstrStream	(int iLoopTime);
	bool isValueOperated		(int iSymbolIndex, int iStartIndex, int iEndIndex, LSH_FuncNode* pFunc);
	bool cmpOp					(const LSH_Op* op1, const LSH_Op* op2);
	bool isOpVar				(const LSH_Op* op);
	bool isOpTemp				(const LSH_Op* op);
	void copyOp					(LSH_Op* dest, LSH_Op* source);
	
	void combinePushPop			(LSH_FuncNode* pFunc);
	void combineMovs			(LSH_FuncNode* pFunc);
	void combineTempVariables	(LSH_FuncNode* pFunc);
	void removeUselessMoves		(LSH_FuncNode* pFunc);
	void calculateExpressions	(LSH_FuncNode* pFunc);
	void removeNullNodes		(LSH_FuncNode* pFunc);
	
	void exitOnCodeError(const char* pstrErrorMssg);
		
	int iCurrScope;
	
	int iTempVar1SymbolIndex, //array index and operator	
		iTempVar0SymbolIndex, //mid-value and operator
		iTempVar2SymbolIndex; //for loop expression value
	
	LSH_StringTable		stringTable;
	LSH_SymbolTable		symbolTable;
	LSH_FuncTable		funcTable;
	LSH_DefineTable		defineTable;
	LSH_StructTable     structTable;
	
	LSH_ScriptHeader	scriptHeader;

	LSH_lexer*			pLexer;

	bool				bError;
};

#endif