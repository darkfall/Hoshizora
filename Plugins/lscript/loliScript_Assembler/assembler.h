/*
 *  loadSourceFile.h
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

#ifndef _ASSEMBLER_H_
#define _ASEEMBLER_H_

#include "asm_defines.h"

#include "asm_typeStructs.h"
#include "asm_stringTable.h"
#include "asm_funcTable.h"
#include "asm_symbolTable.h"
#include "asm_labelTable.h"
#include "asm_instructionTable.h"

#include <list>
#include <string>
#include <vector>
#include <fstream>

using std::list;
using std::string;
using std::vector;
using std::ifstream;

const int	ASM_MAX_SOURCE_LINE_SIZE = 4096;

class LSL_Assembler {
public:
	LSL_Assembler();
	~LSL_Assembler();
	
	void    initInstrTable();

	void	loadSourceFile(const char* pstrFileName);
	void	assembleSourceFile();
	void	buildAssembledFile(const char* pstrFileName);
	void	printAssembleStates(const char* pstrFileName);
	
private:
	Token	GetNextToken();
	char*	GetCurrLexeme();
	char	GetLookAheadChar();
	int		SkipToNextLine();
	void	ResetLexer();
	
	void	ExitOnError(const char* pstrErrorMssg);
	void	ExitOnCodeError(const char* pstrErrorMssg);
	void	ExitOnCharExpectedError(char cChar);
	
	
	char **		 ppstrSourceCode;
	int			 iSourceCodeSize;
	int			 iIsSetStackSizeFound;
	LSL_Instr*	 pInstrStream;
	int			 iInstrStreamSize;
	int			 iCurrInstrIndex;
	int			 iIsPriorityFound;
	
	LSL_ScriptHeader scriptHeader;
	
	string			pstrExecFilename;
	
	LSL_StringTable		tStringTable;
	LSL_FuncTable		tFuncTable;
	LSL_StringTable		tHostAPICallTable;
	LSL_SymbolTable		tSymbolTable;
	LSL_LabelTable		tLabelTable;
	LSL_InstrTable		tInstrTable;
	
	FILE*			pSourceFile;
	
	LSL_Lexeme lexer;
};


#endif
