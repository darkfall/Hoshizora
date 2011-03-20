/*
 *  symbolTable.cpp
 *  loliScript_Assembler
 *
 *  Created by griffin clare on 09年12月22日.
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

// #include "stdafx.h"

#include "asm_symbolTable.h"

LSL_SymbolTable::~LSL_SymbolTable() {
	list<LSL_SymbolNode*>::iterator p = symbolTable.begin();
	while(p != symbolTable.end()) {
		if(*p)
			delete *p;
		++p;
	}
	symbolTable.clear();
}

LSL_SymbolNode* LSL_SymbolTable::getSymbolByIdent(const char* pstrName, int iFuncIndex) {
	list<LSL_SymbolNode*>::iterator p = symbolTable.begin();
	
	for(p; p != symbolTable.end(); ++p) {
		if((*p)->sIdent.compare(pstrName) == 0) 
			//to see if symbol scopes are the same or overlap (global/local)
			if((*p)->iFuncIndex == iFuncIndex || (*p)->iStackIndex >= 0)
				return (*p);
	}
	return NULL;
}	

int LSL_SymbolTable::addSymbol(const char* pstrIdent, int iSize, int iStackIndex, int iFuncIndex) {
	list<LSL_SymbolNode*>::iterator p = symbolTable.begin();
	
	for(p; p != symbolTable.end(); ++p) {
		if((*p)->sIdent.compare(pstrIdent) == 0)
			if((*p)->iFuncIndex == iFuncIndex || iStackIndex >= 0)
				return -1;
	}
	
	LSL_SymbolNode* pNewSymbol	= new LSL_SymbolNode;
	pNewSymbol->sIdent		= pstrIdent;
	pNewSymbol->iStackIndex = iStackIndex;
	pNewSymbol->iFuncIndex	= iFuncIndex;
	pNewSymbol->iSize		= iSize;
	pNewSymbol->iIndex		= symbolTable.size();
	
	symbolTable.push_back(pNewSymbol);
	return symbolTable.size()-1;
}

int LSL_SymbolTable::getStackIndexByIdent(const char* pstrIdent, int iFuncIndex) {
	LSL_SymbolNode* pSymbol = getSymbolByIdent(pstrIdent, iFuncIndex);
	if(pSymbol)
		return pSymbol->iStackIndex;
	return 0;
}

int LSL_SymbolTable::getSizeByIdent(const char* pstrIdent, int iFuncIndex) {
	LSL_SymbolNode* pSymbol = getSymbolByIdent(pstrIdent, iFuncIndex);
	if(pSymbol)
		return pSymbol->iSize;
	return 0;
}