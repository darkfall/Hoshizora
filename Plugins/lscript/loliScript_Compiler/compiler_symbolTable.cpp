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
#include "compiler_symbolTable.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

LSH_SymbolNode* LSH_SymbolTable::getSymbolByIndex(int iIndex) {
	if(iIndex < symbolTable.size())
		return symbolTable[iIndex];
	return NULL;
}

LSH_SymbolNode* LSH_SymbolTable::getSymbolByIdent(const char* pstrName, int iScope) {
	vector<LSH_SymbolNode*>::iterator p = symbolTable.begin();
	
	for(p; p != symbolTable.end(); ++p) {
		if(strcasecmp((*p)->sIdent.c_str(), pstrName) == 0) 
			//to see if symbol scopes are the same or overlap (global/local)
			if((*p)->iScope	== iScope || (*p)->iScope == SCOPE_GLOBAL)
				return (*p);
	}
	return NULL;
}	

int LSH_SymbolTable::addSymbol(const char* pstrIdent, int iSize, int iScope, int iType, const char* pstrStructName) {
	if(getSymbolByIdent(pstrIdent, iScope))
		return -1;
	
	LSH_SymbolNode* pNewSymbol	= new LSH_SymbolNode;
	pNewSymbol->sIdent			= pstrIdent;
	pNewSymbol->iSize			= iSize;
	pNewSymbol->iScope			= iScope;
	pNewSymbol->iType			= iType;
	pNewSymbol->iIndex			= symbolTable.size();
	
	if(iType == SYMBOL_TYPE_STRUCT && pstrStructName)
		pNewSymbol->sStructName = pstrStructName;
	
	symbolTable.push_back(pNewSymbol);
	return symbolTable.size()-1;
}

int LSH_SymbolTable::getSymbolSizeByIdent(const char* pstrIdent, int iScope) {
	LSH_SymbolNode* pSymbol = getSymbolByIdent(pstrIdent, iScope);
	if(pSymbol)
		return pSymbol->iSize;
	return 0;
}