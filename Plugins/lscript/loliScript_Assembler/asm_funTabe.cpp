/*
 *  funTab;e.cpp
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


#include "asm_funcTable.h"

LSL_FuncTable::~LSL_FuncTable() {
	FUNC_LIST::iterator p = funcTable.begin();
	while(p != funcTable.end()) {
		delete (*p);
		(*p) = 0;
		++p;
	}
	funcTable.clear();
}

LSL_FuncNode* LSL_FuncTable::getFuncByName(const char* pstrName) {
	FUNC_LIST::iterator p = funcTable.begin();
	
	for(p; p != funcTable.end(); ++p) {
		if((*p)->sFuncName.compare(pstrName) == 0)
			return (*p);
	}
	return (*funcTable.end()) ;
}	

int LSL_FuncTable::addFunc(const char* pstrName, int iEntryPoint) {
	FUNC_LIST::iterator p = funcTable.begin();
	
	for(p; p != funcTable.end(); ++p) {
		if((*p)->sFuncName.compare(pstrName) == 0)
			return -1;
	}
	
	LSL_FuncNode* pNewFunc	= new LSL_FuncNode;
	pNewFunc->sFuncName		= pstrName;
	pNewFunc->iIndex		= funcTable.size();
	pNewFunc->iEntryPoint	= iEntryPoint;
	
	funcTable.push_back(pNewFunc);
	return funcTable.size()-1;
}

void LSL_FuncTable::setFuncInfo(const char* pstrName, int iParamCount, int iLocalDataSize) {
	FUNC_LIST::iterator p = funcTable.begin();
	
	for(p; p != funcTable.end(); ++p) {
		if((*p)->sFuncName.compare(pstrName) == 0)
			break;
	}
	
	if(p != funcTable.end()) {
		(*p)->iParamCount = iParamCount;
		(*p)->iLocalDataSize = iLocalDataSize;
	}
}