/*
 *  vm_scriptGroup.h
 *  loliScript_VM
 *
 *  Created by griffin clare on 09年12月27日.
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

#include "vm_scripts.h"

#include <vector>

using std::vector;

class vm_scriptGroup {
public:
	vm_scriptGroup();
	~vm_scriptGroup();
		
	int loadScript		(const char* pstrScript);
	
	void unloadScript	(int iScriptThreadIndex);
	void resetScript	(int iScriptThreadIndex);
	
	void runScript		(int iScriptThreadIndex);
	void stopScript		(int iScriptThreadIndex);
	
	void pauseScript	(int iThreadIndex, int iPauseDuration);
	
	void registerHostAPIFunc	(int iThreadIndex, const char* pstrName, HostAPIFuncPntr fnFunc);
	
	int  isFuncAvailable		(int iThreadIndex, const char* pstrFuncName);
	int	 invokeScriptFunc		(int iThreadIndex, const char* pstrFuncName);
	int  callScriptFunc			(int iThreadIndex, const char* pstrFuncName);
		
	int   getParamAsInt			(int iThreadIndex, int iParamIndex);
	float getParamAsFloat		(int iThreadIndex, int iParamIndex);
	char* getParamAsString		(int iThreadIndex, int iParamIndex);
	
	void  passParamAsInt		(int iThreadIndex, int iParam);
	void  passParamAsFloat		(int iThreadIndex, float fParam);
	void  passParamAsString		(int iThreadIndex, char* pstrParam);
	
	void returnIntFromHost		(int iThreadIndex, int iParamCount, int iInt);
	void returnFloatFromHost	(int iThreadIndex, int iParamCount, float fFloat);
	void returnStringFromHost	(int iThreadIndex, int iParamCount, char* pstrString);
	void returnFromHost			(int iThreadIndex, int iParamCount);
	
	int   getReturnValueAsInt		(int iThreadIndex);
	float getReturnValueAsFloat		(int iThreadIndex);
	char* getReturnValueAsString	(int iThreadIndex);
	
	void run();
	
private:	
	int findFreeThread();
	
	int isThreadActive(int iThreadIndex);
	
	int iCurrScriptThread;
	
	int iCurrScriptThreadCount;
	
	timeb CurrThreadActiveTime;
	
	vector<vm_script> scripts;
};