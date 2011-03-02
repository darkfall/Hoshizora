/*
 *  vm_defines.h
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

#ifndef _VM_DEFINES_H_
#define _VM_DEFINES_H_

#include <sys/types.h>
#include <sys/timeb.h>

#include <string>

using std::string;

#define LOLISCRIPT_ID_STRING "LOLI"

#define DEFAULT_STACKSIZE 1024

#define MAX_STRING_COERCION_SIZE	256

#define MAX_HOST_API_SIZE	128

#define MAX_SCRIPT_THREAD_COUNT 256

#define ASSEMBLY_FILE_VERSION_MAJOR 0
#define ASSEMBLY_FILE_VERSION_MINOR 7

//#define VM_VERBOSE_MODE

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef void (* HostAPIFuncPntr) (int iThreadIndex);

#define HOSTAPI_GLOBAL			-1

inline char* strupr(char* a) {
	char *ret = a;
	while(*a != '\0') {
		if(islower(*a))
			*a = toupper(*a);
		++a;
    }
	return ret;
}

inline timeb VM_getCurrentTime() {
	timeb ctime;
	ftime(&ctime);
	return ctime;
}

inline void VM_addmillistm(timeb& time, int millitm) {
	time.millitm += millitm;
	if(time.millitm >= 1000) {
		time.time += (int)(time.millitm / 1000);
		time.millitm %= 1000;
	}
}

static unsigned int vm_g_seed2 = 0;

inline void VM_Random_Seed(int seed) {
	time_t lTime;
	time(&lTime);
	if(!seed) vm_g_seed2=lTime;
	else vm_g_seed2=seed;
}

inline int VM_Random_Int(int min, int max) {
	vm_g_seed2=214013*vm_g_seed2+2531011;
	return min+(vm_g_seed2 ^ vm_g_seed2>>15)%(max-min+1);
}

inline float VM_Random_Float(float min, float max) {	
	vm_g_seed2=214013*vm_g_seed2+2531011;
	return min+(vm_g_seed2>>16)*(1.0f/65535.0f)*(max-min);
}

static string sInstrs[] = { "mov",
							"add", "sub", "mul", "div", "mod", "exp", "neg", "inc", "dec",
							"and", "or",  "xor", "not", "shl", "shr",
							"concat", "getchar", "setchar",
							"jmp", "je", "jne", "jg", "jl", "jge", "jle",
							"push", "pop",
							"call", "ret", "callhost",
							"pause", "exit", "print",
							"sin", "cos", "tan", "ln", "lg", "expl", "rec", "sh", "ch", "th", "sqrt",
							"pushparam", "pushhparam", "param",
							"randomInt", "randomFloat"};


#endif