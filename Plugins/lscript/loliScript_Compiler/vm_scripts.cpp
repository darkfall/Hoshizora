/*
 *  vm_scripts.cpp
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

// #include "stdafx.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

#include "vm_scripts.h"
#include "vm_defines.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

#include <cstring>
#include <cmath>

#include <iostream>
#include <string>

#include "vm_defines.h"

#include "lsLog.h"
#include "x_vm_hostAPIFuncTable.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)
#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) / M_PI * 180.0)

//operand ids
#define VM_OP_TYPE_NULL							-1
#define VM_OP_TYPE_INT							0
#define VM_OP_TYPE_FLOAT						1
#define VM_OP_TYPE_STRING						2
#define	VM_OP_TYPE_ABS_STACK_INDEX				3
#define VM_OP_TYPE_REL_STACK_INDEX				4
#define VM_OP_TYPE_INSTR_INDEX					5
#define VM_OP_TYPE_FUNC_INDEX					6
#define VM_OP_TYPE_HOST_API_CALL_INDEX			7
#define VM_OP_TYPE_REG							8
#define VM_OP_TYPE_STACK_BASE_MARKER			9

#define INSTR_MOV               0

#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

#define INSTR_PUSH              26
#define INSTR_POP               27

#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

#define INSTR_PAUSE             31
#define INSTR_EXIT              32

#define INSTR_PRINT				33

#define INSTR_SIN				34
#define INSTR_COS				35
#define INSTR_TAN				36
#define INSTR_LN				37
#define INSTR_LG				38
#define INSTR_EXPL				39
#define INSTR_REC				40
#define INSTR_SH				41
#define INSTR_CH				42
#define INSTR_TH				43
#define INSTR_SQRT				44

#define INSTR_RANDOMINT			48
#define INSTR_RANDOMFLOAT		49

#define INSTR_PUSHPARAM			45
#define INSTR_PUSHHOSTPARAM		46

#define INSTR_PARAM				47

using namespace std;

inline char* int_to_strVM(int n)
{
	char *str = NULL;                  
	int length = 1;                     
	int temp = 0;
	int sign = 1;                      
	
	if(n<0) {
		sign = -1;
		n = -n;
		++length;                         
	}
	
	temp = n;
	do {
		++length;
	}
	while((temp /= 10)>0);
	
	str = (char*)malloc(length);        
	
	if(sign<0)                      
		str[0] = '-';                    
	
	str[--length] = '\0';             
	
	do {
		str[--length] = '0'+n%10;
	}while((n /= 10) > 0);
	
	return str;
}

inline char* fp_to_strVM(double x)
{
	char *str = NULL;            
	char *integral = NULL;           
	char *fraction = NULL;          
	size_t length = 0;            
	
	integral = int_to_strVM((int)x);    
	
	if(x<0)
		x = -x;
	fraction = int_to_strVM((int)(100.0*(x+0.005-(int)x)));
	
	length = strlen(integral)+strlen(fraction)+2;  /* Total length including point and terminator */
	
	/* Fraction must be two digits so allow for it */
	if(strlen(fraction)<2)
		++length;
	
	str = (char*)malloc(length);        /* Allocate memory for total */
	strcpy(str, integral);              /* Copy the integral part    */
	strcat(str, ".");                   /* Append decimal point      */
	
	if(strlen(fraction)<2)              /* If fraction less than two digits */
		strcat(str,"0");                  /* Append leading zero       */
	
	strcat(str, fraction);              /* Append fractional part    */
	
	/* Free up memory for parts as strings */
	free(integral);
	free(fraction);
	
	return str;
}

inline float vm_sh(float fParam) {
	return ((exp(fParam)-exp(-fParam))/2);
}

inline float vm_ch(float fParam) {
	return ((exp(fParam)-exp(-fParam))/2);
}

inline float vm_th(float fParam) {
	return ((exp(fParam)-exp(-fParam))/(exp(fParam)-exp(-fParam)));
}

vm_script::vm_script(const char* pstrFile) {
	iCurrState = vm_script_state_notLoad;
	if(loadScript(pstrFile) == LOAD_SUCCESS) {
		resetScript();
	} else {
		iCurrState = vm_script_state_loadFailed;
	}
}

vm_script::vm_script() {
	iCurrState = vm_script_state_notLoad;
	
	iIsMainFuncPresent			= FALSE;
	iIsPaused					= FALSE;
	instrStream.pInstrs			= NULL;
	stack.pElmnts				= NULL;
	pFuncTable					= NULL;
	hostAPICallTable.ppstrCalls = NULL;
	iTimeSliceDur				= 0;
	iCurrThread					= -1;

}

vm_script::~vm_script() {
	releaseScript();
}

void vm_script::runScript() {
	if(iCurrState == vm_script_state_loaded)
		if(iIsMainFuncPresent)
			iIsRunning = TRUE;
}

void vm_script::releaseScript() {
	if(iCurrState == vm_script_state_loaded) {
		for(int i=0; i<instrStream.iSize; ++i) {
		//	for(int j=0; j<instrStream.pInstrs[i].iOpCount; ++j) 
			//	if(instrStream.pInstrs[i].pOpList[j].iType == VM_OP_TYPE_STRING)
				//	if(instrStream.pInstrs[i].pOpList[j].pstrStringLiteral) free(instrStream.pInstrs[i].pOpList[j].pstrStringLiteral);
			free(instrStream.pInstrs[i].pOpList);
		}
		if(instrStream.pInstrs)
			free(instrStream.pInstrs);
		
//		for(int i=0; i<stack.iSize; ++i) {
//			if(stack.pElmnts[i].iType == VM_OP_TYPE_STRING)
//				if(stack.pElmnts[i].pstrStringLiteral) free(stack.pElmnts[i].pstrStringLiteral);
//		}
		if(stack.pElmnts)
			free(stack.pElmnts);
		
		free(pFuncTable);
		iFuncTableSize = 0;
		
		for(int i=0; i<hostAPICallTable.iSize; ++i)
			if(hostAPICallTable.ppstrCalls[i])
				free(hostAPICallTable.ppstrCalls[i]);
		free(hostAPICallTable.ppstrCalls);
	}
	iIsRunning = FALSE;
	iCurrState = vm_script_state_notLoad;

	iFuncCallFromHost = FALSE;
	iFuncInvokeFromHost = FALSE;
	iIsMainFuncPresent = FALSE;
	iIsPaused = FALSE;
	iIsMainFuncPresent = FALSE;
	paramStack.clear();
	hostAPIParamStack.clear();
	instrStream.pInstrs			= NULL;
	stack.pElmnts				= NULL;
	pFuncTable					= NULL;
	hostAPICallTable.ppstrCalls = NULL;
	iTimeSliceDur				= 0;
	iCurrThread					= -1;

}

void vm_script::setPauseTime(int iPauseTime) {
	if(iIsPaused) {
		VM_addmillistm(lPauseEndTime, iPauseTime);
#ifdef VM_VERBOSE_MODE
		cout<<"\n\n Pause Time: "<<lPauseEndTime.time<<" millitm: "<<lPauseEndTime.millitm<<endl;
#endif
		return;
	}
		
	lPauseEndTime = VM_getCurrentTime();
	VM_addmillistm(lPauseEndTime, iPauseTime);
#ifdef VM_VERBOSE_MODE
	cout<<"\n **Pause Time: "<<lPauseEndTime.time<<" millitm: "<<lPauseEndTime.millitm<<endl;
#endif
	
	iIsPaused = TRUE;
	
#ifdef VM_VERBOSE_MODE
	cout<<" **Curr Time: "<<VM_getCurrentTime().time<<" millitm: "<<VM_getCurrentTime().millitm<<endl;
#endif
}

int vm_script::loadScript(const char* pstrFile) {
	FILE* pScriptFile = fopen(pstrFile, "rb");
	if(!pScriptFile)
		return LOAD_ERROR_INVALID_FILE;
	
	//read script id
	char* pstrIDString = (char*)malloc(5);
	fread(pstrIDString, 4, 1, pScriptFile);
	pstrIDString[strlen(LOLISCRIPT_ID_STRING)] = '\0';
	if(strcmp(pstrIDString, LOLISCRIPT_ID_STRING) != 0)
		return LOAD_ERROR_INVALID_ID;
	
	free(pstrIDString);
	
	//read version
	int iMajorVersion = 0, iMinorVersion = 0;
	fread(&iMajorVersion, 1, 1, pScriptFile);
	fread(&iMinorVersion, 1, 1, pScriptFile);
	
	if(iMajorVersion != ASSEMBLY_FILE_VERSION_MAJOR || iMinorVersion != ASSEMBLY_FILE_VERSION_MINOR)
		return LOAD_ERROR_UNSUPPORTED_VERSION;
	
	//read stack
	fread(&stack.iSize, 4, 1, pScriptFile);
	if(stack.iSize == 0)
		stack.iSize = DEFAULT_STACKSIZE;
	
	if(!(stack.pElmnts = (VM_Value*)malloc(stack.iSize * sizeof(VM_Value))) )
		return LOAD_ERROR_OUT_OF_MEMORY;
	
	//global data
	fread(&iGlobalDataSize, 4, 1, pScriptFile);
	
	int iPriorityType = 0;
	int iUserPriority = 0;
	fread(&iPriorityType, 1, 1, pScriptFile);
	fread(&iUserPriority, 4, 1, pScriptFile);
	switch (iPriorityType) {
		case PRIORITY_USER:
			iTimeSliceDur = iUserPriority;
			break;
			
		case PRIORITY_LOW:
			iTimeSliceDur = THREAD_TIMESLICE_LOW;
			break;
			
		case PRIORITY_MED:
			iTimeSliceDur = THREAD_TIMESLICE_MEDIUM;
			break;
			
		case PRIORITY_HIGH:
			iTimeSliceDur = THREAD_TIMESLICE_HIGH;
			break;
			
		default:
			iTimeSliceDur = THREAD_TIMESLICE_LOW;
			break;
	}
#ifdef VM_VERBOSE_MODE
	logMssg("Get Priority Type: %d, Time Slice Duration: %d\n", iPriorityType, iTimeSliceDur);
#endif
	
	//read main func index
	fread(&iIsMainFuncPresent, 1, 1, pScriptFile);
	fread(&iMainFuncIndex, 4, 1, pScriptFile);
	
	//read instructions
	fread(&instrStream.iSize, 4, 1, pScriptFile);
	if(!(instrStream.pInstrs = (VM_Instr*)malloc(instrStream.iSize * sizeof(VM_Instr))) )
		return LOAD_ERROR_OUT_OF_MEMORY;
	
	for(int iCurrInstrIndex = 0; iCurrInstrIndex < instrStream.iSize; ++iCurrInstrIndex) {
		instrStream.pInstrs[iCurrInstrIndex].iOpcode = 0;
		fread(&instrStream.pInstrs[iCurrInstrIndex].iOpcode, 2, 1, pScriptFile);
		
		instrStream.pInstrs[iCurrInstrIndex].iOpCount = 0;
		fread(&instrStream.pInstrs[iCurrInstrIndex].iOpCount, 1, 1, pScriptFile);
		
		int iOpCount = instrStream.pInstrs[iCurrInstrIndex].iOpCount;
		
		VM_Value* pOpList = (VM_Value*)malloc(iOpCount * sizeof(VM_Value));
		if(!pOpList)
			return LOAD_ERROR_OUT_OF_MEMORY;
		
#ifdef VM_VERBOSE_MODE
		logMssg("Reading Instruction: Count: %d  opCode: %d  iOpCount: %d\n", iCurrInstrIndex, instrStream.pInstrs[iCurrInstrIndex].iOpcode, instrStream.pInstrs[iCurrInstrIndex].iOpCount);
#endif
		
		for(int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
			pOpList[iCurrOpIndex].iType = 0;
			fread(&pOpList[iCurrOpIndex].iType, 1, 1, pScriptFile);
			
			switch (pOpList[iCurrOpIndex].iType) {
				case VM_OP_TYPE_INT:
					fread(&pOpList[iCurrOpIndex].iIntLiteral, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_FLOAT:
					fread(&pOpList[iCurrOpIndex].fFloatLiteral, sizeof(float), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_STRING:
					fread(&pOpList[iCurrOpIndex].iIntLiteral, sizeof(int), 1, pScriptFile);
					pOpList[iCurrOpIndex].iType = VM_OP_TYPE_STRING;
					break;
					
				case VM_OP_TYPE_INSTR_INDEX:
					fread(&pOpList[iCurrOpIndex].iInstrIndex, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_ABS_STACK_INDEX:
					fread(&pOpList[iCurrOpIndex].iStackIndex, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_REL_STACK_INDEX:
					fread(&pOpList[iCurrOpIndex].iStackIndex, sizeof(int), 1, pScriptFile);
					fread(&pOpList[iCurrOpIndex].iOffsetIndex, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_FUNC_INDEX:
					fread(&pOpList[iCurrOpIndex].iFuncIndex, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_HOST_API_CALL_INDEX:
					fread(&pOpList[iCurrOpIndex].iHostAPICallIndex, sizeof(int), 1, pScriptFile);
					break;
					
				case VM_OP_TYPE_REG:
					fread(&pOpList[iCurrOpIndex].iReg, sizeof(int), 1, pScriptFile);
					break;
			}
		}
		instrStream.pInstrs[iCurrInstrIndex].pOpList = pOpList;
	}
	
	//read string table
	int iStringTableSize = 0;
	char** ppstrStringTable;

	fread(&iStringTableSize, 4, 1, pScriptFile);
	
#ifdef VM_VERBOSE_MODE
	logMssg("String Table Size: %d\n", iStringTableSize);
#endif
	
	if(!(ppstrStringTable = (char**)malloc(iStringTableSize * sizeof(char*))) )
		return LOAD_ERROR_OUT_OF_MEMORY;
	
	for(int iCurrStringIndex = 0; iCurrStringIndex < iStringTableSize; ++iCurrStringIndex) {
		int iCurrStringLength = 0;
		fread(&iCurrStringLength, 4, 1, pScriptFile);
		
		ppstrStringTable[iCurrStringIndex] = (char*)malloc(iCurrStringLength+1);
		fread(ppstrStringTable[iCurrStringIndex], iCurrStringLength, 1, pScriptFile);
		ppstrStringTable[iCurrStringIndex][iCurrStringLength] = '\0';
		
#ifdef VM_VERBOSE_MODE
		logMssg("read string: %s\n", ppstrStringTable[iCurrStringIndex]);
#endif
	}
		
	//replace string instruction's string operand with string in the string table
	for(int iCurrInstrIndex = 0; iCurrInstrIndex < instrStream.iSize; ++iCurrInstrIndex) {
		int			iOpCount	= instrStream.pInstrs[iCurrInstrIndex].iOpCount;
		VM_Value*	pOpList		= instrStream.pInstrs[iCurrInstrIndex].pOpList;
		
		for(int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
			if(pOpList[iCurrOpIndex].iType == VM_OP_TYPE_STRING) {
				int iStringIndex = pOpList[iCurrOpIndex].iIntLiteral;
							
				char* pstrStringCopy = (char*)malloc(strlen(ppstrStringTable[iStringIndex])+1);
				strcpy(pstrStringCopy, ppstrStringTable[iStringIndex]);
				pOpList[iCurrOpIndex].pstrStringLiteral = pstrStringCopy;
			} else if(pOpList[iCurrOpIndex].iType == VM_OP_TYPE_HOST_API_CALL_INDEX) {
				int iHostAPIIndex = pOpList[iCurrOpIndex].iIntLiteral;

				char* pstrStringCopy = (char*)malloc(strlen(ppstrStringTable[iHostAPIIndex])+1);
				strcpy(pstrStringCopy, ppstrStringTable[iHostAPIIndex]);
				pOpList[iCurrOpIndex].pstrStringLiteral = pstrStringCopy;
			}
		}
	}
	
	for(int iCurrStringIndex = 0; iCurrStringIndex < iStringTableSize; ++iCurrStringIndex)
		free(ppstrStringTable[iCurrStringIndex]);
	free(ppstrStringTable);
	
	
	//read function table
	fread(&iFuncTableSize, 4, 1, pScriptFile);
	
	if(!(pFuncTable = (VM_Func *)malloc(iFuncTableSize * sizeof(VM_Func))) )
		return LOAD_ERROR_OUT_OF_MEMORY;
	
	for(int iCurrFuncIndex = 0; iCurrFuncIndex < iFuncTableSize; ++iCurrFuncIndex) {
		int iEntryPoint = 0;
		fread(&iEntryPoint, 4, 1, pScriptFile);
		
		int iParamCount = 0;
		fread(&iParamCount, 1, 1, pScriptFile);
		
		int iLocalDataSize = 0;
		fread(&iLocalDataSize, 4, 1, pScriptFile);
		
		//stack size
		int iStackFrameSize = iParamCount + 1 + iLocalDataSize;
		
		int iFuncNameLength = 0;
		fread(&iFuncNameLength, 1, 1, pScriptFile);
		pFuncTable[iCurrFuncIndex].pstrFuncName = (char*)malloc(iFuncNameLength+1);
		fread(pFuncTable[iCurrFuncIndex].pstrFuncName, iFuncNameLength, 1, pScriptFile);
		pFuncTable[iCurrFuncIndex].pstrFuncName[iFuncNameLength] = '\0';
		
		pFuncTable[iCurrFuncIndex].iEntryPoint = iEntryPoint;
		pFuncTable[iCurrFuncIndex].iParamCount = iParamCount;
		pFuncTable[iCurrFuncIndex].iLocalDataSize = iLocalDataSize;
		pFuncTable[iCurrFuncIndex].iStackFrameSize = iStackFrameSize;
		
#ifdef VM_VERBOSE_MODE
		logMssg("reading Function: %s, iEntryPoint: %d  ParamCount: %d  LocalDataSize: %d   StackFramSize;  %d\n", pFuncTable[iCurrFuncIndex].pstrFuncName, iEntryPoint, iParamCount, iLocalDataSize, iStackFrameSize);
#endif
	}
		

	//read host API call table
	fread(&hostAPICallTable.iSize, 4, 1, pScriptFile);
	
	if(!(hostAPICallTable.ppstrCalls = (char**)malloc(hostAPICallTable.iSize * sizeof(char *))) )
		return LOAD_ERROR_OUT_OF_MEMORY;
	
	for(int iCurrCallIndex = 0; iCurrCallIndex < hostAPICallTable.iSize; ++iCurrCallIndex) {
		int iCallLength = 0;
		fread(&iCallLength, 1, 1, pScriptFile);
		
		char* pstrCurrCall = (char*)malloc(iCallLength+1);
		fread(pstrCurrCall, iCallLength, 1, pScriptFile);
		pstrCurrCall[iCallLength] = '\0';
		
		hostAPICallTable.ppstrCalls[iCurrCallIndex] = strupr(pstrCurrCall);
		
#ifdef VM_VERBOSE_MODE
		logMssg("reading hostAPICallTable: %s\n", hostAPICallTable.ppstrCalls[iCurrCallIndex]);
#endif
	}
		
	iCurrState = vm_script_state_loaded;
	resetScript();
	
	iPassedParamSize = 0;

	fclose(pScriptFile);
	return LOAD_SUCCESS;
}

void vm_script::run() {
//	int iExitExec = FALSE;
	
	if((iCurrState == vm_script_state_loaded || iIsRunning) && (iIsMainFuncPresent || iFuncInvokeFromHost || iFuncCallFromHost)) {
		int iCurrInstr = instrStream.iCurrInstr;
		if(iCurrInstr < 0)
			return;
		
		if(iIsPaused) {
			timeb currTime = VM_getCurrentTime();
			if(currTime.time >= lPauseEndTime.time &&
			   currTime.millitm >= lPauseEndTime.millitm) {
				iIsPaused = FALSE;
			} else {
				iIsRunning = TRUE;
				return;
			}
		}
		
		int iOpcode = instrStream.pInstrs[iCurrInstr].iOpcode;
		
#ifdef VM_VERBOSE_MODE
		logMssg("\t    ");
		if(iOpcode < 10)
			logMssg("%d ", iOpcode);
		else
			logMssg("%d ", iOpcode);
		logMssg("%s  ", sInstrs[iOpcode].c_str());
#endif
		
		switch (iOpcode) {
			case INSTR_MOV:
			case INSTR_ADD:
			case INSTR_SUB:
			case INSTR_MUL:
			case INSTR_DIV:
			case INSTR_MOD:
			case INSTR_EXP:
				
			case INSTR_SIN:
			case INSTR_COS:
			case INSTR_TAN:
			case INSTR_SH:
			case INSTR_CH:
			case INSTR_TH:
			case INSTR_LN:
			case INSTR_LG:
			case INSTR_EXPL:

			case INSTR_REC:
			case INSTR_SQRT:
				
			case INSTR_AND:
			case INSTR_OR:
			case INSTR_XOR:
			case INSTR_SHL:
			case INSTR_SHR: {
				VM_Value dest = resolveOpValue(0);
				VM_Value source = resolveOpValue(1);
				
				switch (iOpcode) {
					case INSTR_MOV:
					//	if(resolveOpPntr(0) == resolveOpPntr(1))
					//		break;
						
						copyValue(&dest, source);
						
						break;
						
					case INSTR_ADD:
						if(dest.iType == VM_OP_TYPE_INT) {
							if(source.iType == VM_OP_TYPE_FLOAT) {
								dest.iType = VM_OP_TYPE_FLOAT;
								dest.fFloatLiteral = (float)dest.iIntLiteral;
								dest.fFloatLiteral += source.fFloatLiteral;
							} else
								dest.iIntLiteral +=	coerceValueToInt(source);
						}
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							dest.fFloatLiteral += coerceValueToFloat(source);
						}
						break;
						
					case INSTR_SUB:
						if(dest.iType == VM_OP_TYPE_INT) {
							if(source.iType == VM_OP_TYPE_FLOAT) {
								dest.iType = VM_OP_TYPE_FLOAT;
								dest.fFloatLiteral = (float)dest.iIntLiteral;
								dest.fFloatLiteral -= source.fFloatLiteral;
							} else
								dest.iIntLiteral -= coerceValueToInt(source);
						}						
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							dest.fFloatLiteral -= coerceValueToFloat(source);
						}
						break;
						
					case INSTR_MUL:
						if(dest.iType == VM_OP_TYPE_INT) {
							if(source.iType == VM_OP_TYPE_FLOAT) {
								dest.iType = VM_OP_TYPE_FLOAT;
								dest.fFloatLiteral = (float)dest.iIntLiteral;
								dest.fFloatLiteral *= source.fFloatLiteral;
							} else
								dest.iIntLiteral *= coerceValueToInt(source);
						}						
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							dest.fFloatLiteral *= coerceValueToFloat(source);
						}
						break;
						
					case INSTR_DIV:
						if(dest.iType == VM_OP_TYPE_INT) dest.fFloatLiteral = (float)dest.iInstrIndex;
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral /= coerceValueToFloat(source);
						break;
						
					case INSTR_MOD:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral %= coerceValueToInt(source);
						break;
						
					case INSTR_EXP:
						if(dest.iType == VM_OP_TYPE_INT) {
							if(source.iType == VM_OP_TYPE_FLOAT) {
								dest.iType = VM_OP_TYPE_FLOAT;
								dest.fFloatLiteral = (float)dest.iIntLiteral;
								dest.fFloatLiteral = pow(dest.fFloatLiteral, coerceValueToFloat(source));
							} else
								dest.iIntLiteral = (int)pow(dest.iIntLiteral, coerceValueToFloat(source));
						}						
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							dest.fFloatLiteral = (float)pow(dest.fFloatLiteral, coerceValueToFloat(source));
						}
						break;
						
					case INSTR_SIN:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = sinf(DEGREES_TO_RADIANS(coerceValueToFloat(source)));
						break;
						
					case INSTR_COS:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = cosf(DEGREES_TO_RADIANS(coerceValueToFloat(source)));
						break;
						
					case INSTR_TAN:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = cosf(DEGREES_TO_RADIANS(coerceValueToFloat(source)));
						break;
						
					case INSTR_EXPL:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = exp(coerceValueToFloat(source));
						break;
						
					case INSTR_LG:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = log10(coerceValueToFloat(source));
						break;
						
					case INSTR_LN:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = logf(coerceValueToFloat(source));
						break;

					case INSTR_SQRT:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = sqrtf(coerceValueToFloat(source));
						break;

					case INSTR_REC:
						dest.iType = VM_OP_TYPE_FLOAT;
						if(coerceValueToFloat(source) != 0.f)
							dest.fFloatLiteral = 1.f / coerceValueToFloat(source);
						else
							dest.fFloatLiteral = 0.f;
						break;
					
					case INSTR_SH:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = vm_sh(coerceValueToFloat(source));
						break;
						
					case INSTR_CH:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = vm_ch(coerceValueToFloat(source));
						break;
						
					case INSTR_TH:
						dest.iType = VM_OP_TYPE_FLOAT;
						dest.fFloatLiteral = vm_th(coerceValueToFloat(source));
						break;
						
					case INSTR_AND:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral &= coerceValueToInt(source);
						break;
						
					case INSTR_OR:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral |= coerceValueToInt(source);
						break;
						
					case INSTR_XOR:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral ^= coerceValueToInt(source);
						break;
						
					case INSTR_SHL:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral <<= coerceValueToInt(source);
						break;
						
					case INSTR_SHR:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral >>= coerceValueToInt(source);
						break;
				}
				
				*resolveOpPntr(0) = dest;

#ifdef VM_VERBOSE_MODE
				printOpIndir(0);
				logMssg(", ");
				printOpValue(1);
#endif
				
				break;
			}
				
			case INSTR_NEG:
			case INSTR_NOT:
			case INSTR_INC:
			case INSTR_DEC:
			case INSTR_PRINT: {
			//	int iDestStoreType = getOpType(0);
				
				VM_Value dest = resolveOpValue(0);
				
				switch (iOpcode) {
					case INSTR_NEG:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral = -dest.iIntLiteral;
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							dest.fFloatLiteral = -dest.fFloatLiteral;
						}
						break;
						
					case INSTR_NOT:
						if(dest.iType == VM_OP_TYPE_INT)
							dest.iIntLiteral = ~dest.iIntLiteral;
						break;
						
					case INSTR_INC:
						if(dest.iType == VM_OP_TYPE_INT)
							++dest.iIntLiteral;
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							++dest.fFloatLiteral;
						}
						break;
						
					case INSTR_DEC:
						if(dest.iType == VM_OP_TYPE_INT)
							--dest.iIntLiteral;
						else {
							dest.iType = VM_OP_TYPE_FLOAT;
							--dest.fFloatLiteral;
						}
						break;
						
					case INSTR_PRINT:
#ifdef VM_VERBOSE_MODE
						logMssg(" [ Print Value: %s ] ", resolveOpAsString(0));
#else
						cout<<resolveOpAsString(0)<<endl;
#endif
						break;
				}
				
				if(iOpcode != INSTR_PRINT)
					*resolveOpPntr(0) = dest;

#ifdef VM_VERBOSE_MODE				
				printOpIndir(0);
#endif
				break;
			}
				
			case INSTR_CONCAT: {
				VM_Value dest = resolveOpValue(0);
				
				char* pstrSourceString = resolveOpAsString(1);
				
				int iPrevType = dest.iType;
				if(dest.iType != VM_OP_TYPE_STRING) {
					string sTempString;
					if(dest.iType == VM_OP_TYPE_INT) {
						sTempString = int_to_strVM(dest.iIntLiteral);
						dest.iType = VM_OP_TYPE_STRING;
						dest.pstrStringLiteral = (char*)malloc(sTempString.size());
						strcpy(dest.pstrStringLiteral, sTempString.c_str());
					}
					else if(dest.iType == VM_OP_TYPE_FLOAT) {
						sTempString = fp_to_strVM(dest.fFloatLiteral);
						dest.iType = VM_OP_TYPE_STRING;
						dest.pstrStringLiteral = (char*)malloc(sTempString.size());
						strcpy(dest.pstrStringLiteral, sTempString.c_str());
					}					
					else {
						printWarningMssg("Invalid concat a string to a nonstring value, break");
						break;
					}
				}
				
				int iNewStringLength = strlen(dest.pstrStringLiteral) + strlen(pstrSourceString);
				char* pstrNewString = (char*)malloc(iNewStringLength+1);
				
				strcpy(pstrNewString, dest.pstrStringLiteral);
				strcat(pstrNewString, pstrSourceString);
				
				free(dest.pstrStringLiteral);
				dest.pstrStringLiteral = pstrNewString;
				
				*resolveOpPntr(0) = dest;
				
				// recover previous type
				if(iPrevType != VM_OP_TYPE_STRING) {
					if(iPrevType == VM_OP_TYPE_INT) {
						VM_Value* pVal = resolveOpPntr(0);
						int iResult = atoi(pVal->pstrStringLiteral);
						free(pVal->pstrStringLiteral);
						
						pVal->iType = VM_OP_TYPE_INT;
						pVal->iIntLiteral = iResult;
					} else if(iPrevType == VM_OP_TYPE_FLOAT) {
						VM_Value* pVal = resolveOpPntr(0);
						float fResult = atof(pVal->pstrStringLiteral);
						free(pVal->pstrStringLiteral);
						
						pVal->iType = VM_OP_TYPE_FLOAT;
						pVal->fFloatLiteral = fResult;
					} else
						printWarningMssg("Unable to recover previous concat variable type, retain OP_TYPE_STRING");
				}
				
#ifdef VM_VERBOSE_MODE
				printOpIndir(0);
				logMssg(", ");
				printOpValue(1);
#endif
				break;
			}
				
			case INSTR_GETCHAR: {
				
				break;
			}
				
			case INSTR_SETCHAR: {
				break;
			}
				
			case INSTR_JMP: {
				int iTargetIndex = resolveOpAsInstrIndex(0);
				
				instrStream.iCurrInstr = iTargetIndex;
				
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
#endif
				break;
			}
				
			case INSTR_JE:
			case INSTR_JNE:
			case INSTR_JG:
			case INSTR_JL:
			case INSTR_JGE:
			case INSTR_JLE: {
				VM_Value op0 = resolveOpValue(0);
				VM_Value op1 = resolveOpValue(1);
				
				int iTargetIndex = resolveOpAsInstrIndex(2);
				
				int iJump = FALSE;
				switch (iOpcode) {
					case INSTR_JE: {
						switch (op0.iType) {
							case VM_OP_TYPE_INT:
								if(op0.iIntLiteral == coerceValueToInt(op1))
									iJump = TRUE;
								break;
								
							case VM_OP_TYPE_FLOAT:
								if(op0.fFloatLiteral == coerceValueToFloat(op1))
									iJump = TRUE;
								break;
								
							case VM_OP_TYPE_STRING:
								if(strcmp(op0.pstrStringLiteral, op1.pstrStringLiteral) == 0)
									iJump = TRUE;
								break;
								
							default:
								break;
						}
						break;
					}
						
					case INSTR_JNE: {
						switch (op0.iType) {
							case VM_OP_TYPE_INT:
								if(op0.iIntLiteral != coerceValueToInt(op1))
									iJump = TRUE;
								break;
								
							case VM_OP_TYPE_FLOAT:
								if(op0.fFloatLiteral != coerceValueToFloat(op1))
									iJump = TRUE;
								break;
								
							case VM_OP_TYPE_STRING:
								if(strcmp(op0.pstrStringLiteral, op1.pstrStringLiteral) != 0)
									iJump = TRUE;
								break;
								
							default:
								break;
						}
						break;
					}
						
					case INSTR_JG:
						if(op0.iType == VM_OP_TYPE_INT) {
							if(op0.iIntLiteral > coerceValueToInt(op1))
								iJump = TRUE;
						} else {
							if(op0.fFloatLiteral > coerceValueToFloat(op1))
								iJump = TRUE;
						}
						break;
						
					case INSTR_JL:
						if(op0.iType == VM_OP_TYPE_INT) {
							if(op0.iIntLiteral < coerceValueToInt(op1))
								iJump = TRUE;
						} else {
							if(op0.fFloatLiteral < coerceValueToFloat(op1))
								iJump = TRUE;
						}
						break;
						
					case INSTR_JGE:
						if(op0.iType == VM_OP_TYPE_INT) {
							if(op0.iIntLiteral >= coerceValueToInt(op1))
								iJump = TRUE;
						} else {
							if(op0.fFloatLiteral >= coerceValueToFloat(op1))
								iJump = TRUE;
						}
						break;
						
					case INSTR_JLE:
						if(op0.iType == VM_OP_TYPE_INT) {
							if(op0.iIntLiteral <= coerceValueToInt(op1))
								iJump = TRUE;
						} else {
							if(op0.fFloatLiteral <= coerceValueToFloat(op1))
								iJump = TRUE;
						}
						break;

				}
				
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
				logMssg(", ");
				printOpValue(1);
				logMssg(", ");
				printOpValue(2);
				logMssg(" ");
#endif
				
				if(iJump) {
					instrStream.iCurrInstr = iTargetIndex;
#ifdef VM_VERBOSE_MODE
					logMssg("(True)");
#endif
				}
#ifdef VM_VERBOSE_MODE
				else
					logMssg("(False)");
#endif
				break;
			}
				
			case INSTR_RANDOMINT:
			case INSTR_RANDOMFLOAT: {
				VM_Value* dest = resolveOpPntr(0);
				VM_Value start = resolveOpValue(1);
				VM_Value end = resolveOpValue(2);
				
				if(iOpcode == INSTR_RANDOMINT) {
					dest->iType = VM_OP_TYPE_INT;
				//	VM_Random_Seed(0);
					dest->iIntLiteral = VM_Random_Int(coerceValueToInt(start), coerceValueToInt(end));
				} else if(iOpcode == INSTR_RANDOMFLOAT) {
					dest->iType = VM_OP_TYPE_FLOAT;
				//	VM_Random_Seed(0);
					dest->fFloatLiteral = VM_Random_Float(coerceValueToFloat(start), coerceValueToFloat(end));
				}
				
			
			//	*resolveOpPntr(0) = dest;
								
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
				logMssg(", ");
				printOpValue(1);
				logMssg(", ");
				printOpValue(2);
				logMssg(" ");
#endif
				
				break;
			}
				
			case INSTR_PUSH: {
				VM_Value Source = resolveOpValue(0);
				
				push(Source);
				
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
#endif
				break;
			}
				
			case INSTR_POP: {
				*resolveOpPntr(0) = pop();
				
#ifdef VM_VERBOSE_MODE
				printOpIndir(0);
#endif
				break;
			}
								
			case INSTR_PUSHHOSTPARAM: {
				VM_Value Source = resolveOpValue(0);
				
				pushHostAPIParam(Source);
				
#ifdef VM_VERBOSE_MODE
				printOpIndir(0);
#endif
				break;
			}
				
			case INSTR_CALL: {
				int iFuncIndex = resolveOpAsFuncIndex(0);
				
				++instrStream.iCurrInstr;
				
				callFunc(iFuncIndex);
				break;
			}
				
			case INSTR_RET: {
				VM_Value funcIndex = pop();
				if(funcIndex.iType == VM_OP_TYPE_STACK_BASE_MARKER) {
					if(iFuncCallFromHost)
						iFuncCallFromHost = FALSE;
				}
				
				if(iFuncInvokeFromHost)
					iFuncInvokeFromHost = FALSE;
					
				int iErrorCode;
				VM_Func currFunc = getFunc(funcIndex.iFuncIndex, iErrorCode);
				int iFrameIndex = funcIndex.iOffsetIndex;
				
				VM_Value returnAddr = getStackValue(stack.iTopIndex-(currFunc.iLocalDataSize+1));
				
				popFrame(currFunc.iStackFrameSize);
				
				stack.iFrameIndex = iFrameIndex;
				
				instrStream.iCurrInstr = returnAddr.iInstrIndex;
				
#ifdef VM_VERBOSE_MODE
				logMssg("%d", returnAddr.iInstrIndex);
#endif
				break;
			}
				
			case INSTR_CALLHOST: {
				iHostParamSize = 0;
				iIsReturnedFromHost = FALSE;
				
				VM_Value val = resolveOpValue(0);
				char* pstrFuncName = val.pstrStringLiteral;
				if(pstrFuncName) {
					if(x_vm_hostAPIFuncTable::getInstance()->callHostAPIFunc(pstrFuncName, iHostID, iCurrThread) == -1)
						cout<<"vm_script::callHostAPIFunc: Error calling host API function: "<<pstrFuncName<<endl;
					
					if(!iIsReturnedFromHost)
						returnIntFromHost(iHostParamSize, 0);
				} else {
					cout<<"vm_script::callHostAPIFunc: Error calling host API function: "<<pstrFuncName<<endl;
					iIsReturnedFromHost = TRUE;
				}
				
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
#endif
				break;
			}
				
			case INSTR_PAUSE: {
				if(iIsPaused) {
					VM_addmillistm(lPauseEndTime, resolveOpAsInt(0));
#ifdef VM_VERBOSE_MODE
					cout<<"\n\n Pause Time: "<<lPauseEndTime.time<<" millitm: "<<lPauseEndTime.millitm<<endl;
#endif
					return;
				}
				
				int iPauseDuration = resolveOpAsInt(0);
				
				lPauseEndTime = VM_getCurrentTime();
				VM_addmillistm(lPauseEndTime, iPauseDuration);
#ifdef VM_VERBOSE_MODE
				cout<<"\n **Pause Time: "<<lPauseEndTime.time<<" millitm: "<<lPauseEndTime.millitm<<endl;
#endif
				
				iIsPaused = TRUE;
				
#ifdef VM_VERBOSE_MODE
				cout<<" **Curr Time: "<<VM_getCurrentTime().time<<" millitm: "<<VM_getCurrentTime().millitm<<endl;				
				printOpValue(0);
#endif
				break;
			}

			case INSTR_EXIT: {
				VM_Value exitCode = resolveOpValue(0);
				
				iIsRunning = FALSE;
							
#ifdef VM_VERBOSE_MODE
				printOpValue(0);
#endif
				
				instrStream.iCurrInstr = -1;

				break;
			}
		}
		
#ifdef VM_VERBOSE_MODE
		logMssg("\n");
#endif
		
		if(iCurrInstr == instrStream.iCurrInstr) {
			++instrStream.iCurrInstr;
		}
	} else if(iFuncCallFromHost) {
		iFuncCallFromHost = 0;
	}
}

void vm_script::resetScript() {
	if(pFuncTable) {
		if(iIsMainFuncPresent)
			instrStream.iCurrInstr = pFuncTable[iMainFuncIndex].iEntryPoint;
	}
	
	stack.iTopIndex = 0;
	stack.iFrameIndex = 0;
	
	for(int iCurrElmntIndex = 0; iCurrElmntIndex < stack.iSize; ++iCurrElmntIndex)
		stack.pElmnts[iCurrElmntIndex].iType = VM_OP_TYPE_NULL;
	
	iIsPaused = FALSE;
	iIsRunning = FALSE;
	iFuncInvokeFromHost = FALSE;
	iFuncCallFromHost = FALSE;
	
	pushFrame(iGlobalDataSize);
	pushFrame(pFuncTable[iMainFuncIndex].iStackFrameSize+1);
}
	
	
int vm_script::getOpType(int iOpIndex) {
	return instrStream.pInstrs[instrStream.iCurrInstr].pOpList[iOpIndex].iType;
}

int vm_script::resolveOpStackIndex(int iOpIndex) {
	int iCurrInstr = instrStream.iCurrInstr;
	
	VM_Value opValue = instrStream.pInstrs[iCurrInstr].pOpList[iOpIndex];
	
	switch (opValue.iType) {
		case VM_OP_TYPE_ABS_STACK_INDEX:
			return opValue.iStackIndex;
			break;
			
		case VM_OP_TYPE_REL_STACK_INDEX: {
			int iBaseIndex = opValue.iStackIndex;
			int iOffsetIndex = opValue.iOffsetIndex;
			
			VM_Value stackValue = getStackValue(iOffsetIndex);
			if(iBaseIndex > 0)
				return iBaseIndex + coerceValueToInt(stackValue);
			else
				return iBaseIndex - coerceValueToInt(stackValue);
			break;
		}
			
		default:
			return 0;
			break;
	}
}

VM_Value vm_script::resolveOpValue(int iOpIndex) {
	int iCurrInstr = instrStream.iCurrInstr;
	
	VM_Value opValue = instrStream.pInstrs[iCurrInstr].pOpList[iOpIndex];
	
	switch (opValue.iType) {
		case VM_OP_TYPE_ABS_STACK_INDEX:
		case VM_OP_TYPE_REL_STACK_INDEX: {
			int iAbsIndex = resolveOpStackIndex(iOpIndex);
			return getStackValue(iAbsIndex);
			break;
		}
			
		case VM_OP_TYPE_REG:
			return _retVal;
			break;
			
		default:
			return opValue;
			break;
	}
}

int vm_script::resolveOpType(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return opValue.iType;
}

int vm_script::resolveOpAsInt(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return coerceValueToInt(opValue);
}

float vm_script::resolveOpAsFloat(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return coerceValueToFloat(opValue);
}

char* vm_script::resolveOpAsString(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	char* pstrString = coerceValueToString(opValue);
	return pstrString;
}

int vm_script::resolveOpAsStackIndex(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return opValue.iInstrIndex;
}

int vm_script::resolveOpAsInstrIndex(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return opValue.iInstrIndex;
}

int vm_script::resolveOpAsFuncIndex(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return opValue.iFuncIndex;
}

char* vm_script::resolveOpAsHostAPICall(int iOpIndex) {
	VM_Value opValue = resolveOpValue(iOpIndex);
	return getHostAPICall(opValue.iHostAPICallIndex);
}

VM_Value* vm_script::resolveOpPntr(int iOpIndex) {
	int iIndirMethod = getOpType(iOpIndex);
	switch (iIndirMethod) {
		case VM_OP_TYPE_ABS_STACK_INDEX:
		case VM_OP_TYPE_REL_STACK_INDEX: {
			int iStackIndex = resolveOpStackIndex(iOpIndex);
			return &stack.pElmnts[resolveStackIndex(iStackIndex)];
			break;
		}
			
		case VM_OP_TYPE_REG:
			return &_retVal;
			break;
			
		default:
			return 0;
			break;
	}
}

VM_Value vm_script::getStackValue(int iStackIndex) {
	if(iStackIndex >= stack.iSize)
		return stack.pElmnts[0];
	return stack.pElmnts[resolveStackIndex(iStackIndex)];
}

void vm_script::setStackValue(int iStackIndex, VM_Value val) {
	if(iStackIndex >= stack.iSize)
		return;
	stack.pElmnts[resolveStackIndex(iStackIndex)] = val;
}

int vm_script::resolveStackIndex(int iIndex) {
	if(iIndex < 0)
		return (iIndex+stack.iFrameIndex);
	return iIndex;
}

int vm_script::coerceValueToInt(const VM_Value& val) {
	switch (val.iType) {
		case VM_OP_TYPE_INT:
			return val.iIntLiteral;
			break;
			
		case VM_OP_TYPE_FLOAT:
			return (int)val.fFloatLiteral;
			break;
			
		case VM_OP_TYPE_STRING:
			return atoi(val.pstrStringLiteral);
			break;
			
		default:
			return 0;
			break;
	}
}

float vm_script::coerceValueToFloat(const VM_Value& val) {
	switch (val.iType) {
		case VM_OP_TYPE_INT:
			return (float)val.iIntLiteral;
			break;
			
		case VM_OP_TYPE_FLOAT:
			return val.fFloatLiteral;
			break;
			
		case VM_OP_TYPE_STRING:
			return atof(val.pstrStringLiteral);
			break;
			
		default:
			return 0.f;
			break;
	}
}

char* vm_script::coerceValueToString(const VM_Value& val) {
	char* pstrCoercion;
	if(val.iType != VM_OP_TYPE_STRING)
		pstrCoercion = (char*)malloc(MAX_STRING_COERCION_SIZE+1);
				
	switch (val.iType) {
		case VM_OP_TYPE_INT:
			pstrCoercion = int_to_strVM(val.iIntLiteral);
			return pstrCoercion;
			break;
			
		case VM_OP_TYPE_FLOAT:
			sprintf(pstrCoercion, "%f", val.fFloatLiteral);
			return pstrCoercion;
			break;
			
		case VM_OP_TYPE_STRING:
			return val.pstrStringLiteral;
			break;
			
		default:
			free(pstrCoercion);
			return NULL;
			break;
	}
}

void vm_script::exitOnError(const char* pErrorMssg) {
	exitWithMssgBox(0, "Script VM Error: %s\n", pErrorMssg);
}

void vm_script::push(VM_Value val) {
	if(stack.iTopIndex < stack.iSize)
		stack.pElmnts[stack.iTopIndex] = val;
	else {
		exitOnError("Stack fulled");
	}
	++stack.iTopIndex;
}

VM_Value vm_script::pop() {
	VM_Value val;
	if(stack.iTopIndex > 0) {
		--stack.iTopIndex;
	
		copyValue(&val, stack.pElmnts[stack.iTopIndex]);
	} else {
		exitOnError("Empty stack");
	}
	return val;
}

void vm_script::pushFrame(int iSize) {
	stack.iTopIndex += iSize;
	
	stack.iFrameIndex = stack.iTopIndex;
}

void vm_script::popFrame(int iSize) {
	stack.iTopIndex -= iSize;
}

void vm_script::pushParam(const VM_Value& val) {
	paramStack.push_back(val);
}

VM_Value vm_script::popParam() {
	VM_Value val = paramStack[paramStack.size()-1];
	paramStack.erase(paramStack.end()-1);
	return val;
}

void vm_script::pushHostAPIParam(const VM_Value& val) {
	if(val.iType == VM_OP_TYPE_REG) {
		hostAPIParamStack.push_back(_retVal);
		logMssg("Retval pushed");
	}
	else
		hostAPIParamStack.push_back(val);
}

void vm_script::copyValue(VM_Value* pDest, VM_Value source) {
	if(pDest->iType == VM_OP_TYPE_STRING)
		free(pDest->pstrStringLiteral);
	
	pDest->iType = source.iType;
	*pDest = source;
	
	if(source.iType == VM_OP_TYPE_STRING) {
		pDest->pstrStringLiteral = (char*)malloc(strlen(source.pstrStringLiteral)+1);
		strcpy(pDest->pstrStringLiteral, source.pstrStringLiteral);
	}

}

VM_Func vm_script::getFunc(int iIndex, int& iErrorCode) {
	if(iIndex < iFuncTableSize && iIndex >= 0)
		return pFuncTable[iIndex];
	iErrorCode = 1;
	return pFuncTable[0];
}

char* vm_script::getHostAPICall(int iIndex) {
	if(iIndex < hostAPICallTable.iSize && iIndex >= 0)
		return hostAPICallTable.ppstrCalls[iIndex];
	return NULL;
}

void vm_script::printOpIndir(int iOpIndex) {
	int iIndirMethod = getOpType(iOpIndex);
	
	switch (iIndirMethod) {
		case VM_OP_TYPE_REG:
			logMssg("_RetVal");
			break;
			
		case VM_OP_TYPE_ABS_STACK_INDEX:
		case VM_OP_TYPE_REL_STACK_INDEX: {
			int iStackIndex = resolveOpStackIndex(iOpIndex);
			logMssg("[ %d ]", iStackIndex);
			break;
		}
	}
}

void vm_script::printOpValue(int iOpIndex) {
	VM_Value op = resolveOpValue(iOpIndex);
	
	switch (op.iType) {
		case VM_OP_TYPE_NULL:
			logMssg("Null ");
			break;
			
		case VM_OP_TYPE_INT:
			logMssg("%d ", op.iIntLiteral);
			break;
			
		case VM_OP_TYPE_FLOAT:
			logMssg("%f ", op.fFloatLiteral);
			break;
			
		case VM_OP_TYPE_STRING:
			logMssg("\"%s \"", op.pstrStringLiteral);
			break;
			
		case VM_OP_TYPE_INSTR_INDEX:
			logMssg("%d ", op.iInstrIndex);
			break;
			
		case VM_OP_TYPE_HOST_API_CALL_INDEX: {
			char* pstrHostAPICall = resolveOpAsHostAPICall(iOpIndex);
			logMssg("%s ", pstrHostAPICall);
			break;
		}
	}
}


int vm_script::getParamAsInt(int iParamIndex) {
	if(hostAPIParamStack.size()-(iParamIndex+1) < 0) {
		logMssg("vm_script::getParamAsInt: invalid param index");
		return 0;
	}
	VM_Value param = hostAPIParamStack[hostAPIParamStack.size()-(iParamIndex+1)];
	++iHostParamSize;
	
	return coerceValueToInt(param);
}

float vm_script::getParamAsFloat(int iParamIndex) {
	if(hostAPIParamStack.size()-(iParamIndex+1) < 0) {
		logMssg("vm_script::getParamAsFloat: invalid param index");
		return 0.f;
	}
	VM_Value param = hostAPIParamStack[hostAPIParamStack.size()-(iParamIndex+1)];	
	++iHostParamSize;

	return coerceValueToFloat(param);
}

char* vm_script::getParamAsString(int iParamIndex) {
	if(hostAPIParamStack.size()-(iParamIndex+1) < 0) {
		logMssg("vm_script::getParamAsString: invalid param index");
		return 0;
	}
	VM_Value param = hostAPIParamStack[hostAPIParamStack.size()-(iParamIndex+1)];
	++iHostParamSize;

	return coerceValueToString(param);
}

void vm_script::returnIntFromHost(int iParamCount, int iInt) {
	if(hostAPIParamStack.size() < iParamCount)
		logMssg("vm::returnFloatFromHost: host API parameter table size error");
	hostAPIParamStack.clear();
	
	_retVal.iType = VM_OP_TYPE_INT;
	_retVal.iIntLiteral = iInt;
	
	iIsReturnedFromHost = TRUE;
}

void vm_script::returnFloatFromHost(int iParamCount, float fFloat) {
	if(hostAPIParamStack.size() < iParamCount)
		logMssg("vm::returnFloatFromHost: host API parameter table size error");
	hostAPIParamStack.clear();	
	
	_retVal.iType = VM_OP_TYPE_FLOAT;
	_retVal.fFloatLiteral = fFloat;
	
	iIsReturnedFromHost = TRUE;
}

void vm_script::returnStringFromHost(int iParamCount, char* pstrString) {
	if(hostAPIParamStack.size() < iParamCount)
		cout<<"vm::returnFloatFromHost: host API parameter table size error"<<endl;
	hostAPIParamStack.clear();
	
	VM_Value returnValue;
	returnValue.iType = VM_OP_TYPE_STRING;
	returnValue.pstrStringLiteral = pstrString;
	
	copyValue(&_retVal, returnValue);
	
	iIsReturnedFromHost = TRUE;
}

int vm_script::callFunc(int iIndex) {
	int iErrorCode = 0;
	VM_Func dest = getFunc(iIndex, iErrorCode);
	if(iErrorCode) return FALSE;
	
	VM_Value returnAddr;
	returnAddr.iInstrIndex = instrStream.iCurrInstr;
	push(returnAddr);
		
	VM_Value funcIndex;
	funcIndex.iFuncIndex = iIndex;
	funcIndex.iOffsetIndex = stack.iFrameIndex;
	
	pushFrame(dest.iLocalDataSize+1);

	setStackValue(stack.iTopIndex-1, funcIndex);
	
	instrStream.iCurrInstr = dest.iEntryPoint;
	
#ifdef VM_VERBOSE_MODE
	logMssg("$$[%d]$$", instrStream.iCurrInstr);
	
	logMssg(" %d ( Entry Point: %d, Frame Size: %d )", iIndex, dest.iEntryPoint, dest.iStackFrameSize);
#endif
	return TRUE;
}

void vm_script::passParamAsInt(int iInt) {
	VM_Value param;
	param.iType = VM_OP_TYPE_INT;
	param.iIntLiteral = iInt;
	push(param);
	
	++iPassedParamSize;
}

void vm_script::passParamAsFloat(float fFloat) {
	VM_Value param;
	param.iType = VM_OP_TYPE_FLOAT;
	param.fFloatLiteral = fFloat;
	push(param);
	
	++iPassedParamSize;
}

void vm_script::passParamAsString(char* pstrString) {
	VM_Value param;
	param.iType = VM_OP_TYPE_STRING;
	param.pstrStringLiteral = (char*)malloc(strlen(pstrString)+1);
	strcpy(param.pstrStringLiteral, pstrString);
	push(param);
	
	++iPassedParamSize;
}

void vm_script::passParam(VM_Value param) {
	push(param);
	
	++iPassedParamSize;
}

int vm_script::getFuncIndexByName(const char* pstrFuncName) {
	if(!pstrFuncName) return -1;

	char* pstrFuncNameUpper = (char*)malloc(strlen(pstrFuncName)+1);
	strcpy(pstrFuncNameUpper, pstrFuncName);
	strupr(pstrFuncNameUpper);

	for(int iFuncIndex = 0; iFuncIndex < iFuncTableSize; ++iFuncIndex) {
		if(strcmp(pstrFuncNameUpper, pFuncTable[iFuncIndex].pstrFuncName) == 0) {
			free(pstrFuncNameUpper);
			return iFuncIndex;
		}
	}
	free(pstrFuncNameUpper);
	return -1;
}

int vm_script::isFuncAvailable(const char* pstrFuncName) {
	if(getFuncIndexByName(pstrFuncName) == -1)
		return FALSE;
	return TRUE;
}
		
int vm_script::invokeScriptFunc(const char* pstrFuncName) {
	int iFuncIndex = getFuncIndexByName(pstrFuncName);
	if(iFuncIndex == -1) {
		cout<<"Cannot find function: %s in sscript"<<pstrFuncName<<endl;
		return FALSE;
	}
	
	iFuncInvokeFromHost = TRUE;
	return callFunc(iFuncIndex);
}

int vm_script::getReturnValueAsInt() {
	return coerceValueToInt(_retVal);
}

float vm_script::getReturnValueAsFloat() {
	return coerceValueToFloat(_retVal);
}

char* vm_script::getReturnValueAsString() {
	char* pstrCoercion;
	if(_retVal.iType != VM_OP_TYPE_STRING)
		pstrCoercion = (char*)malloc(MAX_STRING_COERCION_SIZE+1);
	else
		pstrCoercion = (char*)malloc(strlen(_retVal.pstrStringLiteral)+1);
	
	switch(_retVal.iType) {
		case VM_OP_TYPE_INT:
			pstrCoercion = int_to_strVM(_retVal.iIntLiteral);
			return pstrCoercion;
			break;
			
		case VM_OP_TYPE_FLOAT:
			sprintf(pstrCoercion, "%f", _retVal.fFloatLiteral);
			return pstrCoercion;
			break;
			
		case VM_OP_TYPE_STRING:
			strcpy(pstrCoercion, _retVal.pstrStringLiteral);
			return pstrCoercion;
			break;
			
		default:
			free(pstrCoercion);
			return NULL;
			break;
	}
	return 0;
}

int vm_script::callScriptFunc(const char* pstrFuncName) {
	int iFuncIndex = getFuncIndexByName(pstrFuncName);

	if(iFuncIndex == -1) {
		return FALSE;
	}
	
#ifdef VM_VERBOSE_MODE
	cout<<"Passed Param Size: "<<iPassedParamSize<<endl;
	cout<<"Func Param Size:	  "<<dest.iParamCount<<endl;
#endif
	int iErrorCode = 0;
	VM_Func func = getFunc(iFuncIndex, iErrorCode);
	if(iErrorCode) {
		cout<<"vm_script::callScriptFunc: invalid func name"<<pstrFuncName<<endl;
		return FALSE;
	}
	if(func.iParamCount != -1) {
		if(iPassedParamSize != func.iParamCount) {
			cout<<"vm_script::callFunc: invalid param size - param size not equal"<<endl;
			return FALSE;
		}
	}
	iPassedParamSize = 0;
	
	
	iFuncCallFromHost = TRUE;
	callFunc(iFuncIndex);
	
	VM_Value stackBase = getStackValue(stack.iTopIndex-1);
	stackBase.iType = VM_OP_TYPE_STACK_BASE_MARKER;
	setStackValue(stack.iTopIndex-1, stackBase);

	while(iFuncCallFromHost) {
		run();
	}
	return TRUE;
}

void vm_script::printWarningMssg(const string& sErrorMssg) {
	cout<<"Warning: "<<sErrorMssg<<endl;
}

void vm_script::registerHostAPIFunc(const char* name, HostAPIFuncPntr func, int _iHostID) {
	VM_HostAPIFunc APIFunc;
	APIFunc.pstrName			= (char*)malloc(strlen(name)+1);
	strcpy(APIFunc.pstrName, name);
	strupr(APIFunc.pstrName);
	APIFunc.fnFunc				= func;
	APIFunc.iIsActive			= TRUE;

	iHostID = _iHostID;

	
	hostAPIFuncTable.push_back(APIFunc);
}

int vm_script::_GetFuncParamCount(int iFuncIndex) {
	if(iFuncIndex < 0 || iFuncIndex >= iFuncTableSize) return 0;
	int iErrorCode = 0;
	return getFunc(iFuncIndex, iErrorCode).iParamCount;
}

int vm_script::_GetFuncParamCount(const char* pstrFuncName) {
	return _GetFuncParamCount(getFuncIndexByName(pstrFuncName));
}

int vm_script::_GetFuncEntryPoint(int iFuncIndex) {
	if(iFuncIndex < 0 || iFuncIndex >= iFuncTableSize) return -1;
	int iErrorCode = 0;
	return getFunc(iFuncIndex, iErrorCode).iEntryPoint;
}

int vm_script::_GetFuncEntryPoint(const char* pstrFuncName) {
	return _GetFuncEntryPoint(getFuncIndexByName(pstrFuncName));
}

int vm_script::_GetFuncLocalDataSize(int iFuncIndex) {
	if(iFuncIndex < 0 || iFuncIndex >= iFuncTableSize) return 0;
	int iErrorCode = 0;
	return getFunc(iFuncIndex, iErrorCode).iLocalDataSize;
}

int vm_script::_GetFuncLocalDataSize(const char* pstrFuncName) {
	return _GetFuncLocalDataSize(getFuncIndexByName(pstrFuncName));
}

void vm_script::passParamList(const vector<VM_Value>& vParamList) {
	for(size_t i=0; i<vParamList.size(); ++i) {
		passParam(vParamList[i]);
	}
}

void vm_script::callScriptFuncWithParamList(const char* pstrFuncName, const vector<VM_Value>& vParamList) {
	passParamList(vParamList);
	callScriptFunc(pstrFuncName);
}

void vm_script::invokeScriptFuncWithParamList(const char* pstrFuncName, const vector<VM_Value>& vParamList) {
	passParamList(vParamList);
	invokeScriptFunc(pstrFuncName);
}

vector<VM_Value> vm_script::buildParamListFromCurrentParamStack() {
	return hostAPIParamStack;
}

vector<VM_Value> vm_script::buildRangedParamListFromCurrentParamStack(int iStart, int iEnd) {
	vector<VM_Value> vParamList;
	int begin = iStart; if(begin < 0) begin = 0; if(begin >= hostAPIParamStack.size()) return vParamList;
	int end = iEnd; if(end > hostAPIParamStack.size()) end = hostAPIParamStack.size();

	if(begin > end) swap(begin, end);
	
	for(int i=begin; i<end; ++i) {
		vParamList.push_back(hostAPIParamStack[i]);
	}
	return vParamList;
}
