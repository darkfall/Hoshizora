/*
 *  vm_scripts.h
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

#include <string>
#include <iostream>

#include <cstdio>
#include <cstring>


#include "vm_structs.h"

#include <vector>

using namespace std;

#define LOAD_SUCCESS						0
#define LOAD_ERROR_INVALID_FILE				1
#define LOAD_ERROR_INVALID_ID				2
#define LOAD_ERROR_UNSUPPORTED_VERSION		3
#define LOAD_ERROR_INVALID_STACKSIZE		4
#define LOAD_ERROR_OUT_OF_MEMORY			5
#define LOAD_ERROR_OUT_OF_THREADS			6

#define PRIORITY_USER	0
#define PRIORITY_LOW	1
#define PRIORITY_MED	2
#define PRIORITY_HIGH	3

#define THREAD_TIMESLICE_HIGH    60
#define THREAD_TIMESLICE_MEDIUM  40
#define THREAD_TIMESLICE_LOW	 20

typedef enum vm_script_state {
	vm_script_state_loaded,
	vm_script_state_loadFailed,
	vm_script_state_notLoad,
};

class vm_script {
public:	
	friend class vm_scriptGroup;

	vm_script(const char* pstrFile);
	vm_script();
	
	~vm_script();
	
	int		loadScript(const char* pstrFile);
	
	void	resetScript();
	void	releaseScript();
	
	void    setPauseTime		(int iPauseTime);
	
	int		getParamAsInt		(int iParamIndex);
	float	getParamAsFloat		(int iParamIndex);
	char*	getParamAsString	(int iParamIndex);
	
	int		invokeScriptFunc	(const char* pstrFuncName);   // Synchronize
	int		callScriptFunc		(const char* pstrFuncName);	  // Asynchronous
	int		callHostFunc		(const char* pstrFuncName);
	int     isFuncAvailable		(const char* pstrFuncName);
	
	void	passParamAsInt		(int iParam);
	void    passParamAsFloat	(float fParam);
	void	passParamAsString	(char* pstrParam);
	
	void	returnIntFromHost		(int iParamCount, int iInt);
	void	returnFloatFromHost		(int iParamCount,float fFloat);
	void	returnStringFromHost	(int iParamCount, char* pstrString);
	
	int		getReturnValueAsInt		();
	float   getReturnValueAsFloat	();
	char*   getReturnValueAsString	();
		
	void	run();
	void    runScript();
	
private:
	int		getOpType				(int iOpIndex);
	
	inline int		resolveOpType			(int iOpIndex);
	inline VM_Value	resolveOpValue			(int iOpIndex);
	inline int		resolveOpStackIndex		(int iOpIndex);
	
	inline int		resolveOpAsInt			(int iOpIndex);
	inline float	resolveOpAsFloat		(int iOpIndex);
	inline char*	resolveOpAsString		(int iOpIndex);
	inline int		resolveOpAsStackIndex	(int iOpIndex);
	inline int		resolveOpAsInstrIndex	(int iOpIndex);
	inline int		resolveOpAsFuncIndex	(int iOpIndex);
	inline char*	resolveOpAsHostAPICall	(int iOpIndex);
	inline int		resolveStackIndex		(int iStackIndex);
	
	inline VM_Value*	resolveOpPntr		(int iOpIndex);
	
	inline VM_Value	getStackValue			(int iStackIndex);
	inline void		setStackValue			(int iStackIndex, VM_Value val);
	
	VM_Value	pop						();
	void		push					(VM_Value val);
	void		popFrame				(int iSize);
	void		pushFrame				(int iSize);
	
	VM_Value	popParam				();
	void		pushParam				(const VM_Value& val);
	
	void		pushHostAPIParam		(const VM_Value& val);
	
	inline int		coerceValueToInt		(VM_Value val);
	inline float	coerceValueToFloat		(VM_Value val);
	inline char*	coerceValueToString		(VM_Value val);

	inline void	copyValue				(VM_Value* pDest, VM_Value source);
	
	VM_Func	getFunc					(int iIndex);
	char*   getHostAPICall			(int iIndex);
	
	void	printOpIndir			(int iOpIndex);
	void	printOpValue			(int iOpIndex);
	
	int		callFunc				(int iIndex);
	int		getFuncIndexByName		(const char* pstrFuncName);
	
	void	exitOnError				(const char* pErrorMssg);
	void	printWarningMssg		(const string& sErrorMssg);
	
	void	registerHostAPIFunc		(const char* funcName, HostAPIFuncPntr func);
		
	int					iIsRunning;
	int					iIsPaused;
	vm_script_state		iCurrState;
	int					iTimeSliceDur;
	
	int					iIsReturnedFromHost;
	int					iHostParamSize;
	
	int					iCurrThread;
	
	int					iGlobalDataSize;
	
	int					iIsMainFuncPresent;
	int					iMainFuncIndex;
	
	int					iFuncInvokeFromHost;
	int					iFuncCallFromHost;
	
	timeb				lPauseEndTime;
	
	VM_Value			_retVal;
	
	VM_InstrStream		instrStream;
	VM_RuntimeStack		stack;
	
	int					iFuncTableSize;
	VM_Func*			pFuncTable;
	
	VM_HostAPICallTable	hostAPICallTable;
	
	char**				ppstrStringTable;
	
	vector<VM_HostAPIFunc> hostAPIFuncTable;
	
	vector<VM_Value>	paramStack;
	vector<VM_Value>	hostAPIParamStack;
};
