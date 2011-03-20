/*
 *  compiler_symbolTable.h
 *  loliScript_Assembler
 *
 *  Created by griffin clare on 2010年1月9日.
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

#ifndef _COMPILER_SYMBOLTABLE_H_
#define _COMPILER_SYMBOLTABLE_H_

#include <string>
#include <vector>

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define strcasecmp stricmp
#endif


using std::string;
using std::vector;

#define SCOPE_GLOBAL	-1

#define SYMBOL_TYPE_VAR		0
#define SYMBOL_TYPE_PARAM	1
#define SYMBOL_TYPE_STRUCT  2

typedef struct _LSH_SymbolNode {
	int		iIndex;
	string	sIdent;
	int		iSize;
	int		iScope;
	int		iType;
	
	//
	string  sStructName;
} LSH_SymbolNode;

class LSH_SymbolTable {
public:
	~LSH_SymbolTable() {
		for(size_t i=0; i<symbolTable.size(); ++i)
			delete symbolTable[i];
		symbolTable.clear();
	}

	LSH_SymbolNode* getSymbolByIndex	(int iIndex);
	LSH_SymbolNode* getSymbolByIdent	(const char* pstrName, int iScope);	
	
	int			addSymbol				(const char* pstrIdent, int iSize, int iScope, int iType, const char* pstrStructName=0);	
		
	int			getSymbolSizeByIdent	(const char* pstrIdent, int iScope);
	
	int			size() const			{return symbolTable.size(); }
	
	vector<LSH_SymbolNode*>	symbolTable;
};

#endif