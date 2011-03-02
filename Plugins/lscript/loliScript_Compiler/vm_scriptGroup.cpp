/*
 *  vm_scriptGroup.cpp
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

// #include "stdafx.h"

#include "vm_scriptGroup.h"
#include "vm_defines.h"
#include <string>

#include "lsLog.h"
vm_scriptGroup::vm_scriptGroup(int id) {
	iCurrScriptThread = 0;
	iCurrScriptThreadCount = 0;

	scripts.reserve(MAX_SCRIPT_THREAD_COUNT);

	iScriptGroupID = id;
}

vm_scriptGroup::~vm_scriptGroup() {
	scripts.clear();
}

int vm_scriptGroup::findFreeThread() {
	for(int iCurrThread = 0; iCurrThread < static_cast<int>(scripts.size()); ++iCurrThread) {
		if(scripts[iCurrThread].iCurrState != vm_script_state_loaded)
			return iCurrThread;
	}
	vm_script newScript;
	scripts.push_back(newScript);
	return scripts.size()-1;
}

int vm_scriptGroup::loadScript(const char* pstrScript) {
	iCurrScriptThread = findFreeThread();
	if(iCurrScriptThread == -1)
		return -1;
	++iCurrScriptThreadCount;
	
	scripts[iCurrScriptThread].iCurrThread = iCurrScriptThread;
	if(scripts[iCurrScriptThread].loadScript(pstrScript) == LOAD_SUCCESS) {
		scripts[iCurrScriptThread].iHostID = iScriptGroupID;
		return iCurrScriptThreadCount-1;
	}
	logMssg("Invalid Script File\n");
	return -1;
}

int vm_scriptGroup::isThreadActive(int iThreadIndex) {
	if(iThreadIndex >= 0 && iThreadIndex < static_cast<int>(scripts.size()))
		if(scripts[iThreadIndex].iCurrState == vm_script_state_loaded)
			return TRUE;
	return FALSE;
}

void vm_scriptGroup::pauseScript(int iThreadIndex, int iPauseDuration) {
	if(!isThreadActive(iThreadIndex)) {
		logMssg("Invalid script thread index");
		return;
	}
	if(!scripts[iThreadIndex].iIsRunning) {
		logMssg("Script Index: %d is not running", iThreadIndex);
		return;
	}
	scripts[iThreadIndex].setPauseTime(iThreadIndex);
}
		

void vm_scriptGroup::unloadScript(int iScriptThreadIndex) {
	if(!isThreadActive(iScriptThreadIndex)) {
		logMssg("Invalid script thread index");
		return;
	}
	if(scripts[iScriptThreadIndex].iCurrState == vm_script_state_loaded) {
		scripts[iScriptThreadIndex].releaseScript();
		scripts[iScriptThreadIndex].iCurrState = vm_script_state_loaded;
	}
}

void vm_scriptGroup::resetScript(int iScriptThreadIndex) {
	if(!isThreadActive(iScriptThreadIndex)) {
		logMssg("Invalid script thread index");
		return;
	}
	if(scripts[iScriptThreadIndex].iCurrState == vm_script_state_loaded)
		scripts[iScriptThreadIndex].resetScript();
}
	
void vm_scriptGroup::runScript(int iThreadIndex) {
	if(!isThreadActive(iThreadIndex)) {
		logMssg("Invalid script thread index");
		return;
	}
	scripts[iThreadIndex].runScript();
}

void vm_scriptGroup::stopScript(int iThreadIndex) {
	if(!isThreadActive(iThreadIndex)) {
		logMssg("Invalid script thread index");
		return;
	}
	if(!scripts[iThreadIndex].iIsRunning) {
		logMssg("Script Index: %d is not running", iThreadIndex);
		return;
	}
	scripts[iThreadIndex].iIsRunning = FALSE;
}

int vm_scriptGroup::getParamAsInt(int iThreadIndex, int iParamIndex) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].getParamAsInt(iParamIndex);
	return 0;
}

float vm_scriptGroup::getParamAsFloat(int iThreadIndex, int iParamIndex) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].getParamAsFloat(iParamIndex);
	return 0;
}

char* vm_scriptGroup::getParamAsString(int iThreadIndex, int iParamIndex) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].getParamAsString(iParamIndex);
	return 0;
}

void vm_scriptGroup::returnIntFromHost(int iThreadIndex, int iParamCount, int iInt) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].returnIntFromHost(iParamCount, iInt);
}

void vm_scriptGroup::returnFloatFromHost(int iThreadIndex, int iParamCount, float fFloat) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].returnFloatFromHost(iParamCount, fFloat);
}

void vm_scriptGroup::returnStringFromHost(int iThreadIndex, int iParamCount, char* pstrString) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].returnStringFromHost(iParamCount, pstrString);
}

void vm_scriptGroup::returnFromHost(int iThreadIndex, int iParamCount) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].returnIntFromHost(iParamCount, 0);
}

void vm_scriptGroup::run() {
	timeb iCurrTime = VM_getCurrentTime();
	
	timeb timeSliceTime = iCurrTime;
		
	timeb startt = VM_getCurrentTime();
	timeb endt = VM_getCurrentTime();
	
	while(TRUE) {	
		if(!scripts[iCurrScriptThread].iIsRunning)
			--iCurrScriptThreadCount;
		if(iCurrScriptThreadCount == 0) {
			endt = VM_getCurrentTime();
			break;
		}
		
		if(iCurrTime.time >= timeSliceTime.time && iCurrTime.millitm >= timeSliceTime.millitm) {
			VM_Random_Seed(0);
			break;
			while(TRUE) {
				++iCurrScriptThread;
				if(iCurrScriptThread >= static_cast<int>(scripts.size()))
					iCurrScriptThread = 0;
				if(scripts[iCurrScriptThread].iCurrState == vm_script_state_loaded &&
				   scripts[iCurrScriptThread].iIsRunning)
					break;
			}
			
			timeSliceTime = VM_getCurrentTime();
			VM_addmillistm(timeSliceTime, scripts[iCurrScriptThread].iTimeSliceDur);
			
#ifdef MODE_VERBOSE
			cout<<"**************************\nRunning Script Thead: "<<iCurrScriptThread<<" total Thread: "<<iCurrScriptThreadCount<<"  time slice duration: "<<scripts[iCurrScriptThread].iTimeSliceDur<<endl<<"******************************"<<endl;
#endif
		}
		
		
		scripts[iCurrScriptThread].run();
	}
}

void vm_scriptGroup::registerHostAPIFunc(int iThreadIndex, const char* pstrName, HostAPIFuncPntr fnFunc) {
	if(iThreadIndex != -1 && isThreadActive(iThreadIndex)) {
		scripts[iThreadIndex].registerHostAPIFunc(pstrName, fnFunc, iScriptGroupID);
	} else {
		for(int i=0; i < static_cast<int>(scripts.size()); ++i)
			if(isThreadActive(i))
				scripts[i].registerHostAPIFunc(pstrName, fnFunc, iScriptGroupID);
	}
}

int vm_scriptGroup::invokeScriptFunc(int iThreadIndex, const char* pstrFuncName) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].invokeScriptFunc(pstrFuncName);
	return FALSE;
}

int vm_scriptGroup::callScriptFunc(int iThreadIndex, const char* pstrFuncName) {
	if(isThreadActive(iThreadIndex)) 
		return scripts[iThreadIndex].callScriptFunc(pstrFuncName);
	return FALSE;
}

int vm_scriptGroup::isFuncAvailable(int iThreadIndex, const char* pstrFuncName) {
	if(isThreadActive(iThreadIndex))
		return scripts[iThreadIndex].isFuncAvailable(pstrFuncName);
	return FALSE;
}		

void vm_scriptGroup::passParamAsInt(int iThreadIndex, int iParam) {
	if(isThreadActive(iThreadIndex)) {
		scripts[iThreadIndex].passParamAsInt(iParam);
	}
}

void vm_scriptGroup::passParamAsFloat(int iThreadIndex, float fParam) {
	if(isThreadActive(iThreadIndex)) {
		scripts[iThreadIndex].passParamAsFloat(fParam);
	}
}

void vm_scriptGroup::passParamAsString(int iThreadIndex, char* pstrParam) {
	if(isThreadActive(iThreadIndex)) {
		scripts[iThreadIndex].passParamAsString(pstrParam);
	}
}

int vm_scriptGroup::getReturnValueAsInt(int iThreadIndex) {
	if(isThreadActive(iThreadIndex)) {
		return scripts[iThreadIndex].getReturnValueAsInt();
	}
	return 0;
}

float vm_scriptGroup::getReturnValueAsFloat(int iThreadIndex) {
	if(isThreadActive(iThreadIndex)) {
		return scripts[iThreadIndex].getReturnValueAsFloat();
	}
	return 0.f;
}

char* vm_scriptGroup::getReturnValueAsString(int iThreadIndex) {
	if(isThreadActive(iThreadIndex)) {
		return scripts[iThreadIndex].getReturnValueAsString();
	}
	return 0;
}

void vm_scriptGroup::sendMessage(int iThreadIndex, const char* pstrMessage) {
	if(isThreadActive(iThreadIndex)) {
		scripts[iThreadIndex].callScriptFunc(pstrMessage);
	}
}

void vm_scriptGroup::sendMessageToAll(const char* pstrMessage) {
	for(size_t i=0; i<scripts.size(); ++i) {
		if(isThreadActive(i)) {
			scripts[i].callScriptFunc(pstrMessage);
		}
	}
}