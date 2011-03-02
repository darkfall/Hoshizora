/*
 *  error.h
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
#ifndef ASM_ERROR_H
#define ASM_ERROR_H

#define ERROR_MSSG_LOCAL_SETSTACKSIZE			"Local SetStackSize"
#define ERROR_MSSG_MULTIPLE_SETSTACKSIZES		"Multiple SetStackSizes"
#define ERROR_MSSG_INVALID_STACK_SIZE			"Invalid stack size"
#define ERROR_MSSG_NESTED_FUNC					"Nested function"
#define	ERROR_MSSG_IDENT_EXPECTED				"Identifier expected"
#define ERROR_MSSG_FUNC_REDEFINITION			"Redefinitioned function"
#define ERROR_MSSG_IDENT_REDEFINITION			"Redefinitioned Identifier"
#define ERROR_MSSG_INVALID_ARRAY_SIZE			"Invalid array size"
#define ERROR_MSSG_INVALID_INSTR				"Invalid instruction"
#define ERROR_MSSG_INVALID_INDENT				"Invalid indentifier"
#define ERROR_MSSG_GLOBAL_LINE_LABEL			"Global label identifier"
#define ERROR_MSSG_LINE_LABEL_REDEFINITION		"Redefinitioned label"
#define ERROR_MSSG_INVALID_INPUT				"Invalid source input"
#define ERROR_MSSG_INVALID_OP					"Invalid operator"
#define ERROR_MSSG_GLOBAL_INSTR					"Global instruction"
#define ERROR_MSSG_INVALID_STRING				"Invalid string"
#define ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED	"Array not indexed"
#define ERROR_MSSG_UNDEFINED_IDENT				"Undefined identification"
#define ERROR_MSSG_INVALID_ARRAY				"Invalid array"
#define ERROR_MSSG_INVALID_ARRAY_INDEX			"Invalid array index"
#define ERROR_MSSG_UNDEFINED_LINE_LABEL			"Undefined line label"
#define ERROR_MSSG_UNDEFINED_FUNC				"Undefined function"
#define ERROR_MSSG_GLOBAL_PARAM					"Global parameter"
#define ERROR_MSSG_MAIN_PARAM					"Main function parameter"
#define ERROR_MSSG_INVALID_PRIORITY				"Invalid SetPriority parameter"
#define ERROR_MSSG_MULTIPLE_SETPRIORITY			"Multiple SetPriority found"
#define ERROR_MSSG_LOCAL_PRIORITY				"Local SetPriority call"


#endif