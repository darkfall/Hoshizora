/*
 *  funTable.cpp
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

#include "compiler_funcTable.h"

_LSH_ICodeInstr::~_LSH_ICodeInstr() {
	for(size_t i=0; i<vOpList.size(); ++i)
		if(vOpList[i])
			delete vOpList[i];
	vOpList.clear();
}

_LSH_FuncNode::~_LSH_FuncNode() {
	for(size_t i=0; i<vICodeStream.size(); ++i)
		if(vICodeStream[i])
			delete vICodeStream[i];
	vICodeStream.clear();
}

LSH_FuncTable::LSH_FuncTable() {
	iCurrJumpTargetIndex = 0;
}

LSH_FuncTable::~LSH_FuncTable() {
	for(size_t i=0; i<funcTable.size(); ++i)
		if(funcTable[i])
			delete funcTable[i];
	funcTable.clear();
}

LSH_FuncNode* LSH_FuncTable::getFuncByIndex(int iIndex) {
	if(iIndex > funcTable.size())
		return NULL;
	return funcTable[iIndex];
	
}

LSH_FuncNode* LSH_FuncTable::getFuncByName(const char* pstrName) {
	vector<LSH_FuncNode*>::iterator p = funcTable.begin();
	
	for(p; p != funcTable.end(); ++p) {
		if(strcasecmp((*p)->sFuncName.c_str(), pstrName) == 0)
			return (*p);
	}
	return NULL;
}	

int LSH_FuncTable::addFunc(const char* pstrName, _FUNCTION_TYPES ftType) {
	if(getFuncByName(pstrName))
		return -1;
	
	LSH_FuncNode* pNewFunc		= new LSH_FuncNode;
	pNewFunc->sFuncName			= pstrName;
	pNewFunc->iIndex			= funcTable.size();
	pNewFunc->ftType			= ftType;
	pNewFunc->iParamCount		= 0;
	pNewFunc->vICodeStream.clear();
	
	funcTable.push_back(pNewFunc);
	return funcTable.size()-1;
}

void LSH_FuncTable::setFuncParamCount(int iIndex, int iParamCount) {
	LSH_FuncNode* pFunc = getFuncByIndex(iIndex);
	pFunc->iParamCount = iParamCount;
}

LSH_ICodeNode* LSH_FuncTable::getICodeNodeByImpIndex(int iFuncIndex, int iInstrIndex) {
	LSH_FuncNode* pFunc = getFuncByIndex(iFuncIndex);
	if(!pFunc)
		return NULL;
	if(iInstrIndex > pFunc->vICodeStream.size())
		return NULL;
	
	return pFunc->vICodeStream[iInstrIndex];
}

void LSH_FuncTable::addICodeSourceLine(int iFuncIndex, const char* pstrSourceLine) {
	LSH_FuncNode* pFunc = getFuncByIndex(iFuncIndex);
	if(!pFunc)
		return;
	
	LSH_ICodeNode* pSourceLineNode	= new LSH_ICodeNode;
	pSourceLineNode->iType			= ICODE_NODE_SOURCE_LINE;
	pSourceLineNode->pstrSourceLine = (char*)malloc(strlen(pstrSourceLine)+1);
	strcpy(pSourceLineNode->pstrSourceLine, pstrSourceLine);
	
	pFunc->vICodeStream.push_back(pSourceLineNode);
}

int LSH_FuncTable::addICodeInstr(int iFuncIndex, int iOpcode) {
	LSH_FuncNode* pFunc = getFuncByIndex(iFuncIndex);
	if(!pFunc)
		return -1;
	
	LSH_ICodeNode* pInstrNode = new LSH_ICodeNode;
	pInstrNode->iType = ICODE_NODE_INSTR;
	pInstrNode->instr.iOpcode = iOpcode;
	
	pFunc->vICodeStream.push_back(pInstrNode);
	return pFunc->vICodeStream.size()-1;
}

LSH_Op* LSH_FuncTable::getICodeOpByIndex(LSH_ICodeNode* pInstr, int iOpIndex) {
	if(iOpIndex > pInstr->instr.vOpList.size())
		return NULL;
	
	return pInstr->instr.vOpList[iOpIndex];
}

void LSH_FuncTable::addICodeOp(int iFuncIndex, int iInstrIndex, LSH_Op oValue) {
	LSH_ICodeNode* pInstr = getICodeNodeByImpIndex(iFuncIndex, iInstrIndex);
	if(!pInstr)
		return;
	
	LSH_Op* pValue = new LSH_Op;
	memcpy(pValue, &oValue, sizeof(LSH_Op));
	
	pInstr->instr.vOpList.push_back(pValue);
}

void LSH_FuncTable::addIntICodeOp(int iFuncIndex, int iInstrIndex, int iValue) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_INT;
	oValue.iIntLiteral = iValue;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addFloatICodeOp(int iFuncIndex, int iInstrIndex, float fValue) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_FLOAT;
	oValue.fFloatLiteral = fValue;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}
	
void LSH_FuncTable::addStringICodeOp(int iFuncIndex, int iInstrIndex, int iStringIndex) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_STRING_INDEX;
	oValue.iStringIndex = iStringIndex;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addVarICodeOp(int iFuncIndex, int iInstrIndex, int iSymbolIndex) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_VAR;
	oValue.iSymbolIndex = iSymbolIndex;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addArrayIndexAbsICodeOp(int iFuncIndex, int iInstrIndex, int iArraySymbolIndex, int iOffset) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_ABS_ARRAY_INDEX;
	oValue.iSymbolIndex = iArraySymbolIndex;
	oValue.iOffset = iOffset;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addArrayIndexVarICodeOp(int iFuncIndex, int iInstrIndex, int iArraySymbolIndex, int iOffsetSymbolIndex) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_VAR_ARRAY_INDEX;
	oValue.iSymbolIndex = iArraySymbolIndex;
	oValue.iOffsetSymbolIndex = iOffsetSymbolIndex;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addFuncICodeOp(int iFuncIndex, int iInstrIndex, int iOpFuncIndex) {
	LSH_Op oValue;
	
	oValue.iType = OP_TYPE_FUNC_INDEX;
	oValue.iFuncIndex = iOpFuncIndex;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addRegICodeOp(int iFuncIndex, int iInstrIndex, int iRegCode) {
	LSH_Op oValue;

	oValue.iType = OP_TYPE_REG;
	oValue.iRegCode = iRegCode;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

void LSH_FuncTable::addJumpTargetICodeOp(int iFuncIndex, int iInstrIndex, int iTargetIndex) {
	LSH_Op oValue;

	oValue.iType = OP_TYPE_JUMP_TARGET_INDEX;
	oValue.iJumpTargetIndex = iTargetIndex;
	
	addICodeOp(iFuncIndex, iInstrIndex, oValue);
}

int LSH_FuncTable::getNextJumpTargetIndex() {
	return iCurrJumpTargetIndex++;
}

void LSH_FuncTable::addICodeJumpTarget(int iFuncIndex, int iTargetIndex) {
	LSH_FuncNode* pFunc = getFuncByIndex(iFuncIndex);
	if(!pFunc)
		return;
	
	LSH_ICodeNode* pSourceLineNode = new LSH_ICodeNode;
	pSourceLineNode->iType = ICODE_NODE_JUMP_TARGET;
	pSourceLineNode->iJumpTargetIndex = iTargetIndex;
	
	pFunc->vICodeStream.push_back(pSourceLineNode);
}


int LSH_FuncTable::coerceICodeOpToInt(LSH_Op* op) {
	switch (op->iType) {
		case OP_TYPE_INT:
			return op->iIntLiteral;
			break;
			
		case OP_TYPE_FLOAT:
			return (int)op->fFloatLiteral;
			break;
		
		default:
			return 0;
			break;
	}
}

float LSH_FuncTable::coerceICodeOpToFloat(LSH_Op* op) {
	switch (op->iType) {
		case OP_TYPE_INT:
			return (float)op->iIntLiteral;
			break;
			
		case OP_TYPE_FLOAT:
			return op->fFloatLiteral;
			break;
			
		default:
			return 0.f;
			break;
	}
}
