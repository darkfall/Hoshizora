/*
 *  labelTable.h
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

#ifndef _ASM_LABELTABLE_H_
#define _ASM_LABELTABLE_H_

#include <string>
#include <vector>

using std::string;

typedef struct _LSL_labelNode {
	int		iIndex;
	string	sIdent;
	int		iTargetIndex;
	int		iFuncIndex;
} LSL_LabelNode;

class LSL_LabelTable {
public:
	~LSL_LabelTable();
	
	int			addLabel		(const char* pstrIdent, int iTargetIndex, int iFuncIndex);	
	LSL_LabelNode*	getLabelByIdent	(const char* pstrIdent, int iFuncIndex);
	
	int			size() const	{return labelTable.size(); }
	
	typedef std::vector<LSL_LabelNode*> LABEL_LIST;
	LABEL_LIST labelTable;
};

#endif
	