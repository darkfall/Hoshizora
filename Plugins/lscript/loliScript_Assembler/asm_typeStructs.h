/*
 *  struct.h
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

#include <string>

#ifndef ASM_TYPE_STRUCTS_H_
#define ASM_TYPE_STRUCTS_H_

using std::string;

typedef int Token;
typedef int LexState;

#define MAX_INSTR_LOOKUP_COUNT				256
#define MAX_INSTR_MNEMONIC_SIZE				16  

#define MAX_LEXEME_SIZE						256

//Operand

typedef struct _LSL_Op {
	int iType;
	union {
		int		iIntLiteral;
		float	fFloatLiteral;
		int		iStringTableIndex;
		int		iStackIndex;
		int		iInstrIndex;
		int		iFuncIndex;
		int		iHostAPICallIndex;
		int		iReg;
	};
	//use when operand is defined as a relative stack index
	int iOffsetIndex;
} LSL_Op;

//Lexeme

typedef struct _LSL_Lexeme {
	int		iCurrSourceLine;                        // Current line in the source
	size_t	iIndex0,								// Indices into the string
			iIndex1;
	Token	CurrToken;								// Current token
	char	pstrCurrLexeme[MAX_LEXEME_SIZE];		// Current lexeme
	int		iCurrLexState; 
} LSL_Lexeme;

//Instruction

typedef struct _LSL_Instr {
	int iOpcode;
	int iOpCount;
	LSL_Op* pOpList;
	
	_LSL_Instr(): iOpcode(0), iOpCount(0), pOpList(NULL) {}
} LSL_Instr;

//Script Header
/*
 (1). Main Header
 - ID String			“LOSR”						4
 - Version				Major.Minor					2
 - StackSize			(SetStackSize)				4
 - Global Data Size		All Global Data Size		4
 - Priority Type		Priority Type				1
 - User Priority		User Priority time			4
 - _Main() tag			1 - there is _Main 0 - not	1
 - _Main() index		Index of _Main function		4
*/

#define PRIORITY_USER	0
#define PRIORITY_LOW	1
#define PRIORITY_MED	2
#define PRIORITY_HIGH	3

#define PRIORITY_LOW_KEYWORD	"LOW"
#define PRIORITY_MED_KEYWORD	"MEDIUM"
#define PRIORITY_HIGH_KEYWORD	"HIGH"

typedef struct _LSL_ScriptHeader {
	int iStackSize;
	int iGlobalDataSize;
	
	int iIsMainFuncPresent;
	int iMainFuncIndex;
	
	int iPriorityType;
	int iUserPriority;
	
	_LSL_ScriptHeader(): iStackSize(0), iGlobalDataSize(0), iIsMainFuncPresent(FALSE), iMainFuncIndex(0), iPriorityType(1), iUserPriority(0) {}
	
} LSL_ScriptHeader;

#endif


