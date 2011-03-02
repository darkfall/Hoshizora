/*
 *  intructionTable.cpp
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

#include "asm_instructionTable.h"

#include "asm_stringProcess.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

int	LSL_InstrTable::addInstrLookup(const char* pstrMnemonic, int iOpcode, int iOpCount) {
	if(iInstrIndex >= ASM_MAX_INSTR_LOOKUP_COUNT)
		return -1;
	
	strcpy(instrTable[iInstrIndex].sMnemonic, pstrMnemonic);
	strupr(instrTable[iInstrIndex].sMnemonic);
	instrTable[iInstrIndex].iOpcode	= iOpcode;
	instrTable[iInstrIndex].iOpCount	= iOpCount;
	
	instrTable[iInstrIndex].opList = (LSL_OpTypes*)malloc(iOpCount*sizeof(LSL_OpTypes));
	
	++iInstrIndex;
	return iInstrIndex-1;
}


void LSL_InstrTable::setOpType(int iInstrIndex, int iOpIndex, LSL_OpTypes iOpType)	{
	instrTable[iInstrIndex].opList[iOpIndex] = iOpType;
}

int	LSL_InstrTable::getInstrByMnemonic(const char* pstrMnemonic, LSL_InstrLookup* pInstr) {
	for(int iCurrInstrIndex = 0; iCurrInstrIndex < iInstrIndex; ++iCurrInstrIndex) {
		if(strcmp(instrTable[iCurrInstrIndex].sMnemonic, pstrMnemonic) == 0) {
			*pInstr = instrTable[iCurrInstrIndex];
			return TRUE;
		}
	}
	return FALSE;
}