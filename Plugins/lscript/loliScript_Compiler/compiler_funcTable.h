/*
 *  funcTable.h
 *  loliScript_Assembler
 *
 *  Created by griffin clare on 09年12月21日.
 *  Copyright 2009 ring0dev. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  
 *  Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  
 *  Neither the name of this project's author nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */
#pragma once

#include <string>
#include <list>
#include <vector>

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define strcasecmp stricmp
#endif

using std::list;
using std::string;
using std::vector;

#define ICODE_NODE_NULL			-1
#define ICODE_NODE_INSTR		0
#define ICODE_NODE_SOURCE_LINE	1
#define ICODE_NODE_JUMP_TARGET	2

#define OP_TYPE_NULL						-1
#define OP_TYPE_INT							0
#define OP_TYPE_FLOAT						1
#define OP_TYPE_STRING_INDEX				2
#define OP_TYPE_VAR							3
#define	OP_TYPE_ABS_ARRAY_INDEX				4
#define OP_TYPE_VAR_ARRAY_INDEX				5
#define OP_TYPE_JUMP_TARGET_INDEX			6
#define OP_TYPE_FUNC_INDEX					7
#define OP_TYPE_REG							9

#define INSTR_NULL				-1

#define INSTR_MOV               0

#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

#define INSTR_PUSH              26
#define INSTR_POP               27

#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

#define INSTR_PAUSE             31
#define INSTR_EXIT              32

#define INSTR_PRINT				33

#define INSTR_SIN				34
#define INSTR_COS				35
#define INSTR_TAN				36
#define INSTR_LN				37
#define INSTR_LG				38
#define INSTR_EXPL				39
#define INSTR_REC				40
#define INSTR_SH				41
#define INSTR_CH				42
#define INSTR_TH				43
#define INSTR_SQRT				44

#define INSTR_RANDOMINT			48
#define INSTR_RANDOMFLOAT		49

#define INSTR_PUSHPARAM			45
#define INSTR_PUSHHOSTPARAM		46

#define INSTR_PARAM				47


typedef enum _FUNCTION_TYPES {
	FUNCTION_TYPE_NONE,
	FUNCTION_TYPE_HOSTAPI,
	FUNCTION_TYPE_SCRIPT,
	FUNCTION_TYPE_RSRVD_INSTR,
	FUNCTION_TYPE_RSRVD_FUNC,
	FUNCTION_TYPE_CLASS,
};

typedef struct _LSH_Op {
	int iType;
	union {
		int		iIntLiteral;
		float	fFloatLiteral;
		int		iStringIndex;
		int		iSymbolIndex;
		int		iJumpTargetIndex;
		int		iFuncIndex;
		int		iRegCode;
	};
	int iOffset;
	int iOffsetSymbolIndex;
	
	_LSH_Op():iType(OP_TYPE_INT), iIntLiteral(0) {}
} LSH_Op;

typedef struct _LSH_ICodeInstr {
	int iOpcode;
	
	vector<LSH_Op*> vOpList;
	
	_LSH_ICodeInstr():iOpcode(-1) {}
	~_LSH_ICodeInstr();
} LSH_ICodeInstr;

typedef struct _LSH_ICodeNode {
	int iType;
	
	LSH_ICodeInstr	instr;

	union  {
		char*			pstrSourceLine;
		
		int				iJumpTargetIndex;
	};
} LSH_ICodeNode;


typedef struct _LSH_FuncNode {
	int					iIndex;
	string				sFuncName;
	int					iParamCount;
	int					iLocalDataSize;
	_FUNCTION_TYPES		ftType;
	
	string sStructName; // which struct the func belongs to;
	int	   iType;		// function type, public or private etc
	
	int					iIsReturnValue;
	
	vector<LSH_ICodeNode*> vICodeStream;
	
	~_LSH_FuncNode();
} LSH_FuncNode;

class LSH_FuncTable {
public:	
	LSH_FuncTable();
	~LSH_FuncTable();
	
	LSH_FuncNode*	getFuncByName	(const char* pstrName);
	LSH_FuncNode*	getFuncByIndex	(int iIndex);
	
	int			addFunc				(const char* pstrName, _FUNCTION_TYPES type);	
	
	void		setFuncParamCount	(int iIndex, int iParamCount);
	
	int			size() const		{return funcTable.size(); }
	
	//ICode operations
	
	LSH_ICodeNode*	getICodeNodeByImpIndex	(int iFuncIndex, int iInstrIndex);
	
	void			addICodeSourceLine		(int iFuncIndex, const char* pstrSourceLine);
	
	int				addICodeInstr			(int iFuncIndex, int iOpcode);
	
	LSH_Op*			getICodeOpByIndex		(LSH_ICodeNode* pInstr, int iOpIndex);
	
	
	void addICodeOp	(int iFuncIndex, int iInstrIndex, LSH_Op oValue);
	
	void addIntICodeOp				(int iFuncIndex, int iInstrIndex, int iValue);
	void addFloatICodeOp			(int iFuncIndex, int iInstrIndex, float fValue);
	void addStringICodeOp			(int iFuncIndex, int iInstrIndex, int iStringIndex);
	void addVarICodeOp				(int iFuncIndex, int iInstrIndex, int iSymbolIndex);
	void addArrayIndexAbsICodeOp	(int iFuncIndex, int iInstrIndex, int iArraySymbolIndex, int iOffset);
	void addArrayIndexVarICodeOp	(int iFuncIndex, int iInstrIndex, int iArraySymbolIndex, int iOffsetSymbol);
	void addFuncICodeOp				(int iFuncIndex, int iInstrIndex, int iOpFuncIndex);
	void addRegICodeOp				(int iFuncIndex, int iInstrIndex, int iRegCode);
	void addJumpTargetICodeOp		(int iFuncIndex, int iInstrIndex, int iTargetIndex);
	
	int  getNextJumpTargetIndex();
	
	void addICodeJumpTarget (int iFuncIndex, int iTargetIndex);
	
	
	int		coerceICodeOpToInt(LSH_Op* op);
	float	coerceICodeOpToFloat(LSH_Op* op);	
	
private:
	int iCurrJumpTargetIndex;

	vector<LSH_FuncNode*> funcTable;

	static LSH_FuncTable* instance;
};
