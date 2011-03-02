/*
 *  labelTable.cpp
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
// #include "../../KuiWin32/headers/xLog.h"

#include "asm_labelTable.h"

LSL_LabelTable::~LSL_LabelTable() {
	list<LSL_LabelNode*>::iterator p = labelTable.begin();
	while(p != labelTable.end()) {
		if(*p)
			delete *p;
		++p;
	}
	labelTable.clear();
}

int	LSL_LabelTable::addLabel(const char* pstrIdent, int iTargetIndex, int iFuncIndex)  {
	list<LSL_LabelNode*>::iterator p = labelTable.begin();
	
	for(p; p != labelTable.end(); ++p) {
		if((*p)->sIdent.compare(pstrIdent) == 0 && iFuncIndex == (*p)->iFuncIndex)
			return -1;
	}
	
	LSL_LabelNode* pNewLabel	= new LSL_LabelNode;
	pNewLabel->sIdent		= pstrIdent;
	pNewLabel->iTargetIndex = iTargetIndex;
	pNewLabel->iFuncIndex	= iFuncIndex;
	pNewLabel->iIndex		= labelTable.size();
	
	labelTable.push_back(pNewLabel);
	return labelTable.size()-1;
}	

LSL_LabelNode* LSL_LabelTable::getLabelByIdent(const char* pstrIdent, int iFuncIndex)  {
	list<LSL_LabelNode*>::iterator p = labelTable.begin();
	
	for(p; p != labelTable.end(); ++p) {
		if((*p)->sIdent.compare(pstrIdent) == 0 &&
		   (*p)->iFuncIndex == iFuncIndex )
			return (*p);
	}
	return NULL;
}	