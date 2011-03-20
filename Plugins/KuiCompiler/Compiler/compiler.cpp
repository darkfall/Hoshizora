/*
 *  compiler.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月9日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */
#include "compiler.h"

#include "compiler_error.h"
#include "compiler_global.h"

#include <fstream>

//#include "../../KuiWin32/headers/xLog.h"

//#include "../../KuiWin32/headers/xCommon.h"

//#include "../../Wrapper.h"

#define TAB_STOP_WIDTH 8

#define TEMP_VAR_T0 "_T0"
#define TEMP_VAR_T1 "_T1"
#define TEMP_VAR_T2 "_T2"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

LSH_Compiler::LSH_Compiler() {
	pOutputFile = 0;
	pParser = new LSH_Parser;
}

LSH_Compiler::~LSH_Compiler() {
	if(pParser)
		delete pParser;
}

int LSH_Compiler::loadSourceFile(const char* pstrSourceFile) {
	includedFile.sFileName = pstrSourceFile;	
	includedFile.vIncludeFileList.clear();


	if(pParser)
		delete pParser;
	pParser = new LSH_Parser;


	if(doLoadSourceFile(includedFile.sFileName) != SOURCE_FILE_LOAD_SUCCESS)
		return 0;
	
	sSourceFileName = pstrSourceFile;

	return TRUE;
}

SOURCE_FILE_LOAD_ERROR LSH_Compiler::doLoadSourceFile(const string& sFileName) {
	ifstream srSourceFile(sFileName.c_str());
	if(!srSourceFile.is_open()) {
		string sErrorMssg("Cannot open file: ");
		sErrorMssg += sFileName;
		Compiler_ExitOnError(sErrorMssg.c_str());
		return SOURCE_FILE_LOAD_ERROR_IO_ERROR;
	}
	
	includedFile.vIncludeFileList.push_back(sFileName);
	
	string buffer;
	while(getline(srSourceFile, buffer)) {
		if(buffer.size() > 0) {
			if(buffer[0] == '#') {
				RSRVD_IDENT_TYPE identType = getReservedIdentType(buffer);
				
				if(identType == RSRVD_IDENT_TYPE_INCLUDE) {
					string sIncludeFileName = buffer.substr(buffer.find(' ')+1, buffer.size());
					if(sIncludeFileName[0] != '"')
						Compiler_ExitOnError(("\" expected\nCodeLine: "+buffer).c_str());
					
					sIncludeFileName.erase(sIncludeFileName.begin());
					
					int iEndRefIndex = sIncludeFileName.find('"');
					if(iEndRefIndex == string::npos)
						Compiler_ExitOnError(("\" expected\nCodeLine: "+buffer).c_str());
					
					sIncludeFileName.erase(sIncludeFileName.begin()+iEndRefIndex);
					
					int iIsFileIncluded = FALSE;
					for(size_t iCurrIncludedFileIndex = 0; iCurrIncludedFileIndex < includedFile.vIncludeFileList.size(); ++iCurrIncludedFileIndex) {
						if(includedFile.vIncludeFileList[iCurrIncludedFileIndex].compare(sIncludeFileName) == 0) {
							iIsFileIncluded = TRUE;
							break;
						}
					}
					if(iIsFileIncluded) {
						string sErrorMssg("Duplicate include file: ");
						sErrorMssg += sIncludeFileName;
						Compiler_ExitOnError(sErrorMssg.c_str());
					}
					
					doLoadSourceFile(sIncludeFileName.c_str());
				}
				else
					pParser->vSourceCode.push_back(buffer);
			}
			else
				pParser->vSourceCode.push_back(buffer);
			
		}
	}
	srSourceFile.close();
	return SOURCE_FILE_LOAD_SUCCESS;
}

RSRVD_IDENT_TYPE LSH_Compiler::getReservedIdentType(const string& sSourceLine) {
	string sIdent = sSourceLine.substr(1, sSourceLine.find(' ')-1);
	
	if(sIdent.compare("include") == 0)
		return RSRVD_IDENT_TYPE_INCLUDE;
	else if(sIdent.compare("define") == 0)
		return RSRVD_IDENT_TYPE_DEFINE;
	
	return RSRVD_IDENT_TYPE_UNKNOWN;
}

int LSH_Compiler::compileSourceFile() {
	//Add temp variables , _T0, _T1, _T2
	pParser->iTempVar0SymbolIndex = pParser->symbolTable.addSymbol(TEMP_VAR_T0, 1, SCOPE_GLOBAL, SYMBOL_TYPE_VAR);
	pParser->iTempVar1SymbolIndex = pParser->symbolTable.addSymbol(TEMP_VAR_T1, 1, SCOPE_GLOBAL, SYMBOL_TYPE_VAR);
	pParser->iTempVar2SymbolIndex = pParser->symbolTable.addSymbol(TEMP_VAR_T2, 1, SCOPE_GLOBAL, SYMBOL_TYPE_VAR);
	
	preprocessSourceFile();
	
	
	if(pParser->parseSourceCode())
		return TRUE;
							 
	return FALSE;
}

bool isSizeZero(string& str) {
	return (str.size() == 0);
}

void LSH_Compiler::preprocessSourceFile() {
	int iInBlockComment = FALSE;
	int iInString		= FALSE;
	
	for(int p = 0; p < static_cast<int>(pParser->vSourceCode.size()); ++p) {
		for(int iCurrCharIndex = 0; iCurrCharIndex <static_cast<int>(pParser->vSourceCode[p].size()); ++iCurrCharIndex) {
			if(pParser->vSourceCode[p][iCurrCharIndex] == '"') {
				if(iInString)
					iInString = FALSE;
				else
					iInString = TRUE;
			}
			
			if(iCurrCharIndex > static_cast<int>(pParser->vSourceCode[p].size()))
				break;
			
			
			if(pParser->vSourceCode[p][iCurrCharIndex] == '/' && pParser->vSourceCode[p][iCurrCharIndex+1] == '/' && !iInString && !iInBlockComment) {
				pParser->vSourceCode[p].erase(iCurrCharIndex, pParser->vSourceCode[p].size());
				break;
			}
			
			if(pParser->vSourceCode[p][iCurrCharIndex] == '/' && pParser->vSourceCode[p][iCurrCharIndex+1] == '*' && !iInString && !iInBlockComment) {
				iInBlockComment = TRUE;
				if(pParser->vSourceCode[p].find("*/") != string::npos) {
					pParser->vSourceCode[p].erase(0, pParser->vSourceCode[p].size());
					iInBlockComment = FALSE;
				}
			}
			
			if(pParser->vSourceCode[p][iCurrCharIndex] == '*' && pParser->vSourceCode[p][iCurrCharIndex+1] == '/' && iInBlockComment) {
				pParser->vSourceCode[p].erase(iCurrCharIndex, 2);
				iInBlockComment = FALSE;
			}
			
			if(iInBlockComment) {
				pParser->vSourceCode[p].erase(iCurrCharIndex);
			}
		}
	}
	pParser->vSourceCode.erase(remove_if(pParser->vSourceCode.begin(), pParser->vSourceCode.end(), isSizeZero), pParser->vSourceCode.end());
}


string sInstrs[] = 
{
	"mov",
	"add", "sub", "mul", "div", "mod", "exp", "neg", "inc", "dec",
	"and", "or",  "xor", "not", "shl", "shr",
	"concat", "getchar", "setchar",
	"jmp", "je", "jne", "jg", "jl", "jge", "jle",
	"push", "pop",
	"call", "ret", "callhost",
	"pause", "exit", "print",
	"sin", "cos", "tan", "ln", "lg", "expl", "rec", "sh", "ch", "th", "sqrt",
	"pushparam", "pushhparam", "param",
	"randomInt", "randomFloat"
};

void LSH_Compiler::emitHeader() {
	if(!pOutputFile)
		return;
	
#ifndef WIN32
	time_t currTime;
	currTime = time(NULL);
	struct tm* pCurrTime;
	pCurrTime = localtime(&currTime);
#else
	string currTime = getCurrTimeInString();
#endif
	
	fprintf(pOutputFile, "; %s\n\n", sOutputFileName.c_str());
	fprintf(pOutputFile, "; Source File: %s\n",	sSourceFileName.c_str());
	fprintf(pOutputFile, "; LSH Compiler By GriffinBu(GameMaster&Studio Symphonie), Global Version: %d.%d\n", LSH_VERSION_MAJOR, LSH_VERSION_MINOR);

#ifndef WIN32
	fprintf(pOutputFile, "; Time: %s\n\n", asctime(pCurrTime));
#else
	fprintf(pOutputFile, "; Time: %s\n\n", currTime.c_str());
#endif
}

void LSH_Compiler::emitDirectives() {
	int iAddNewLine = FALSE;
	
	if(pParser->scriptHeader.iStackSize) {
		fprintf(pOutputFile, "\tSetStackSize %d\n", pParser->scriptHeader.iStackSize);
		iAddNewLine = TRUE;
	}
	
	if(pParser->scriptHeader.iPriorityType != PRIORITY_NONE) {
		fprintf(pOutputFile, "\tSetPriority ");
		switch (pParser->scriptHeader.iPriorityType) {
			case PRIORITY_LOW:
				fprintf(pOutputFile, PRIORITY_LOW_KEYWORD);
				break;
				
			case PRIORITY_MED:
				fprintf(pOutputFile, PRIORITY_MED_KEYWORD);
				break;
				
			case PRIORITY_HIGH:
				fprintf(pOutputFile, PRIORITY_HIGH_KEYWORD);
				break;
				
			case PRIORITY_USER:
				fprintf(pOutputFile, "%d", pParser->scriptHeader.iUserPriority);
		}
		fprintf(pOutputFile, "\n");
		
		iAddNewLine = TRUE;
	}
	
	if(iAddNewLine)
		fprintf(pOutputFile, "\n");
}

void LSH_Compiler::emitScopeSymbols(int iScope, int iType) {
	int iAddNewLine = FALSE;
	
	LSH_SymbolNode* pCurrSymbol;
	for(int iCurrSymbolIndex = 0; iCurrSymbolIndex < pParser->symbolTable.size(); ++iCurrSymbolIndex) {
		pCurrSymbol = pParser->symbolTable.getSymbolByIndex(iCurrSymbolIndex);
		
		if(pCurrSymbol->iScope == iScope && pCurrSymbol->iType == iType) {
			fprintf(pOutputFile, "\t");
			if(iScope != SCOPE_GLOBAL)
				fprintf(pOutputFile, "\t");
						
			if(pCurrSymbol->iType == SYMBOL_TYPE_PARAM)
				fprintf(pOutputFile, "Param %s", pCurrSymbol->sIdent.c_str());
			
			if(pCurrSymbol->iType == SYMBOL_TYPE_VAR || pCurrSymbol->iType == SYMBOL_TYPE_STRUCT) {
				fprintf(pOutputFile, "Var %s", pCurrSymbol->sIdent.c_str());
				
				if(pCurrSymbol->iSize > 1)
					fprintf(pOutputFile, "[ %d ]", pCurrSymbol->iSize);
			}
			
			fprintf(pOutputFile, "\n");
			iAddNewLine = TRUE;
		}
	}
	if(iAddNewLine)
		fprintf(pOutputFile, "\n");
}

void LSH_Compiler::emitFunc(LSH_FuncNode* pFunc) {
	fprintf(pOutputFile, "\tFunc %s\n", pFunc->sFuncName.c_str());
	fprintf(pOutputFile, "\t{\n");
	
	emitScopeSymbols(pFunc->iIndex, SYMBOL_TYPE_PARAM);
	emitScopeSymbols(pFunc->iIndex, SYMBOL_TYPE_VAR);
	emitScopeSymbols(pFunc->iIndex, SYMBOL_TYPE_STRUCT);
	
	if(pFunc->vICodeStream.size() > 0) {
		int iIsFirstSourceLine = TRUE;
		
		for(int iCurrInstrIndex = 0; iCurrInstrIndex < static_cast<int>(pFunc->vICodeStream.size()); ++iCurrInstrIndex) {
			LSH_ICodeNode* pCurrNode = pParser->funcTable.getICodeNodeByImpIndex(pFunc->iIndex, iCurrInstrIndex);
			
			switch (pCurrNode->iType) {
				case ICODE_NODE_SOURCE_LINE: {
					char* pstrSourceLine = pCurrNode->pstrSourceLine;
					
					int iLastCharIndex = strlen(pstrSourceLine)-1;
					if(pstrSourceLine[iLastCharIndex] == '\n')
						pstrSourceLine[iLastCharIndex] = '\0';
					
					if(!iIsFirstSourceLine)
						fprintf(pOutputFile, "\n");
					
					fprintf(pOutputFile, "\t\t; %s\n\n", pstrSourceLine);
					break;
				}
					
				case ICODE_NODE_INSTR: {
					fprintf(pOutputFile, "\t\t%s", sInstrs[pCurrNode->instr.iOpcode].c_str());
					
					int iOpCount = pCurrNode->instr.vOpList.size();
					
					if(iOpCount) {
						fprintf(pOutputFile, "\t");
						
						if(sInstrs[pCurrNode->instr.iOpcode].size() < TAB_STOP_WIDTH)
							fprintf(pOutputFile, "\t");
					}
					
					for(int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
						LSH_Op* pOp = pParser->funcTable.getICodeOpByIndex(pCurrNode, iCurrOpIndex);
						
						switch (pOp->iType) {
							case OP_TYPE_INT:
								fprintf(pOutputFile, "%d", pOp->iIntLiteral);
								break;
								
							case OP_TYPE_FLOAT:
								fprintf(pOutputFile, "%f", pOp->fFloatLiteral);
								break;
								
							case OP_TYPE_STRING_INDEX:
								fprintf(pOutputFile, "\"%s\"", pParser->stringTable.getStringByIndex(pOp->iStringIndex).c_str());
								break;
								
							case OP_TYPE_VAR:
								fprintf(pOutputFile, "%s", pParser->symbolTable.getSymbolByIndex(pOp->iSymbolIndex)->sIdent.c_str());
								break;
								
							case OP_TYPE_ABS_ARRAY_INDEX:
								fprintf(pOutputFile, "%s [ %d ]", pParser->symbolTable.getSymbolByIndex(pOp->iSymbolIndex)->sIdent.c_str(), pOp->iOffset);
								break;
								
							case OP_TYPE_VAR_ARRAY_INDEX:
								fprintf(pOutputFile, "%s [ %s ]", pParser->symbolTable.getSymbolByIndex(pOp->iSymbolIndex)->sIdent.c_str(),
																  pParser->symbolTable.getSymbolByIndex(pOp->iOffsetSymbolIndex)->sIdent.c_str());
								break;
								
							case OP_TYPE_FUNC_INDEX:
								fprintf(pOutputFile, "%s", pParser->funcTable.getFuncByIndex(pOp->iFuncIndex)->sFuncName.c_str());
								break;
								
							case OP_TYPE_REG:
								fprintf(pOutputFile, "_RetVal");
								break;
								
							case OP_TYPE_JUMP_TARGET_INDEX:
								fprintf(pOutputFile, "_L%d", pOp->iJumpTargetIndex);
								break;
						}
						if(iCurrOpIndex != iOpCount-1)
							fprintf(pOutputFile, ", ");
					}
					fprintf(pOutputFile, "\n");
					break;
					
				}
					
				case ICODE_NODE_JUMP_TARGET: {
					fprintf(pOutputFile, "\t_L%d:\n", pCurrNode->iJumpTargetIndex);
				}
			}
				
			if(iIsFirstSourceLine)
				iIsFirstSourceLine = FALSE;
		}
	} else {
		fprintf(pOutputFile, "\t\t; (No code)\n");
	}
	fprintf(pOutputFile, "\t}");
}

void LSH_Compiler::emitCode(const char* pstrOutputFileName) {
	if(!(pOutputFile = fopen(pstrOutputFileName, "w")))
		Compiler_ExitOnError("Could not open output file for output");
	
	sOutputFileName = pstrOutputFileName;
	emitHeader();
	
	fprintf(pOutputFile, "; ---- Directives -----------------------------------\n");
	emitDirectives();
	
	fprintf(pOutputFile, "; ---- Global Variables -----------------------------------\n");
	emitScopeSymbols(SCOPE_GLOBAL, SYMBOL_TYPE_VAR);
	emitScopeSymbols(SCOPE_GLOBAL, SYMBOL_TYPE_STRUCT);
	
	fprintf(pOutputFile, "; ---- Functions -----------------------------------\n");
	
	LSH_FuncNode* pCurrFunc;
	LSH_FuncNode* pMainFunc = 0;
	
	int iFuncTableSize = pParser->funcTable.size();
	for(int iCurrFuncIndex = 0; iCurrFuncIndex < iFuncTableSize; ++iCurrFuncIndex) {
		pCurrFunc = pParser->funcTable.getFuncByIndex(iCurrFuncIndex);
		if(pCurrFunc) {
			if(pCurrFunc->ftType == FUNCTION_TYPE_SCRIPT) {
				if(strcasecmp(pCurrFunc->sFuncName.c_str(), MAIN_FUNC_NAME) == 0)
					pMainFunc = pCurrFunc;
				else {
					emitFunc(pCurrFunc);
					fprintf(pOutputFile, "\n\n");
				}
			}
		}
	}
	
	if(pMainFunc) {
		fprintf(pOutputFile, "; ---- Main -----------------------------------\n");
		fprintf(pOutputFile, "\n\n");
		emitFunc(pMainFunc);
	}
	
	fclose(pOutputFile);
}