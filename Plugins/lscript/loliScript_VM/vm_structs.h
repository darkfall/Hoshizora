/*
 *  vm_structs.h
 *  loliScript_VM
 *
 *  Created by griffin clare on 09年12月24日.
 *  Copyright 2009 ring0Dev. All rights reserved.
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

#ifndef _VM_STRUCTS_H_
#define _VM_STRUCTS_H_

#include "vm_defines.h"

typedef struct _VM_Value {
	int iType;
	union  {
		int		iIntLiteral;
		float	fFloatLiteral;
		char*	pstrStringLiteral;
		int		iStackIndex;
		int		iInstrIndex;
		int		iFuncIndex;
		int		iHostAPICallIndex;
		int		iReg;
	};
	int	iOffsetIndex;
	
	_VM_Value(): iType(-1), iOffsetIndex(0) {}
} VM_Value;

typedef struct _VM_Instr {
	int		iOpcode;
	int		iOpCount;
	VM_Value*	pOpList;
	
	_VM_Instr(): iOpcode(0), iOpCount(0) {}
} VM_Instr;

typedef struct _VM_InstrStream {
	VM_Instr*	pInstrs;
	int			iSize;
	int			iCurrInstr;
	
	_VM_InstrStream(): iSize(0), iCurrInstr(-1) {}
} VM_InstrStream;

typedef struct _VM_RuntimeStack {
	VM_Value*	pElmnts;
	int			iSize;
	int			iTopIndex;
	int			iFrameIndex;		//Current Framework Index
	
	_VM_RuntimeStack(): iSize(0), iTopIndex(0), iFrameIndex(0) {}
} VM_RuntimeStack;

typedef struct _VM_Func {
	int	iEntryPoint;
	int	iParamCount;
	int	iLocalDataSize;
	int	iStackFrameSize;
	
	char* pstrFuncName;
	
	_VM_Func(): iEntryPoint(0), iParamCount(0), iLocalDataSize(0), iStackFrameSize(0) {}
} VM_Func;

typedef struct _VM_HostAPICallTable {
	char**	ppstrCalls;
	int		iSize;
	
	_VM_HostAPICallTable(): iSize(0) {}
} VM_HostAPICallTable;

typedef struct _VM_HostAPIFunc {
	int iIsActive;
	
	char* pstrName;
	HostAPIFuncPntr fnFunc;
	
	_VM_HostAPIFunc():iIsActive(FALSE), pstrName(NULL) {}
} VM_HostAPIFunc;

#endif
