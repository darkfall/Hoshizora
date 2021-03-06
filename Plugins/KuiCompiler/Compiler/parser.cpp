﻿/*
 *  parser.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月10日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "parser.h"

#include "compiler_error.h"

#include <cmath>

//#include "../../KuiWin32/headers/xLog.h"

#ifdef WIN32
#pragma warning( disable : 4996 ) // disable deprecated warning 
#endif

LSH_Parser::LSH_Parser() {
	pLexer = new LSH_lexer;
	
	initReservedFunctions();
}

LSH_Parser::~LSH_Parser() {
	delete pLexer;
}

int LSH_Parser::isOpRelational(int iOpType){
	if(iOpType != OP_TYPE_EQUAL &&
	   iOpType != OP_TYPE_NOT_EQUAL &&
	   iOpType != OP_TYPE_LESS &&
	   iOpType != OP_TYPE_GREATER &&
	   iOpType != OP_TYPE_LESS_EQUAL &&
	   iOpType != OP_TYPE_GREATER_EQUAL)
		return FALSE;
	return TRUE;
}

int LSH_Parser::isOpLogical(int iOpType) {
	if(iOpType != OP_TYPE_LOGICAL_AND &&
	   iOpType != OP_TYPE_LOGICAL_OR &&
	   iOpType != OP_TYPE_LOGICAL_NOT)
		return FALSE;
	return TRUE;
}

void LSH_Parser::readToken(Token reqToken) {
	if(pLexer->getNextToken() != reqToken) {
		string sErrorMssg;
		switch (reqToken) {
			case TOKEN_TYPE_INT:
				sErrorMssg = "Integer";
				break;
				
			case TOKEN_TYPE_FLOAT:
				sErrorMssg = "Float";
				break;
				
			case TOKEN_TYPE_IDENT:
				sErrorMssg = "Identifier";
				break;
				
			case TOKEN_TYPE_RSRVD_VAR:
				sErrorMssg = "Var";
				break;
				
			case TOKEN_TYPE_RSRVD_TRUE:
				sErrorMssg = "True";
				break;
				
			case TOKEN_TYPE_RSRVD_FALSE:
				sErrorMssg = "False";
				break;
				
			case TOKEN_TYPE_RSRVD_ELSE:
				sErrorMssg = "Else";
				break;
				
			case TOKEN_TYPE_RSRVD_BREAK:
				sErrorMssg = "Break";
				break;
				
			case TOKEN_TYPE_RSRVD_CONTINUE:
				sErrorMssg = "Continue";
				break;
				
			case TOKEN_TYPE_RSRVD_FOR:
				sErrorMssg = "For";
				break;
				
			case TOKEN_TYPE_RSRVD_WHILE:
				sErrorMssg = "While";
				break;
				
			case TOKEN_TYPE_RSRVD_FUNC:
				sErrorMssg = "Func";
				break;
				
			case TOKEN_TYPE_RSRVD_RETURN:
				sErrorMssg = "Return";
				break;
				
			case TOKEN_TYPE_RSRVD_HOST:
				sErrorMssg = "Host";
				break;
				
			case TOKEN_TYPE_OP:
				sErrorMssg = "Operator";
				break;
				
			case TOKEN_TYPE_DELIM_COMMA:
				sErrorMssg = ",";
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_PAREN:
				sErrorMssg = "(";
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_PAREN:
				sErrorMssg = ")";
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_BRACE:
				sErrorMssg = "[";
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_BRACE:
				sErrorMssg = "]";
				break;
				
			case TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
				sErrorMssg = "{";
				break;
				
			case TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE:
				sErrorMssg = "}";
				break;
				
			case TOKEN_TYPE_DELIM_SEMICOLON:
				sErrorMssg = ";";
				break;
				
			case TOKEN_TYPE_DELIM_POINT:
				sErrorMssg = ".";
				break;
				
			case TOKEN_TYPE_STRING:
				sErrorMssg = "String";
				break;
		}
		sErrorMssg += " expected";
		exitOnCodeError(sErrorMssg.c_str());
	}
}

bool LSH_Parser::parseSourceCode() {
	pLexer->resetLexer(vSourceCode.begin(), vSourceCode.end());

	bError = true;
	
	iCurrScope = SCOPE_GLOBAL;
	
	while (TRUE) {
		parseStatement();
		
		if(pLexer->getNextToken() == TOKEN_TYPE_END_OF_STREAM)
			break;
		else
			pLexer->rewindTokenStream();
	}
	optimizeInstrStream(2);

	return bError;
}

void LSH_Parser::parseStatement() {
	if(pLexer->getLookAheadChar() == ';') {
		readToken(TOKEN_TYPE_DELIM_SEMICOLON);
		return;
	}
	
	Token initToken = pLexer->getNextToken();
	switch (initToken) {
		case TOKEN_TYPE_END_OF_STREAM:
			exitOnCodeError("Unexpected end of file");
			break;
			
		case TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
			parseBlock();
			break;
			
		case TOKEN_TYPE_RSRVD_VAR:
			parseVar();
			break;
			
		case TOKEN_TYPE_RSRVD_HOST:
			parseHost();
			break;
			
		case TOKEN_TYPE_RSRVD_FUNC:
			parseFunc();
			break;
			
		case TOKEN_TYPE_RSRVD_IF:
			parseIf();
			break;
			
		case TOKEN_TYPE_RSRVD_WHILE:
			parseWhile();
			break;
			
		case TOKEN_TYPE_OP:
			parseForwardOp();
			break;
			
		case TOKEN_TYPE_RSRVD_FOR:
			parseFor();
			break;
			
		case TOKEN_TYPE_RSRVD_BREAK:
			parseBreak();
			break;
			
		case TOKEN_TYPE_RSRVD_CONTINUE:
			parseContinue();
			break;
			
		case TOKEN_TYPE_RSRVD_RETURN:
			parseReturn();
			break;
			
		case TOKEN_TYPE_DELIM_POUND:
			parsePound();
			break;
			
		case TOKEN_TYPE_IDENT: {
			if(symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope)) {
				parseAssign();
			}
			else if(structTable.getStructByIdent(pLexer->getCurrLexeme())) {
				parseStructAssign();
			}
			else if(funcTable.getFuncByName(pLexer->getCurrLexeme())) {
				funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
				parseFuncCall(FALSE);
				readToken(TOKEN_TYPE_DELIM_SEMICOLON);
			} else {
				exitOnCodeError("Invalid identifier");
			}
			break;
		}
			
		case TOKEN_TYPE_RSRVD_PRINT:
			parsePrint();
			break;
			
		case TOKEN_TYPE_RSRVD_STRUCT:
			parseStruct();
			break;
			
		default:
			exitOnCodeError("Unexpected input");
			break;
	}
}

void LSH_Parser::parseBlock() {
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("Code blocks illegal in global scope");
	
	while(pLexer->getLookAheadChar() != '}')
		parseStatement();
	
	readToken(TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE);
}

void LSH_Parser::parseVar() {
	readToken(TOKEN_TYPE_IDENT);
	
	char pstrIdent[LEXER_MAX_LEXEME_SIZE];
	pLexer->copyCurrLexeme(pstrIdent);
	
	int iSize = 1;
	if(pLexer->getLookAheadChar() == '[') {
		readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
		readToken(TOKEN_TYPE_INT);
		
		iSize = atoi(pLexer->getCurrLexeme());
		
		readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
	}
	
	int iSymbolIndex;
	iSymbolIndex = symbolTable.addSymbol(pstrIdent, iSize, iCurrScope, SYMBOL_TYPE_VAR);
	if(iSymbolIndex == -1)
		exitOnCodeError("Identifier redefinition");

	int iNextToken = pLexer->getNextToken();
	if(iNextToken == TOKEN_TYPE_OP) {
		if(pLexer->getCurrOp() == OP_TYPE_ASSIGN) {
			if(iCurrScope != SCOPE_GLOBAL) {
				parseSubExpr();
			
				int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOV);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iSymbolIndex);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			} else {
				while(pLexer->getNextToken() != TOKEN_TYPE_DELIM_SEMICOLON);
				pLexer->rewindTokenStream();
			}
		} else
			pLexer->rewindTokenStream();
	} else
		pLexer->rewindTokenStream();
	
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
}

void LSH_Parser::parseHost() {
	readToken(TOKEN_TYPE_IDENT);
	
	int iFuncIndex = -1;
	if((iFuncIndex = funcTable.addFunc(pLexer->getCurrLexeme(), FUNCTION_TYPE_HOSTAPI)) == -1)
		exitOnCodeError("HostAPI redefinition");
	
	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	
	if(pLexer->getLookAheadChar() != ')') {
		int iParamCount = 0;		
		while (TRUE) {
			readToken(TOKEN_TYPE_IDENT);
			++iParamCount;
			if(pLexer->getLookAheadChar() == ')')
				break;
			readToken(TOKEN_TYPE_DELIM_COMMA);
		}
		
		funcTable.setFuncParamCount(iFuncIndex, iParamCount);
	}
	
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
}

void LSH_Parser::parseFunc() {
	if(iCurrScope != SCOPE_GLOBAL)
		exitOnCodeError("Nested functions illegal");
	
	readToken(TOKEN_TYPE_IDENT);
	
	int iFuncIndex = funcTable.addFunc(pLexer->getCurrLexeme(), FUNCTION_TYPE_SCRIPT);
	if(iFuncIndex == -1)
		exitOnCodeError("Function redefinition");
	
	if(strcasecmp(pLexer->getCurrLexeme(), MAIN_FUNC_NAME) == 0) {
		scriptHeader.iIsMainFuncPresent = TRUE;
		scriptHeader.iMainFuncIndex = iFuncIndex;
	}
	
	iCurrScope = iFuncIndex;
	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	
	if(pLexer->getLookAheadChar() != ')') {
		if(scriptHeader.iIsMainFuncPresent && scriptHeader.iMainFuncIndex == iFuncIndex)
			exitOnCodeError("_Main () cannot accept parameters");
		
		int iParamCount = 0;
		char ppstrParamList[MAX_FUNC_DECLARE_PARAM_COUNT][PARSER_MAX_IDENT_SIZE];
		
		while (TRUE) {
			readToken(TOKEN_TYPE_IDENT);
			pLexer->copyCurrLexeme(ppstrParamList[iParamCount]);
			++iParamCount;
			if(pLexer->getLookAheadChar() == ')')
				break;
			readToken(TOKEN_TYPE_DELIM_COMMA);
		}
		
		funcTable.setFuncParamCount(iCurrScope, iParamCount);
		while(iParamCount > 0) {
			--iParamCount;
			symbolTable.addSymbol(ppstrParamList[iParamCount], 1, iCurrScope, SYMBOL_TYPE_PARAM);
		}
	}
	
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
	
	readToken(TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE);
	
	parseBlock();
	
	iCurrScope = SCOPE_GLOBAL;
}

void LSH_Parser::parseExpr() {
	int iInstrIndex;
	int iOpType;
	
	parseSubExpr();
	
	while(TRUE) {
		Token tNextToken = pLexer->getNextToken();
		if(tNextToken != TOKEN_TYPE_OP ||
		   (!isOpRelational(pLexer->getCurrOp()) && !isOpLogical(pLexer->getCurrOp()))) {
			pLexer->rewindTokenStream();
			break;
		}
		   
		iOpType = pLexer->getCurrOp();
		parseSubExpr();
		
		if(isOpRelational(iOpType)) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			switch (iOpType) {
				case OP_TYPE_EQUAL: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
					break;
				}
					
				case OP_TYPE_NOT_EQUAL: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JNE);
					break;
				}
					
				case OP_TYPE_GREATER: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JG);
					break;
				}
				
				case OP_TYPE_LESS: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JL);
					break;
				}
					
				case OP_TYPE_GREATER_EQUAL: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JGE);
					break;
				}
					
				case OP_TYPE_LESS_EQUAL: {
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JLE);
					break;
				}
			}
			
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			
			pLexer->getNextToken();
			int iType = 0;
			if(pLexer->getCurrToken() == TOKEN_TYPE_DELIM_CLOSE_PAREN) {
				char cNextChar = pLexer->getLookAheadChar();
				if(cNextChar == '|') iType = 1;
				else if(cNextChar == '&') iType = 2;
			} else {
				if(pLexer->getCurrOp() == OP_TYPE_LOGICAL_AND) iType = 2;
				else if(pLexer->getCurrOp() == OP_TYPE_LOGICAL_OR) iType = 1;
			}

			if(sLogicalLoopTargetStack.size() > 0) {
			   if(sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_WHILE ||
				  sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_IF ||
				  sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_FOR) {
				 			//			iExitJumpTargetIndex = funcTable.getNextJumpTargetIndex();
				/*   
				   if(sLogicalOperatorStack.size() > 0) {
					   if(sLogicalOperatorStack.top() == LOGICAL_AND) {
						   int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex();

						   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
				   
						   iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
						   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLogicalLoopTargetStack.top().iEndTargetIndex);
				   
						   funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
					   } else if(sLogicalOperatorStack.top() == LOGICAL_OR) {
						   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLogicalLoopTargetStack.top().iEndTargetIndex);
					   }
					   sLogicalOperatorStack.pop();
				   } else {
							int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex();
						   
						   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
						   
						   iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
						   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLogicalLoopTargetStack.top().iEndTargetIndex);
						   
						   funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
					  // }
				   }*/
				   if(iType == 1) {
					   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLogicalLoopTargetStack.top().iStartTargetIndex);
				   } else {
					   int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex();
					   
					   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
					   
					   iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
					   funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLogicalLoopTargetStack.top().iEndTargetIndex);
					   
					   funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
				
				   }
						 
				   pLexer->rewindTokenStream();
	//			   iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
	//			   funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
				   
	//			   funcTable.addICodeJumpTarget(iCurrScope, iExitJumpTargetIndex);
			   }
			} else {
				int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex(),
					iExitJumpTargetIndex = funcTable.getNextJumpTargetIndex();
				
				funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
			
				//push 0
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
				funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
			
				//jmp exit
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
				funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iExitJumpTargetIndex);
			
				//L0: (true)
				funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
			
				//push 1
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
				funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
			
				//L1: (Exit)
				funcTable.addICodeJumpTarget(iCurrScope, iExitJumpTargetIndex);
			}

			pLexer->rewindTokenStream();

		} else {
			switch (iOpType) {
				case OP_TYPE_LOGICAL_AND: {
					if(sLogicalLoopTargetStack.size() > 0) {
						if(sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_WHILE ||
						   sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_IF ||
						   sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_FOR) {					
							//JE _T0, 0, TRUE
				//			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
				//			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
				//			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
				///			funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLoopStack.top()->iEndTargetIndex);
							
							//JE _T1, 0, TRUE
				//			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
				//			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
				///			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
					//		funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLoopStack.top()->iEndTargetIndex);
							
				//			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
				//			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
							sLogicalOperatorStack.push(LOGICAL_AND);

							break;
						}							
					} else {
						int iFalseJumpTargetIndex = funcTable.getNextJumpTargetIndex(),
							iExitJumpTargetIndex = funcTable.getNextJumpTargetIndex();
					
						//JE _T0, 0, TRUE
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
						funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iFalseJumpTargetIndex);
					
						//JE _T1, 0, TRUE
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
						funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iFalseJumpTargetIndex);
					
						//push 1
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
					
						//Jmp Exit
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iExitJumpTargetIndex);
					
						// L0: (False)
						funcTable.addICodeJumpTarget(iCurrScope, iFalseJumpTargetIndex);
						
						//Push 0
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
					
						// L1: (Exit)
						funcTable.addICodeJumpTarget(iCurrScope, iExitJumpTargetIndex);
						break;
					}
				}
				
				case OP_TYPE_LOGICAL_OR: {
					if(sLogicalLoopTargetStack.size() > 0) {
						if(sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_WHILE ||
						   sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_IF ||
						   sLogicalLoopTargetStack.top().eType == LOGICAL_EXPRESSION_FOR) {	
							// JNE _T0, 0, TRUE
				/*			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JNE);
							funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
							funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
							funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLoopStack.top()->iEndTargetIndex);
							
							// JNE _T1, 0, TRUE
							iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JNE);
							funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
							funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
							funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, sLoopStack.top()->iEndTargetIndex);
							
							iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
							funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);*/
							sLogicalOperatorStack.push(LOGICAL_OR);
							break;
						}
						
					} else {
						int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex(),
							iExitJumpTargetIndex = funcTable.getNextJumpTargetIndex();
					
						// JNE _T0, 0, TRUE
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JNE);
						funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
					
						// JNE _T1, 0, TRUE
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JNE);
						funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
					
						//Push 0
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
					
						//Jmp Exit
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
						funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iExitJumpTargetIndex);
					
						// L0: (False)
						funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
					
						//Push 1
						iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
						funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
					
						// L1: (Exit)
						funcTable.addICodeJumpTarget(iCurrScope, iExitJumpTargetIndex);
						break;
					}
				}
			}
		}
	}
}
		   
void LSH_Parser::parseSubExpr() {
	int iInstrIndex;
	int iOpType;
	
	parseTerm();
	
	while(TRUE) {
		Token tNextToken = pLexer->getNextToken();
		if(tNextToken != TOKEN_TYPE_OP || (pLexer->getCurrOp() != OP_TYPE_ADD &&
										   pLexer->getCurrOp() != OP_TYPE_SUB &&
										   pLexer->getCurrOp() != OP_TYPE_CONCAT) ) {
			pLexer->rewindTokenStream();
			break;
		}
		
		iOpType = pLexer->getCurrOp();
		parseTerm();
		
		//pop op1 to _T1
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
		
		//pop op2 to _T0
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		
		int iOpInstr;
		switch (iOpType) {
			case OP_TYPE_ADD:
				iOpInstr = INSTR_ADD;
				break;
				
			case OP_TYPE_SUB:
				iOpInstr = INSTR_SUB;
				break;
				
			case OP_TYPE_CONCAT:
				iOpInstr = INSTR_CONCAT;
				break;
		}
		
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, iOpInstr);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
		
		//push result
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	}
}
		
void LSH_Parser::parseTerm() {
	int iInstrIndex;
	int iOpType;
	
	parseFactor();
	
	while(TRUE) {
		Token tNextToken = pLexer->getNextToken();
		int iCurrOp = pLexer->getCurrOp();
		if(tNextToken != TOKEN_TYPE_OP || (iCurrOp != OP_TYPE_MUL &&
										   iCurrOp != OP_TYPE_DIV &&
										   iCurrOp != OP_TYPE_MOD &&
										   iCurrOp != OP_TYPE_EXP &&
										   iCurrOp != OP_TYPE_BITWISE_AND &&
										   iCurrOp != OP_TYPE_BITWISE_OR &&
										   iCurrOp != OP_TYPE_BITWISE_XOR &&
										   iCurrOp != OP_TYPE_BITWISE_SHIFT_LEFT &&
										   iCurrOp != OP_TYPE_BITWISE_SHIFT_RIGHT) ) {
			pLexer->rewindTokenStream();
			break;
		}
		
		iOpType = pLexer->getCurrOp();
		parseFactor();
		
		//pop op1 to _T1
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
		
		//pop op2 to _T0
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		
		int iOpInstr;
		switch (iOpType) {
			case OP_TYPE_MUL:
				iOpInstr = INSTR_MUL;
				break;
			
			case OP_TYPE_DIV:
				iOpInstr = INSTR_DIV;
				break;
				
			case OP_TYPE_MOD:
				iOpInstr = INSTR_MOD;
				break;
				
			case OP_TYPE_EXP:
				iOpInstr = INSTR_EXP;
				break;
				
			case OP_TYPE_BITWISE_AND:
				iOpInstr = INSTR_AND;
				break;
				
			case OP_TYPE_BITWISE_OR:
				iOpInstr = INSTR_OR;
				break;
				
			case OP_TYPE_BITWISE_XOR:
				iOpInstr = INSTR_XOR;
				break;
				
			case OP_TYPE_BITWISE_SHIFT_LEFT:
				iOpInstr = INSTR_SHL;
				break;
				
			case OP_TYPE_BITWISE_SHIFT_RIGHT:
				iOpInstr = INSTR_SHR;
				break;
		}
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, iOpInstr);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
		
		//push result
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	}
}

void LSH_Parser::parseFactor() {
	int iInstrIndex;
	int iUnaryOpPending = FALSE;
	int iOpType;
	
	Token tNextToken = pLexer->getNextToken();
	if(tNextToken == TOKEN_TYPE_OP && (pLexer->getCurrOp() == OP_TYPE_ADD ||
									   pLexer->getCurrOp() == OP_TYPE_SUB ||
									   pLexer->getCurrOp() == OP_TYPE_BITWISE_NOT ||
									   pLexer->getCurrOp() == OP_TYPE_LOGICAL_NOT ||
									   pLexer->getCurrOp() == OP_TYPE_INC ||
									   pLexer->getCurrOp() == OP_TYPE_DEC) ) {
		iUnaryOpPending = TRUE;
		iOpType = pLexer->getCurrOp();
	} else {
		pLexer->rewindTokenStream();
	}
	
	tNextToken = pLexer->getNextToken();
	switch (tNextToken) {
		case TOKEN_TYPE_RSRVD_TRUE:
		case TOKEN_TYPE_RSRVD_FALSE:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			if(tNextToken == TOKEN_TYPE_RSRVD_TRUE)
				funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
			else
				funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
			break;
	
		case TOKEN_TYPE_INT: {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			int iCurrInt = atoi(pLexer->getCurrLexeme());
			if(pLexer->getNextToken() == TOKEN_TYPE_OP) {
				if(pLexer->getCurrOp() == OP_TYPE_INC) {
					funcTable.addIntICodeOp(iCurrScope, iInstrIndex, iCurrInt+1);
					break;
				} else if(pLexer->getCurrOp() == OP_TYPE_DEC) {
					funcTable.addIntICodeOp(iCurrScope, iInstrIndex, iCurrInt-1);
					break;
				} else
					pLexer->rewindTokenStream();
			} else
				pLexer->rewindTokenStream();
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, iCurrInt);
			break;
		}
			
		case TOKEN_TYPE_FLOAT: {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			float iCurrFloat = atof(pLexer->getCurrLexeme());
			if(pLexer->getNextToken() == TOKEN_TYPE_OP) {
				if(pLexer->getCurrOp() == OP_TYPE_INC) {
					funcTable.addIntICodeOp(iCurrScope, iInstrIndex, static_cast<int>(iCurrFloat+1));
					break;
				} else if(pLexer->getCurrOp() == OP_TYPE_DEC) {
					funcTable.addIntICodeOp(iCurrScope, iInstrIndex, static_cast<int>(iCurrFloat-1));
					break;
				} else
					pLexer->rewindTokenStream();
			} else
				pLexer->rewindTokenStream();
			funcTable.addFloatICodeOp(iCurrScope, iInstrIndex, iCurrFloat);
			break;
		}
			
		case TOKEN_TYPE_STRING: {
			int iStringIndex = stringTable.addString(pLexer->getCurrLexeme());
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addStringICodeOp(iCurrScope, iInstrIndex, iStringIndex);
			break;
		}
			
		case TOKEN_TYPE_IDENT: {
			LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope);
			if(pSymbol) {
				if(pLexer->getLookAheadChar() == '[') {
					if(pSymbol->iSize == 1)
						exitOnCodeError("Invalid array");
					
					readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
					if(pLexer->getLookAheadChar() == ']')
						exitOnCodeError("Invalid expression");
					
					parseExpr();
					
					readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
					
					//pop result(array index) into _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
					
					if(pLexer->getNextToken() == TOKEN_TYPE_OP) {
						int iCurrOp = pLexer->getCurrOp();
						switch (iCurrOp) {
							case OP_TYPE_INC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
								break;
								
							case OP_TYPE_DEC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
								break;
								
							default:
								pLexer->rewindTokenStream();
						}
					} else
						pLexer->rewindTokenStream();
					
					//push the original identifier, indexed with _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
					funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
				} else if(pLexer->getLookAheadChar() == '.') {
					if(pSymbol->iType != SYMBOL_TYPE_STRUCT)
						exitOnCodeError("Invalid '.' operator");
					
					structElmntToArrayIndex(pLexer->getCurrLexeme());
					
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
					
					if(pLexer->getNextToken() == TOKEN_TYPE_OP) {
					int iCurrOp = pLexer->getCurrOp();
						switch (iCurrOp) {
							case OP_TYPE_INC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
								break;

							case OP_TYPE_DEC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
								break;

							default:
								pLexer->rewindTokenStream();
						}
					} else
						pLexer->rewindTokenStream();

					//push the original identifier, indexed with _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
					funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
				} else {
					if(pSymbol->iSize != 1)
						exitOnCodeError("Arrays must be indexed");
					
					if(pLexer->getNextToken() == TOKEN_TYPE_OP) {
						int iCurrOp = pLexer->getCurrOp();
						switch (iCurrOp) {
							case OP_TYPE_INC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
								funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
								break;

							case OP_TYPE_DEC:
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
								funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
								break;

							default:
								pLexer->rewindTokenStream();
						}
					} else
						pLexer->rewindTokenStream();

					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
				}
			} else {				
				if(funcTable.getFuncByName(pLexer->getCurrLexeme())) {
					parseFuncCall(TRUE);
				}
			}
			break;
		}
			
		case TOKEN_TYPE_DELIM_OPEN_PAREN:
			parseExpr();
			readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
			break;
			
		default:
			exitOnCodeError("Invalid input");
	}

	if(iUnaryOpPending) {
		//pop the result of the factor
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		
		if(iOpType == OP_TYPE_LOGICAL_NOT) {
			int iTrueJumpTargetIndex = funcTable.getNextJumpTargetIndex(),
				iExitJumpTargetIndex = funcTable.getNextJumpTargetIndex();
			
			//JE _T0, 0, TRUE
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
			funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTrueJumpTargetIndex);
			
			//push 0
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
			
			//Jmp L1
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
			funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iExitJumpTargetIndex);
			
			// L0: (True)
			funcTable.addICodeJumpTarget(iCurrScope, iTrueJumpTargetIndex);
			
			//Push 1
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 1);
			
			// L1: (Exit)
			funcTable.addICodeJumpTarget(iCurrScope, iExitJumpTargetIndex);
		} else {
			int iOpIndex;
			switch (iOpType) {
				case OP_TYPE_SUB:
					iOpIndex = INSTR_NEG;
					break;
					
				case OP_TYPE_BITWISE_NOT:
					iOpIndex = INSTR_NOT;
					break;
					
				case OP_TYPE_INC:
					iOpIndex = INSTR_INC;
					break;
					
				case OP_TYPE_DEC:
					iOpIndex = INSTR_DEC;
					break;
			}
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, iOpIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		}
	}
}

void LSH_Parser::parseFuncCall(int iIsReturnValue) {
	LSH_FuncNode* pFunc = funcTable.getFuncByName(pLexer->getCurrLexeme());
	if(!pFunc)
		exitOnCodeError("Invalid funccall");
	
	// if the function is script functions or hostAPIs
	_FUNCTION_TYPES ftType = pFunc->ftType;
	if(ftType == FUNCTION_TYPE_SCRIPT || ftType == FUNCTION_TYPE_HOSTAPI) {
		int iParamCount = 0;
		readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	
		while (TRUE) {
			if(pLexer->getLookAheadChar() != ')') {
				parseExpr();
			
				if(ftType == FUNCTION_TYPE_HOSTAPI) {
					int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
					
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSHHOSTPARAM);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
				}
					
				++iParamCount;
				if(iParamCount > pFunc->iParamCount)
					exitOnCodeError("Too many parameters");
			
				if(pLexer->getLookAheadChar() != ')')
					readToken(TOKEN_TYPE_DELIM_COMMA);
			} else 
				break;
		}
	
		readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
		if(iParamCount < pFunc->iParamCount)
			exitOnCodeError("Too few parameters");
	
		int iCallInstr = INSTR_CALL;
		if(pFunc->ftType == FUNCTION_TYPE_HOSTAPI)
			iCallInstr = INSTR_CALLHOST;
	
		int iInstrIndex = funcTable.addICodeInstr(iCurrScope, iCallInstr);
		funcTable.addFuncICodeOp(iCurrScope, iInstrIndex, pFunc->iIndex);
		
		if(iIsReturnValue) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addRegICodeOp(iCurrScope, iInstrIndex, REG_CODE_RETVAL);
		}
	} 
	// if the function is reserved instr functions
	// (sin, cos, etc)
	else if(ftType == FUNCTION_TYPE_RSRVD_INSTR) {
		if(pFunc->iParamCount == 1) {
			readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
			parseExpr();
			readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
			
			// pop the result to _T1
			int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			
			// add reserved instr
			int iOpType;
			char* pstrFuncName = const_cast<char*>(pFunc->sFuncName.c_str());
			if(strcasecmp(pstrFuncName, "sin") == 0)
				iOpType = INSTR_SIN;
			else if(strcasecmp(pstrFuncName, "cos") == 0)
				iOpType = INSTR_COS;
			else if(strcasecmp(pstrFuncName, "tan") == 0)
				iOpType = INSTR_TAN;
			else if(strcasecmp(pstrFuncName, "sh") == 0)
				iOpType = INSTR_SH;
			else if(strcasecmp(pstrFuncName, "ch") == 0)
				iOpType = INSTR_CH;
			else if(strcasecmp(pstrFuncName, "th") == 0)
				iOpType = INSTR_TH;
			else if(strcasecmp(pstrFuncName, "ln") == 0)
				iOpType = INSTR_LN;
			else if(strcasecmp(pstrFuncName, "lg") == 0)
				iOpType = INSTR_LG;
			else if(strcasecmp(pstrFuncName, "rec") == 0)
				iOpType = INSTR_REC;
			else if(strcasecmp(pstrFuncName, "sqrt") == 0)
				iOpType = INSTR_SQRT;
			else if(strcasecmp(pstrFuncName, "expl") == 0)
				iOpType = INSTR_EXPL;
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, iOpType);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			
			// push the result
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		} else if(pFunc->iParamCount == 2) {
			readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
			// read the first param
			parseExpr();
			
			int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			
			readToken(TOKEN_TYPE_DELIM_COMMA);
			
			// read the second param
			parseExpr();
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar2SymbolIndex);
			
			readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
			
			int iOpType;
			char* pstrFuncName = const_cast<char*>(pFunc->sFuncName.c_str());
			if(strcasecmp(pstrFuncName, "randomInt") == 0)
				iOpType = INSTR_RANDOMINT;
			else if(strcasecmp(pstrFuncName, "randomFloat") == 0)
				iOpType = INSTR_RANDOMFLOAT;
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, iOpType);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar2SymbolIndex);
			
			// push the result
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		}
	}
}

void LSH_Parser::parseIf() {
	int iInstrIndex;
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("if illegal in global scope");
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	
	int iFalseJumpTargetIndex = funcTable.getNextJumpTargetIndex();
	int iStartJumpTargetIndex = funcTable.getNextJumpTargetIndex();
	
	int iIsInLoop = FALSE;
	if(sLoopStack.size() > 0) {
		if(sLoopStack.top()->eType == LOOP_TYPE_WHILE || sLoopStack.top()->eType == LOOP_TYPE_FOR) {
			LSH_Parser_Loop* pLoop = new LSH_Parser_Loop;
			pLoop->iStartTargetIndex	= sLoopStack.top()->iStartTargetIndex;
			pLoop->iEndTargetIndex		= sLoopStack.top()->iEndTargetIndex;
			pLoop->eType				= LOOP_TYPE_IF_IN_LOOP;
			sLoopStack.push(pLoop);
			iIsInLoop = TRUE;
		} else {
			if(sLoopStack.top()->eType == LOOP_TYPE_IF_IN_LOOP) {
				sLoopStack.pop();
				if(sLoopStack.size() == 0)
					exitOnCodeError("loop stack error");
				while(sLoopStack.top()->eType != LOOP_TYPE_WHILE || sLoopStack.top()->eType != LOOP_TYPE_FOR) {
					sLoopStack.pop();
					if(sLoopStack.size() == 0)
						exitOnCodeError("loop stack error");
				}
				LSH_Parser_Loop* pLoop = new LSH_Parser_Loop;
				pLoop->iStartTargetIndex	= sLoopStack.top()->iStartTargetIndex;
				pLoop->iEndTargetIndex		= sLoopStack.top()->iEndTargetIndex;
				pLoop->eType				= LOOP_TYPE_IF_IN_LOOP;
				sLoopStack.push(pLoop);
				iIsInLoop = TRUE;
			}
		}
	}
	
	LSH_LogicalExpressionTarget target;
	target.eType			= LOGICAL_EXPRESSION_IF;
	target.iEndTargetIndex	= iFalseJumpTargetIndex;
	target.iStartTargetIndex = iStartJumpTargetIndex;
	sLogicalLoopTargetStack.push(target);
			
	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	parseExpr();
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
	
	if(iIsInLoop) 
		if(sLoopStack.top()->eType == LOOP_TYPE_IF_IN_LOOP)
			sLoopStack.pop();
	
	sLogicalLoopTargetStack.pop();
	
	funcTable.addICodeJumpTarget(iCurrScope, iStartJumpTargetIndex);

	// pop the result to _T0
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	
	// Jump to the false target
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
//	funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
//	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iFalseJumpTargetIndex);
	
	parseStatement();
	
	if(pLexer->getNextToken() == TOKEN_TYPE_RSRVD_ELSE) {
		// if there's else, jump to it
		int iSkipFalseJumpTargetIndex = funcTable.getNextJumpTargetIndex();
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
		funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iSkipFalseJumpTargetIndex);
		
		funcTable.addICodeJumpTarget(iCurrScope, iFalseJumpTargetIndex);
		
		parseStatement();
		
		funcTable.addICodeJumpTarget(iCurrScope, iSkipFalseJumpTargetIndex);
	} else {
		pLexer->rewindTokenStream();
		
		funcTable.addICodeJumpTarget(iCurrScope, iFalseJumpTargetIndex);
	}
}

void LSH_Parser::parseWhile() {
	int iInstrIndex;
	
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("Statement illegal in global scope");
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrLexeme());
	
	int iStartTargetIndex = funcTable.getNextJumpTargetIndex(),
		iEndTargetIndex   = funcTable.getNextJumpTargetIndex();
		
	funcTable.addICodeJumpTarget(iCurrScope, iStartTargetIndex);
	
	// push a loop stack value which indicates the start and the end of the loop
	LSH_Parser_Loop* pLoop = new LSH_Parser_Loop;
	pLoop->iStartTargetIndex	= iStartTargetIndex;
	pLoop->iEndTargetIndex		= iEndTargetIndex;
	pLoop->eType				= LOOP_TYPE_WHILE;
	sLoopStack.push(pLoop);
	
	LSH_LogicalExpressionTarget target;
	target.eType				= LOGICAL_EXPRESSION_WHILE;
	target.iEndTargetIndex		= iEndTargetIndex;
	target.iStartTargetIndex	= iStartTargetIndex;
	sLogicalLoopTargetStack.push(target);

	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	parseExpr();
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);
	
	// pop the result to _T0 and jump out of the loop if it's nonezero
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
//	funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
//	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iEndTargetIndex);
	// Pop the result into _T0 and jump out of the loop if it's nonzero*/
	
	parseStatement();
	
	sLogicalLoopTargetStack.pop();

	sLoopStack.pop();
	
	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iStartTargetIndex);
	
	funcTable.addICodeJumpTarget(iCurrScope, iEndTargetIndex);
}

void LSH_Parser::parseFor() {
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("for illegal in global scope");
	
	int iInstrIndex;
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());

	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	
	int iIsArray = FALSE;
	
	// read loop var
	readToken(TOKEN_TYPE_IDENT);
	string sLoopVar = pLexer->getCurrLexeme();
	LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope);

	if(!symbolTable.getSymbolByIdent(sLoopVar.c_str(), iCurrScope))
		exitOnCodeError("undefined variable");
	
	if(pLexer->getLookAheadChar() == '.') {
		structElmntToArrayIndex(pLexer->getCurrLexeme());
		iIsArray = TRUE;
	}
	else if(pLexer->getLookAheadChar() == '[') {
		readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
		if(pLexer->getLookAheadChar() == ']')
			exitOnCodeError("Invalid array index");
		
		parseExpr();
		readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
		
		iIsArray = TRUE;
	}
	
	// if is not assign then continue
	pLexer->getNextToken();
	if(pLexer->getCurrToken() == TOKEN_TYPE_OP) {
		if(pLexer->getCurrOp() != OP_TYPE_ASSIGN)
			exitOnCodeError("= expected");
		
		parseExpr();
		
		// pop the result to the loop variable
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		
		if(iIsArray) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
		}
		
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOV);
		if(iIsArray)
			funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar1SymbolIndex);	
		else 
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);

		readToken(TOKEN_TYPE_DELIM_SEMICOLON);
	} else if(pLexer->getCurrToken() != TOKEN_TYPE_DELIM_SEMICOLON)
		exitOnCodeError("invalid expression");
	
	int iStartTargetIndex = funcTable.getNextJumpTargetIndex(),
		iEndTargetIndex   = funcTable.getNextJumpTargetIndex();
	
	funcTable.addICodeJumpTarget(iCurrScope, iStartTargetIndex);
	
	// push a loop stack value which indicates the start and the end of the loop
	LSH_Parser_Loop* pLoop = new LSH_Parser_Loop;
	pLoop->iStartTargetIndex	= iStartTargetIndex;
	pLoop->iEndTargetIndex		= iEndTargetIndex;
	pLoop->eType				= LOOP_TYPE_FOR;
	sLoopStack.push(pLoop);
	
	LSH_LogicalExpressionTarget target;
	target.eType				= LOGICAL_EXPRESSION_WHILE;
	target.iEndTargetIndex		= iEndTargetIndex;
	target.iStartTargetIndex	= iStartTargetIndex;
	sLogicalLoopTargetStack.push(target);	

	parseExpr();
	
	sLogicalLoopTargetStack.pop();
	
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	
//	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JE);
//	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
//	funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
//	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iEndTargetIndex);
	
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
	
	int iCurrOp;

	pLexer->getNextToken();
	if(pLexer->getCurrToken() == TOKEN_TYPE_OP) {
		iCurrOp = pLexer->getCurrOp();
		
		readToken(TOKEN_TYPE_IDENT);
		if(sLoopVar.compare(pLexer->getCurrLexeme()) != 0)
			exitOnCodeError("Different \"for\" variable");
		
		if(pLexer->getLookAheadChar() == '.') {
			structElmntToArrayIndex(pLexer->getCurrLexeme());
			iIsArray = TRUE;
		}
		else if(pLexer->getLookAheadChar() == '[') {
			readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
			if(pLexer->getLookAheadChar() == ']')
				exitOnCodeError("Invalid array index");
			
			parseExpr();
			readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
			
			iIsArray = TRUE;
		}
	} else if(pLexer->getCurrToken() == TOKEN_TYPE_IDENT) {
		if(sLoopVar.compare(pLexer->getCurrLexeme()) != 0)
			exitOnCodeError("Different \"for\" variable");
		
		if(pLexer->getLookAheadChar() == '.') {
			structElmntToArrayIndex(pLexer->getCurrLexeme());
			iIsArray = TRUE;
		}
		else if(pLexer->getLookAheadChar() == '[') {
			readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
			if(pLexer->getLookAheadChar() == ']')
				exitOnCodeError("Invalid array index");
			
			parseExpr();
			readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
			
			iIsArray = TRUE;
		}
		
		Token tNextToken = pLexer->getNextToken();
		iCurrOp = pLexer->getCurrOp();

		if(tNextToken == TOKEN_TYPE_OP) {
			if((iCurrOp == OP_TYPE_ASSIGN ||
				iCurrOp == OP_TYPE_ASSIGN_ADD ||
				iCurrOp == OP_TYPE_ASSIGN_SUB || 
				iCurrOp == OP_TYPE_ASSIGN_MUL ||
				iCurrOp == OP_TYPE_ASSIGN_DIV ||
				iCurrOp == OP_TYPE_ASSIGN_MOD ||
				iCurrOp == OP_TYPE_ASSIGN_EXP ||
				iCurrOp == OP_TYPE_ASSIGN_AND ||	
				iCurrOp == OP_TYPE_ASSIGN_OR || 
				iCurrOp == OP_TYPE_ASSIGN_XOR ||
				iCurrOp == OP_TYPE_ASSIGN_SHIFT_LEFT ||
				iCurrOp == OP_TYPE_ASSIGN_SHIFT_RIGHT) ) {				
				
				parseExpr();
				
				// store the result in stack
				// because the operation is in the end of the "for" block
				// we need a stack to store the value for the use of nested "for"
			} else
				exitOnCodeError("Illegal assignment operator in \"for\" loop");
		} else
			exitOnCodeError("Invalid operation");
	} else
		exitOnCodeError("Invalid expression");
		
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);

	parseStatement();
	
	// pop the loop value
	sLoopStack.pop();
	
	// calculate expression
	if(iCurrOp == OP_TYPE_INC) {
		if(!iIsArray) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
		}
		else {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
			funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
		}
	} else if(iCurrOp == OP_TYPE_DEC) {
		if(!iIsArray) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
		}
		else {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
			funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
		}
	} else {
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar2SymbolIndex);
		
		if(iIsArray) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		}
	
		switch (iCurrOp) {
			case OP_TYPE_ASSIGN:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOV);
				break;
				
			case OP_TYPE_ASSIGN_ADD:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_ADD);
				break;
			
			case OP_TYPE_ASSIGN_SUB:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SUB);
				break;
			
			case OP_TYPE_ASSIGN_MUL:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MUL);
				break;
			
			case OP_TYPE_ASSIGN_DIV:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DIV);
				break;
			
			case OP_TYPE_ASSIGN_MOD:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOD);
				break;
			
			case OP_TYPE_ASSIGN_EXP:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_EXP);
				break;
	
			case OP_TYPE_ASSIGN_AND:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_AND);
				break;
			
			case OP_TYPE_ASSIGN_OR:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_OR);
				break;
			
			case OP_TYPE_ASSIGN_XOR:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_XOR);
				break;
			
			case OP_TYPE_ASSIGN_SHIFT_LEFT:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SHL);
				break;
			
			case OP_TYPE_ASSIGN_SHIFT_RIGHT:
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SHR);
				break;
		
		}
		if(!iIsArray) {
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar2SymbolIndex);
		} else {
			funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar2SymbolIndex);
		}
	}
	
	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iStartTargetIndex);
	
	funcTable.addICodeJumpTarget(iCurrScope, iEndTargetIndex);
}

void LSH_Parser::parseBreak() {
	if(sLoopStack.size() == 0 || sLoopStack.top()->eType == LOOP_TYPE_NULL)
		exitOnCodeError("break illegal outside loops");
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
	
	// get the jump target index and jump to the end of the loop
	int iTargetIndex = sLoopStack.top()->iEndTargetIndex;
	int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTargetIndex);
}

void LSH_Parser::parseContinue() {
	if(sLoopStack.size() == 0 || sLoopStack.top()->eType == LOOP_TYPE_NULL)
		exitOnCodeError("continue illegal outside loops");
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
	
	int iTargetIndex = sLoopStack.top()->iStartTargetIndex;
	int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_JMP);
	funcTable.addJumpTargetICodeOp(iCurrScope, iInstrIndex, iTargetIndex);
}

void LSH_Parser::parseReturn() {
	int iInstrIndex;
	
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("return illegal in global scope");
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	if(pLexer->getLookAheadChar() != ';') {
		parseExpr();
		
		if(scriptHeader.iIsMainFuncPresent &&
		   scriptHeader.iMainFuncIndex == iCurrScope) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
		} else {
			// not _Main function ,return with _retVal
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addRegICodeOp(iCurrScope, iInstrIndex, REG_CODE_RETVAL);
		}
	} else {
		if(scriptHeader.iIsMainFuncPresent && 
		   scriptHeader.iMainFuncIndex == iCurrScope) {
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOV);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, 0);
		}
	}
	
	if(scriptHeader.iIsMainFuncPresent &&
	   scriptHeader.iMainFuncIndex == iCurrScope) {
		// exit _main with _T0
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_EXIT);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	} else {
		funcTable.addICodeInstr(iCurrScope, INSTR_RET);
	}
	
	LSH_FuncNode* pFunc = funcTable.getFuncByIndex(iCurrScope);
	if(iCurrScope != SCOPE_GLOBAL)
		pFunc->iIsReturnValue = TRUE;
	
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
}

void LSH_Parser::parseAssign() {
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("Assignment illegal in global scope");
	
	int iInstrIndex;
	int iAssignOp;
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	
	LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope);
	if(!pSymbol)
		return;
	
	int iIsArray = FALSE;
	if(pLexer->getLookAheadChar() == '.') {
		structElmntToArrayIndex(pLexer->getCurrLexeme());
		iIsArray = TRUE;
	} else if(pLexer->getLookAheadChar() == '[') {
		if(pSymbol->iSize == 1)
			exitOnCodeError("Invalid Array");
		
		readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
		if(pLexer->getLookAheadChar() == ']')
			exitOnCodeError("Invalid Expression");
		
		parseExpr();
		
		readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
		iIsArray = TRUE;
	} else {
		if(pSymbol->iSize > 1)
			exitOnCodeError("Array must be indexed");
	}
	
	Token tNextToken = pLexer->getNextToken();
	int iCurrOp = pLexer->getCurrOp();
	
	// to see if is var++ or var--
	if(tNextToken == TOKEN_TYPE_OP) {
		if(pLexer->getCurrOp() == OP_TYPE_INC) {
			if(!iIsArray) {
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
			}
			else {
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
				funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
			}
			tNextToken = pLexer->getNextToken();
		} else if(pLexer->getCurrOp() == OP_TYPE_DEC) {
			if(!iIsArray) {
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
			}
			else {
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
				funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
				iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
				funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
			}
			tNextToken = pLexer->getNextToken();
		}
	}
	
	if(tNextToken == TOKEN_TYPE_DELIM_SEMICOLON)
		exitOnCodeError("; unexpected");

	iCurrOp = pLexer->getCurrOp();
	if(tNextToken != TOKEN_TYPE_OP && (iCurrOp != OP_TYPE_ASSIGN &&
									   iCurrOp != OP_TYPE_ASSIGN_ADD &&
									   iCurrOp != OP_TYPE_ASSIGN_SUB && 
									   iCurrOp != OP_TYPE_ASSIGN_MUL &&
									   iCurrOp != OP_TYPE_ASSIGN_DIV &&
									   iCurrOp != OP_TYPE_ASSIGN_MOD &&
									   iCurrOp != OP_TYPE_ASSIGN_EXP &&
									   iCurrOp != OP_TYPE_CONCAT &&
									   iCurrOp != OP_TYPE_ASSIGN_AND &&
									   iCurrOp != OP_TYPE_ASSIGN_OR &&
									   iCurrOp != OP_TYPE_ASSIGN_XOR &&
									   iCurrOp != OP_TYPE_ASSIGN_SHIFT_LEFT &&
									   iCurrOp != OP_TYPE_ASSIGN_SHIFT_RIGHT) )
		exitOnCodeError("Illegal assignment operator");
	
	iAssignOp = pLexer->getCurrOp();
	
	parseExpr();
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
	
	// pop the result into _T0
	iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
	
	// if is an array, pop the top of the stack into _T1 for index
	if(iIsArray) {
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar1SymbolIndex);
	}
	
	switch (iAssignOp) {
		case OP_TYPE_ASSIGN:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOV);
			break;
			
		case OP_TYPE_ASSIGN_ADD:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_ADD);
			break;
			
		case OP_TYPE_ASSIGN_SUB:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SUB);
			break;
			
		case OP_TYPE_ASSIGN_MUL:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MUL);
			break;
			
		case OP_TYPE_ASSIGN_DIV:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DIV);
			break;
			
		case OP_TYPE_ASSIGN_MOD:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_MOD);
			break;
			
		case OP_TYPE_ASSIGN_EXP:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_EXP);
			break;
			
		case OP_TYPE_CONCAT:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_CONCAT);
			break;
			
		case OP_TYPE_ASSIGN_AND:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_AND);
			break;
			
		case OP_TYPE_ASSIGN_OR:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_OR);
			break;
			
		case OP_TYPE_ASSIGN_XOR:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_XOR);
			break;
			
		case OP_TYPE_ASSIGN_SHIFT_LEFT:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SHL);
			break;
			
		case OP_TYPE_ASSIGN_SHIFT_RIGHT:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_SHR);
			break;
	}
	
	if(iIsArray)
		funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar1SymbolIndex);
	else
		funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
	
	funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
}

void LSH_Parser::exitOnCodeError(const char* pstrErrorMssg) {
	cout<<"Error: "<<pstrErrorMssg<<endl;
	cout<<"Line: "<<pLexer->getCurrSourceLineIndex()<<endl;
	
	string sCurrSourceLine = pLexer->getCurrSourceLine();
	for(size_t i=0; i < sCurrSourceLine.size(); ++i) {
		if(sCurrSourceLine[i] == '\t')
			sCurrSourceLine[i] = ' ';
	}
	
	cout<<sCurrSourceLine.c_str()<<endl;
	for(int iCurrSpace=0; iCurrSpace<pLexer->getLexemeStartIndex(); ++iCurrSpace)
		cout<<" ";
	cout<<"^"<<endl;

	string sTmpLine(pLexer->getCurrSourceLine());
	for(size_t i=0; i<sTmpLine.size(); ++i) {
		if(sTmpLine[i] == '\t' || sTmpLine[i] == '\n')
			sTmpLine.erase(sTmpLine.begin()+i);
	}
	cout<<"Script Parser Error: "<<pstrErrorMssg<<endl;
	cout<<"Script Line: "<<pLexer->getCurrSourceLineIndex()<<endl;
	cout<<"Token: "<<pLexer->getCurrLexeme()<<endl;
	cout<<"Script Line: "<<sTmpLine<<endl;
	bError = false;

	while(pLexer->getNextToken() != TOKEN_TYPE_DELIM_SEMICOLON);
	parseStatement();
//	exit(0);
}

void LSH_Parser::parsePrint() {
	if(iCurrScope == SCOPE_GLOBAL)
		exitOnCodeError("Print illegal in global scope");
	
	int iInstrIndex;
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());

	int iIsFirstParamString = FALSE;
	
	readToken(TOKEN_TYPE_DELIM_OPEN_PAREN);
	Token tNextToken = pLexer->getNextToken();
	switch (tNextToken) {
		case TOKEN_TYPE_INT:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, atoi(pLexer->getCurrLexeme()));
			break;
			
		case TOKEN_TYPE_FLOAT:
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
			funcTable.addFloatICodeOp(iCurrScope, iInstrIndex, atof(pLexer->getCurrLexeme()));
			break;
			
		case TOKEN_TYPE_IDENT: {
			LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope);
			if(pSymbol) {
				if(pLexer->getLookAheadChar() == '.') {
					structElmntToArrayIndex(pLexer->getCurrLexeme());
					
					//pop result into _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
					
					//push the original identifier, indexed with _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
					funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
				} else if(pLexer->getLookAheadChar() == '[') {
					if(pSymbol->iSize == 1)
						exitOnCodeError("Invalid array");
					
					readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
					if(pLexer->getLookAheadChar() == ']')
						exitOnCodeError("Invalid expression");
					
					parseExpr();
					
					readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
					
					//pop result into _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
					
					//push the original identifier, indexed with _T0
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
					funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
				} else {
					if(pSymbol->iSize != 1)
						exitOnCodeError("Arrays must be indexed");
					
					//otherwise print the variable
					iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
					funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
				}
			}
			break;
		}
		
		case TOKEN_TYPE_STRING:
			iIsFirstParamString = TRUE;
			break;
	
			
		default:
			exitOnCodeError("Invalid print value");
			break;
	}
	
	if(iIsFirstParamString) {
		string sTempString = pLexer->getCurrLexeme();
			
		size_t iCurrStringIndex = 0;	
		size_t iPrevStringPos = 0;

		for(iCurrStringIndex; iCurrStringIndex < sTempString.size(); ++iCurrStringIndex) {
			if(sTempString[iCurrStringIndex] == '%') {
				switch (sTempString[iCurrStringIndex+1]) {
					case 'i':
					case 'f':
					case 's': {
						readToken(TOKEN_TYPE_DELIM_COMMA);
						pLexer->getNextToken();
						Token tCurrtoken = pLexer->getCurrToken();
						if(tCurrtoken != TOKEN_TYPE_INT && tCurrtoken != TOKEN_TYPE_IDENT && tCurrtoken != TOKEN_TYPE_FLOAT)
							exitOnCodeError("Invalid print type");
						
						if(tCurrtoken == TOKEN_TYPE_IDENT) {
							//print previous string
							iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
							if(iPrevStringPos == 0)
								funcTable.addStringICodeOp(iCurrScope, iInstrIndex, stringTable.addString(sTempString.substr(iPrevStringPos, iCurrStringIndex-iPrevStringPos).c_str()));
							else
								funcTable.addStringICodeOp(iCurrScope, iInstrIndex, stringTable.addString(sTempString.substr(iPrevStringPos+2, iCurrStringIndex-iPrevStringPos-2).c_str()));
						
							//print the variable
							LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope);
							if(!pSymbol)
								exitOnCodeError("Undefined variable");

							if(pLexer->getLookAheadChar() == '.') {
								structElmntToArrayIndex(pLexer->getCurrLexeme());
								
								//pop result into _T0
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
								funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
								
								//push the original identifier, indexed with _T0
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
							} else if(pLexer->getLookAheadChar() == '[') {
								if(pSymbol->iSize == 1)
									exitOnCodeError("Invalid array");
								
								readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
								if(pLexer->getLookAheadChar() == ']')
									exitOnCodeError("Invalid expression");
								
								parseExpr();
								
								readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
								
								//pop result into _T0
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
								funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
								
								//push the original identifier, indexed with _T0
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
								funcTable.addArrayIndexVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex, iTempVar0SymbolIndex);
							} else {
								if(pSymbol->iSize != 1)
									exitOnCodeError("Arrays must be indexed");
								
								//otherwise print the variable
								iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
								funcTable.addVarICodeOp(iCurrScope, iInstrIndex, pSymbol->iIndex);
							}
														
							iPrevStringPos = iCurrStringIndex;
						}
						else {
							sTempString.replace(iCurrStringIndex, 2, pLexer->getCurrLexeme());
						}
						break;
					}
					
					default:
						exitOnCodeError("Invalid escape sequence type");
						break;
				}
			}
		}
		iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PRINT);
		if(iPrevStringPos != 0)
			funcTable.addStringICodeOp(iCurrScope, iInstrIndex, stringTable.addString(sTempString.substr(iPrevStringPos+2, sTempString.size()-iPrevStringPos-2).c_str()));
		else
			funcTable.addStringICodeOp(iCurrScope, iInstrIndex, stringTable.addString(sTempString.c_str()));
	}
	
	readToken(TOKEN_TYPE_DELIM_CLOSE_PAREN);

	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
}

void LSH_Parser::parsePound() {
	switch (pLexer->getNextToken()) {
		case TOKEN_TYPE_RSRVD_DEFINE: {
			string sSourceLine = pLexer->getCurrSourceLine();
			string sDefineName;
			string sDefineLiteral;
			
			if(pLexer->getNextToken() != TOKEN_TYPE_IDENT)
				exitOnCodeError("Ident expected");
			sDefineName = pLexer->getCurrLexeme();
			if(defineTable.isDefineExist(sDefineName))
				exitOnCodeError("Dupilate define symbol");
			
			Token tDefineType = pLexer->getNextToken();
			if(tDefineType != TOKEN_TYPE_STRING && tDefineType != TOKEN_TYPE_INT && tDefineType != TOKEN_TYPE_FLOAT && tDefineType != TOKEN_TYPE_IDENT)
				exitOnCodeError("Invalid define type");
			sDefineLiteral = pLexer->getCurrLexeme();
			
			defineTable.addDefineSymbol(sSourceLine, sDefineName, sDefineLiteral, tDefineType);
			
			replaceSourceDefineSymbols(sDefineName);
			
			readToken(TOKEN_TYPE_DELIM_SEMICOLON);
			break;
		}
			
		case TOKEN_TYPE_RSRVD_INCLUDE:
			break;
			
		default:
			exitOnCodeError("# unexpected");
			break;
	}
}

void LSH_Parser::replaceSourceDefineSymbols(const string& sDefineName) {
	string sDefineLiteral = defineTable.getDefineSymbol(sDefineName);
	if(sDefineLiteral.size() > 0) {
		for(size_t iCurrSourceIndex = 0; iCurrSourceIndex < vSourceCode.size(); ++iCurrSourceIndex) {
			int iDefineNameIndex = vSourceCode[iCurrSourceIndex].find(sDefineName);
			while(iDefineNameIndex != string::npos && !isCharIdentLex(vSourceCode[iCurrSourceIndex][iDefineNameIndex-1]) &&
				  vSourceCode[iCurrSourceIndex].find("#define") == string::npos	&& !isCharIdentLex(vSourceCode[iCurrSourceIndex][iDefineNameIndex+sDefineName.size()])) {
				vSourceCode[iCurrSourceIndex].replace(iDefineNameIndex, sDefineName.size(), sDefineLiteral);
				iDefineNameIndex = vSourceCode[iCurrSourceIndex].find(sDefineName);
			}
		}
	}
}

void LSH_Parser::parseStruct() {
	readToken(TOKEN_TYPE_IDENT);
	
	string sStructName(pLexer->getCurrLexeme());
	int iStructIndex = structTable.addStruct(sStructName);
	if(iStructIndex == -1)
		exitOnCodeError("Duplicate struct definition");
	
	readToken(TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE);
	
	while(true) {
		pLexer->getNextToken();
		if(pLexer->getCurrToken() == TOKEN_TYPE_RSRVD_VAR) {
			readToken(TOKEN_TYPE_IDENT);
			size_t iElmntSize = 1;
			
			string sElmntName = pLexer->getCurrLexeme();
			if(pLexer->getLookAheadChar() == '[') {
				readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
				readToken(TOKEN_TYPE_INT);
				
				iElmntSize = atoi(pLexer->getCurrLexeme());
				
				readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
			}
			
			structTable.addStructElmnt(iStructIndex, sElmntName.c_str(), iElmntSize);
		} else if(pLexer->getCurrToken() == TOKEN_TYPE_IDENT) {
			if(structTable.getStructByIdent(pLexer->getCurrLexeme())) {
				string sStructName = pLexer->getCurrLexeme();				
				readToken(TOKEN_TYPE_IDENT);
				
				structTable.addStructElmnt(iStructIndex, sStructName.c_str(), 1, pLexer->getCurrLexeme());

			} else 
				exitOnCodeError("Undefined symbol");
		} else 
			exitOnCodeError("Invalid token type");
		
		readToken(TOKEN_TYPE_DELIM_SEMICOLON);
		
		if(pLexer->getNextToken() == TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE)
			break;
		else 
			pLexer->rewindTokenStream();
	}
}

void LSH_Parser::parseStructAssign() {
	string sStructName = pLexer->getCurrLexeme();
	
	readToken(TOKEN_TYPE_IDENT);
	
	int iStructSize = structTable.getStructSize(sStructName);
	if(iStructSize == 0)
		exitOnCodeError("Undefined struct");
	
	//add a struct symbol
	symbolTable.addSymbol(pLexer->getCurrLexeme(), iStructSize, iCurrScope, SYMBOL_TYPE_STRUCT, sStructName.c_str());
	
	readToken(TOKEN_TYPE_DELIM_SEMICOLON);
}

void LSH_Parser::structElmntToArrayIndex(const string& sIdentName) {
	LSH_SymbolNode* pSymbol = symbolTable.getSymbolByIdent(sIdentName.c_str(), iCurrScope);
	if(!pSymbol)
		exitOnCodeError("Undefined variable");
	
	string sStructName = pSymbol->sStructName;
	LSH_Struct* pStruct = structTable.getStructByIdent(sStructName);
	if(!pStruct)
		exitOnCodeError("Undefined struct");
	
	readToken(TOKEN_TYPE_DELIM_POINT);
	readToken(TOKEN_TYPE_IDENT);
	
	string sPrevName;
	while(true) {
		//if is a array variable
		int iElmntOffset;
		string sElmntName = pLexer->getCurrLexeme();
		if(sPrevName.size() != 0)
			iElmntOffset= structTable.getElmntOffset(sStructName, pLexer->getCurrLexeme(), 0, sPrevName.c_str());
		else
			iElmntOffset= structTable.getElmntOffset(sStructName, pLexer->getCurrLexeme());
		
		if(iElmntOffset == -2) {
			readToken(TOKEN_TYPE_DELIM_OPEN_BRACE);
			parseExpr();
			readToken(TOKEN_TYPE_DELIM_CLOSE_BRACE);
			
			int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_POP);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_ADD);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			
			int iElmntOffset;
			if(sPrevName.size() != 0)
				iElmntOffset = structTable.getElmntOffset(sStructName, sElmntName.c_str(), 1, sPrevName.c_str());
			else
				iElmntOffset = structTable.getElmntOffset(sStructName, sElmntName.c_str(), 1);
			if(iElmntOffset == -1)
				exitOnCodeError("Undefined struct element");
			funcTable.addIntICodeOp(iCurrScope, iInstrIndex, iElmntOffset);
			
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, iTempVar0SymbolIndex);
			break;
		} else
			//not current struct elmnt, then should be a struct
			if(iElmntOffset == -1) {
				sPrevName = pLexer->getCurrLexeme();

				readToken(TOKEN_TYPE_DELIM_POINT);
				readToken(TOKEN_TYPE_IDENT);				
			}
			else {
				int iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_PUSH);
				funcTable.addIntICodeOp(iCurrScope, iInstrIndex, iElmntOffset);
				break;
			}
	}
}
	
void LSH_Parser::parseForwardOp() {
	int iCurrOpType = pLexer->getCurrOp();
	
	funcTable.addICodeSourceLine(iCurrScope, pLexer->getCurrSourceLine().c_str());
	
	int iInstrIndex;
	switch (iCurrOpType) {
		case OP_TYPE_INC:
			readToken(TOKEN_TYPE_IDENT);
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_INC);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope)->iIndex);
			readToken(TOKEN_TYPE_DELIM_SEMICOLON);
			break;
		
		case OP_TYPE_DEC:
			readToken(TOKEN_TYPE_IDENT);
			iInstrIndex = funcTable.addICodeInstr(iCurrScope, INSTR_DEC);
			funcTable.addVarICodeOp(iCurrScope, iInstrIndex, symbolTable.getSymbolByIdent(pLexer->getCurrLexeme(), iCurrScope)->iIndex);
			readToken(TOKEN_TYPE_DELIM_SEMICOLON);
			break;
			
		default:
			exitOnCodeError("Invalid Forward Operator");
			break;
	}
}

void LSH_Parser::initReservedFunctions() {
	int iFuncIndex;
	iFuncIndex = funcTable.addFunc("sin", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("cos", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("tan", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("sh", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("ch", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("th", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("ln", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("lg", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("rec", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("sqrt", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("expl", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 1);
	
	iFuncIndex = funcTable.addFunc("randomInt", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 2);
	
	iFuncIndex = funcTable.addFunc("randomFloat", FUNCTION_TYPE_RSRVD_INSTR);
	funcTable.setFuncParamCount(iFuncIndex, 2);
}

void LSH_Parser::combinePushPop(LSH_FuncNode* pFunc) {
	int iInstrStreamSize = pFunc->vICodeStream.size();
	// combine push&pop to mov
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {
			int iInstrType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode;
			if(iInstrType == INSTR_POP) {
				int iCurrPopIndex = iCurrInstrStreamIndex;
				int iCurrPushIndex = iCurrPopIndex;
				
				int iIsFuncCalled = FALSE;
				while(iCurrPushIndex > 0) {
					--iCurrPushIndex;
					
					if(pFunc->vICodeStream[iCurrPushIndex]->instr.iOpcode == INSTR_CALL || pFunc->vICodeStream[iCurrPushIndex]->iType == ICODE_NODE_JUMP_TARGET) {
						iIsFuncCalled = TRUE;
						break;
					}
					if(pFunc->vICodeStream[iCurrPushIndex]->iType == ICODE_NODE_INSTR &&
					   pFunc->vICodeStream[iCurrPushIndex]->instr.iOpcode == INSTR_PUSH) {
						break;
					}
				}
				
				if(!iIsFuncCalled && iCurrPushIndex != 0) {
				// if there's a pop right after a push instruction, combine them with a mov
					if(pFunc->vICodeStream[iCurrPushIndex]->instr.vOpList[0]->iType == OP_TYPE_VAR) {
						if(!isValueOperated(pFunc->vICodeStream[iCurrPushIndex]->instr.vOpList[0]->iSymbolIndex, iCurrPushIndex, iCurrPopIndex, pFunc)) {
							pFunc->vICodeStream[iCurrPushIndex]->iType = ICODE_NODE_NULL;
							pFunc->vICodeStream[iCurrPopIndex]->instr.iOpcode = INSTR_MOV;
									 
							funcTable.addVarICodeOp(pFunc->iIndex, iCurrPopIndex, pFunc->vICodeStream[iCurrPushIndex]->instr.vOpList[0]->iSymbolIndex);
						}
					}
					else  if(pFunc->vICodeStream[iCurrPushIndex]->instr.vOpList[0]->iType != OP_TYPE_VAR_ARRAY_INDEX) {
						pFunc->vICodeStream[iCurrPushIndex]->iType = ICODE_NODE_NULL;
						pFunc->vICodeStream[iCurrPopIndex]->instr.iOpcode = INSTR_MOV;
						
						LSH_Op* pOp = pFunc->vICodeStream[iCurrPushIndex]->instr.vOpList[0];
						switch (pOp->iType) {
							case OP_TYPE_INT:
								funcTable.addIntICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->iIntLiteral);
								break;
								
							case OP_TYPE_FLOAT:
								funcTable.addFloatICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->fFloatLiteral);
								break;
								
							case OP_TYPE_STRING_INDEX:
								funcTable.addStringICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->iStringIndex);
								break;
								
							case OP_TYPE_REG:
								funcTable.addRegICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->iRegCode);
								break;
								
							case OP_TYPE_ABS_ARRAY_INDEX:
								funcTable.addArrayIndexAbsICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->iSymbolIndex, pOp->iOffset);
								break;
								
							case OP_TYPE_VAR_ARRAY_INDEX:
								funcTable.addArrayIndexVarICodeOp(pFunc->iIndex, iCurrPopIndex, pOp->iSymbolIndex, pOp->iOffsetSymbolIndex);
								break;
								
							default:
								exitOnCodeError("Invalid push operator");
								break;
						}
					}
				}
			}
		}
	}
}

void LSH_Parser::combineMovs(LSH_FuncNode* pFunc) {
	int iInstrStreamSize = pFunc->vICodeStream.size();
	// combine movs
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {
			int iInstrType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode;
			
			if(iInstrType == INSTR_MOV) {
				int iTempIndex = iCurrInstrStreamIndex-1;

				while(iTempIndex > 0 && pFunc->vICodeStream[iTempIndex]->iType == ICODE_NODE_NULL) {
					--iTempIndex;
				}
			
				if(iTempIndex > 0) {
					if(pFunc->vICodeStream[iTempIndex]->instr.iOpcode == INSTR_MOV) {
						LSH_Op* pOp1 = pFunc->vICodeStream[iTempIndex]->instr.vOpList[0];
						LSH_Op* pOp2 = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1];
						
						if(isOpTemp(pOp1) && isOpTemp(pOp2) && cmpOp(pOp1, pOp2)) {		
							pFunc->vICodeStream[iTempIndex]->iType = ICODE_NODE_NULL;
							
							pOp1 = pFunc->vICodeStream[iTempIndex]->instr.vOpList[1];
							copyOp(pOp2, pOp1);
						}
					}
				}
			}
		}
	}
	
	// if there is two mov of the same value together, throw one
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {
			int iInstrType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode;
			
			if(iInstrType == INSTR_MOV) {
				int iTempIndex = iCurrInstrStreamIndex+1;				
				while(iTempIndex < iInstrStreamSize && pFunc->vICodeStream[iTempIndex]->iType == ICODE_NODE_NULL)
					++iTempIndex;
				
				if(iTempIndex >= iInstrStreamSize)
					break;
				
				if(pFunc->vICodeStream[iTempIndex]->instr.iOpcode == INSTR_MOV) {
					LSH_Op* pOp1 = pFunc->vICodeStream[iTempIndex]->instr.vOpList[0];
					
					if(cmpOp(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0], pOp1))						
						pFunc->vICodeStream[iCurrInstrStreamIndex]->iType = ICODE_NODE_NULL;
				}
			}
		}
	}

	// if there is two mov of the same value together, throw one
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {
			int iInstrType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode;
			
			if(iInstrType == INSTR_PUSHHOSTPARAM) {
				int iTempIndex = iCurrInstrStreamIndex-1;				
				while(iTempIndex > 0 && pFunc->vICodeStream[iTempIndex]->iType == ICODE_NODE_NULL)
					--iTempIndex;
				
				if(iTempIndex <= 0)
					break;
				
				if(pFunc->vICodeStream[iTempIndex]->instr.iOpcode == INSTR_MOV) {
					LSH_Op* pOp1 = pFunc->vICodeStream[iTempIndex]->instr.vOpList[0];
					
					if(cmpOp(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0], pOp1)) {
						pFunc->vICodeStream[iTempIndex]->iType = ICODE_NODE_NULL;
						copyOp(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0], pFunc->vICodeStream[iTempIndex]->instr.vOpList[1]);
					}
				}
				
			}
		}
	}
}

void LSH_Parser::combineTempVariables(LSH_FuncNode* pFunc) {
	int iInstrStreamSize = pFunc->vICodeStream.size();
	
	// combine temp variables
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {				
			int iOpSize = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList.size();
			int iCurrOpIndex = 1;
			
			if(iOpSize > 0 && pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0]->iType != OP_TYPE_REG) {
				for(iCurrOpIndex; iCurrOpIndex < iOpSize; ++iCurrOpIndex) {
					LSH_Op* pOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOpIndex];
					
					int iIsFound = FALSE;
					if(pOp->iType == OP_TYPE_VAR && (pOp->iSymbolIndex == iTempVar0SymbolIndex ||
													 pOp->iSymbolIndex == iTempVar1SymbolIndex ||
													 pOp->iSymbolIndex == iTempVar2SymbolIndex)) {
						int iForwardInstrIndex = iCurrInstrStreamIndex-1;
						if(iForwardInstrIndex <= 0)
							break;
						
						for(iForwardInstrIndex; iForwardInstrIndex > iCurrInstrStreamIndex-iOpSize-1; --iForwardInstrIndex) {
							if(pFunc->vICodeStream[iForwardInstrIndex]->iType == ICODE_NODE_INSTR) {
								if(pFunc->vICodeStream[iForwardInstrIndex]->instr.iOpcode == INSTR_MOV) {
									LSH_Op* pCurrOp = pFunc->vICodeStream[iForwardInstrIndex]->instr.vOpList[0];
									
									if(cmpOp(pCurrOp, pOp)) {
										pCurrOp = pFunc->vICodeStream[iForwardInstrIndex]->instr.vOpList[1];											
										copyOp(pOp, pCurrOp);
										iIsFound = TRUE;

										pFunc->vICodeStream[iForwardInstrIndex]->iType = ICODE_NODE_NULL;
										break;
									}
								} else break;
							} else if(pFunc->vICodeStream[iForwardInstrIndex]->iType == ICODE_NODE_JUMP_TARGET)
								break;
						}
					}
				}
			}
		}
	} 
	

	
	// combine temp variables
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {		
			if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode == INSTR_MOV) {
				if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0]->iType != OP_TYPE_REG) {
					LSH_Op* pCurrOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0];
					if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1]->iType == OP_TYPE_VAR) {				
						int iOpSymbolIndex = pCurrOp->iSymbolIndex;
						if(iOpSymbolIndex == iTempVar0SymbolIndex || iOpSymbolIndex == iTempVar1SymbolIndex || iOpSymbolIndex == iTempVar2SymbolIndex) {
							int iCurrInstrIndex = iCurrInstrStreamIndex;
							int iIsJump = FALSE;
							while(iCurrInstrIndex < iInstrStreamSize-1) {
								++iCurrInstrIndex;
								while (pFunc->vICodeStream[iCurrInstrIndex]->iType == ICODE_NODE_NULL) {
									++iCurrInstrIndex;
								}
								
								if(pFunc->vICodeStream[iCurrInstrIndex]->iType == ICODE_NODE_JUMP_TARGET ||
								   pFunc->vICodeStream[iCurrInstrIndex]->instr.iOpcode == INSTR_CALL ||
								   pFunc->vICodeStream[iCurrInstrIndex]->instr.iOpcode == INSTR_CALLHOST) {
									iIsJump = TRUE;
									break;
								}
								
								if(pFunc->vICodeStream[iCurrInstrIndex]->iType == ICODE_NODE_INSTR) {
									if(pFunc->vICodeStream[iCurrInstrIndex]->instr.iOpcode == INSTR_MOV) {
										if(pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[1]->iType == OP_TYPE_VAR) {
											int iNextSymbolIndex = pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[1]->iSymbolIndex;
											if(iOpSymbolIndex != iNextSymbolIndex ||
											   pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1]->iSymbolIndex != pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[0]->iSymbolIndex)
												iIsJump = TRUE;
											break;
										} else if(pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[1]->iType == OP_TYPE_ABS_ARRAY_INDEX ||
												  pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[1]->iType == OP_TYPE_VAR_ARRAY_INDEX)
											iIsJump = TRUE;
									} else {
										for(size_t iTempIndex = 0; iTempIndex < pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList.size(); ++iTempIndex) {
											if(cmpOp(pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[iTempIndex], pCurrOp))
												iIsJump = TRUE;
										}
									}
								}
							}
							if(!iIsJump) {						
								for(size_t iMidInstrIndex = iCurrInstrStreamIndex+1; iMidInstrIndex < iCurrInstrIndex; ++iMidInstrIndex) {
									for(size_t iMidOpIndex = 0; iMidOpIndex < pFunc->vICodeStream[iMidInstrIndex]->instr.vOpList.size(); ++iMidOpIndex) {
										if(cmpOp(pFunc->vICodeStream[iMidInstrIndex]->instr.vOpList[iMidOpIndex], pCurrOp)) {
											copyOp(pFunc->vICodeStream[iMidInstrIndex]->instr.vOpList[iMidOpIndex], pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1]);
										}
									}
								}
								
								pFunc->vICodeStream[iCurrInstrIndex]->iType = ICODE_NODE_NULL;
								pFunc->vICodeStream[iCurrInstrStreamIndex]->iType = ICODE_NODE_NULL;
							}
						}
					}
				}
			}
			
		}
	}
	
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {		
			int iOpType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode;
			if(iOpType == INSTR_JE || iOpType == INSTR_JNE || iOpType == INSTR_JMP || iOpType == INSTR_JG ||
			   iOpType == INSTR_JGE || iOpType == INSTR_JL || iOpType == INSTR_JLE) {
				LSH_Op* pTempVarOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0];
				if(pTempVarOp->iType == OP_TYPE_VAR) {
					if(pTempVarOp->iSymbolIndex == iTempVar0SymbolIndex || pTempVarOp->iSymbolIndex == iTempVar1SymbolIndex) {
						int iCurrInstrIndex = iCurrInstrStreamIndex-1;
						if(pFunc->vICodeStream[iCurrInstrIndex]->iType == ICODE_NODE_INSTR) {
							if(pFunc->vICodeStream[iCurrInstrIndex]->instr.iOpcode == INSTR_MOV) {
								if(pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[0]->iType == OP_TYPE_VAR) {
									if(pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[0]->iSymbolIndex == pTempVarOp->iSymbolIndex) {
										copyOp(pTempVarOp, pFunc->vICodeStream[iCurrInstrIndex]->instr.vOpList[1]);
										pFunc->vICodeStream[iCurrInstrIndex]->iType = ICODE_NODE_NULL;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void LSH_Parser::removeUselessMoves(LSH_FuncNode* pFunc) {
	int iInstrStreamSize = pFunc->vICodeStream.size();
	// remove useless moves
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {		
			if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode == INSTR_MOV) {
				LSH_Op* pOp1 = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0];
				LSH_Op* pOp2 = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1];
				if(cmpOp(pOp1, pOp2))
					pFunc->vICodeStream[iCurrInstrStreamIndex]->iType = ICODE_NODE_NULL;
			}
		}
	}
	
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {	
			int iInstrOpSize = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList.size();
			for(int iCurrOp = 0; iCurrOp < iInstrOpSize; ++iCurrOp) {
				int iOpType = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iType;
				if(iOpType == OP_TYPE_VAR_ARRAY_INDEX) {
					int iOffsetIndex = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iOffsetSymbolIndex;
					if(pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.iOpcode == INSTR_MOV) {
						if(pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[0]->iSymbolIndex == iOffsetIndex) {	
							int iSourceType = pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]->iType;
							
							if(iSourceType == OP_TYPE_VAR) {
								pFunc->vICodeStream[iCurrInstrStreamIndex-1]->iType = ICODE_NODE_NULL;
								
								pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iOffsetSymbolIndex = pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]->iSymbolIndex;
							} else if(iSourceType != OP_TYPE_ABS_ARRAY_INDEX && iSourceType != OP_TYPE_VAR_ARRAY_INDEX) {
								pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iType = OP_TYPE_ABS_ARRAY_INDEX;
								
								pFunc->vICodeStream[iCurrInstrStreamIndex-1]->iType = ICODE_NODE_NULL;
								
								switch (iSourceType) {
									case OP_TYPE_INT:
										pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iOffset = pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]->iIntLiteral;
										break;
										
									case OP_TYPE_FLOAT:
										pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iOffset = (int)pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]->fFloatLiteral;
										break;
										
									case OP_TYPE_STRING_INDEX:
										pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[iCurrOp]->iOffset = atoi(stringTable.getStringByIndex(pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]->iStringIndex).c_str());
										break;
										
									default:
										exitOnCodeError("Invalid Array Index");
										break;
								}
							}
						}
					}
				}
			}
		}
	}
	
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {	
			if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode == INSTR_PUSH) {
				if(iCurrInstrStreamIndex-1 > 0) {
					if(pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.iOpcode == INSTR_MOV) {
						if(cmpOp(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0], pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[0])) {
							pFunc->vICodeStream[iCurrInstrStreamIndex-1]->iType = ICODE_NODE_NULL;
							copyOp(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0], pFunc->vICodeStream[iCurrInstrStreamIndex-1]->instr.vOpList[1]);
						}
					}
				}
			}
			
		}
	}
}

void LSH_Parser::calculateExpressions(LSH_FuncNode* pFunc) {
	int iInstrStreamSize = pFunc->vICodeStream.size();
	// caculate pure number caculations
	for(int iCurrInstrStreamIndex = 0; iCurrInstrStreamIndex < iInstrStreamSize; ++iCurrInstrStreamIndex) {
		if(pFunc->vICodeStream[iCurrInstrStreamIndex]->iType == ICODE_NODE_INSTR) {	
			//coerce un-string operators to string
			if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode == INSTR_CONCAT) {
				LSH_Op* pOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1];
				if(pOp->iType != OP_TYPE_STRING_INDEX && pOp->iType != OP_TYPE_VAR && pOp->iType != OP_TYPE_ABS_ARRAY_INDEX && pOp->iType != OP_TYPE_VAR_ARRAY_INDEX) {
					string sTempString;
					switch (pOp->iType) {
						case OP_TYPE_INT:
							sTempString = int_to_str(pOp->iIntLiteral);
							break;

						case OP_TYPE_FLOAT:
							sTempString = fp_to_str(pOp->fFloatLiteral);
							break;

						default: {
							string sErrorMssg("Invalid concat operator: concat ");
							sErrorMssg += symbolTable.getSymbolByIndex(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0]->iSymbolIndex)->sIdent;
							sErrorMssg += "\n";
							sErrorMssg += "Line: ";
							sErrorMssg += int_to_str(iCurrInstrStreamIndex);
							exitOnCodeError(sErrorMssg.c_str());
							break;
								 }
					}
					int iStringIndex = stringTable.addString(sTempString.c_str());
					if(iStringIndex == -1)
						exitOnCodeError("Error adding string to string table");

					pOp->iType = OP_TYPE_STRING_INDEX;
					pOp->iStringIndex = iStringIndex;
				}
			}

			if(pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.iOpcode == INSTR_MOV) {
				LSH_Op* pOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0];
				if(pOp->iSymbolIndex == iTempVar0SymbolIndex) {

					pOp = pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1];
					if(pOp->iType == OP_TYPE_INT || pOp->iType == OP_TYPE_FLOAT) {
						float fMidResult;
						if(pOp->iType == OP_TYPE_INT)
							fMidResult = (float)pOp->iIntLiteral;
						else
							fMidResult = pOp->fFloatLiteral;

						pOp	= pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[0];
						if(pFunc->vICodeStream[iCurrInstrStreamIndex+1]->iType == ICODE_NODE_INSTR) {
							int iTempInstrIndex = iCurrInstrStreamIndex+1;
							if(iTempInstrIndex >= iInstrStreamSize-1)
								break;

							int iIsStop = FALSE;
							while(pFunc->vICodeStream[iTempInstrIndex]->iType == ICODE_NODE_INSTR &&
								pFunc->vICodeStream[iTempInstrIndex]->instr.iOpcode != INSTR_MOV &&
								cmpOp(pOp, pFunc->vICodeStream[iTempInstrIndex]->instr.vOpList[0])) {

									if(iIsStop || iTempInstrIndex > pFunc->vICodeStream.size())
										break;


									if(pFunc->vICodeStream[iTempInstrIndex]->instr.vOpList.size() == 2) {
										LSH_Op* pTempOp = pFunc->vICodeStream[iTempInstrIndex]->instr.vOpList[1];

										if(isOpVar(pTempOp)) {
											iIsStop = TRUE;
											break;
										}

										int iInstrType = pFunc->vICodeStream[iTempInstrIndex]->instr.iOpcode;
										switch (iInstrType) {
										case INSTR_ADD:
											fMidResult += funcTable.coerceICodeOpToFloat(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_SUB:
											fMidResult -= funcTable.coerceICodeOpToFloat(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_MUL:
											fMidResult *= funcTable.coerceICodeOpToFloat(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_DIV:
											fMidResult /= funcTable.coerceICodeOpToFloat(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_MOD:
											fMidResult = (int)fMidResult % funcTable.coerceICodeOpToInt(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_EXP:
											pow(fMidResult, funcTable.coerceICodeOpToFloat(pTempOp));
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_SHL:
											fMidResult = (int)fMidResult << funcTable.coerceICodeOpToInt(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_SHR:
											fMidResult = (int)fMidResult >> funcTable.coerceICodeOpToInt(pTempOp);
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										default:
											iIsStop = TRUE;
											break;
										}
									} else if(pFunc->vICodeStream[iTempInstrIndex]->instr.vOpList.size() == 1) {
										int iInstrType = pFunc->vICodeStream[iTempInstrIndex]->instr.iOpcode;
										switch (iInstrType) {
										case INSTR_NEG:
											fMidResult = -fMidResult;
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_INC:
											++fMidResult;
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										case INSTR_DEC:
											--fMidResult;
											pFunc->vICodeStream[iTempInstrIndex]->iType = ICODE_NODE_NULL;
											break;

										default:
											iIsStop = TRUE;
											break;
										}
									}							
									++iTempInstrIndex;
									while(pFunc->vICodeStream[iTempInstrIndex]->iType == ICODE_NODE_NULL)
										++iTempInstrIndex;
							}

							pOp	= pFunc->vICodeStream[iCurrInstrStreamIndex]->instr.vOpList[1];
							if(pOp->iType == OP_TYPE_INT)
								pOp->iIntLiteral = (int)fMidResult;
							else
								pOp->fFloatLiteral = fMidResult;

							//	for(int iMidInstrIndex = iCurrInstrStreamIndex+1; iMidInstrIndex < iTempInstrIndex-1; ++iMidInstrIndex)
							//		pFunc->vICodeStream[iMidInstrIndex]->iType = ICODE_NODE_NULL;
							iCurrInstrStreamIndex += (iTempInstrIndex-iCurrInstrStreamIndex-1);
						}
					}
				}
			}
		}
	}
}	

bool isNodeNull(LSH_ICodeNode* node) {
	return (node->iType == ICODE_NODE_NULL);
}

void LSH_Parser::removeNullNodes(LSH_FuncNode* pFunc) {
	pFunc->vICodeStream.erase(remove_if(pFunc->vICodeStream.begin(), pFunc->vICodeStream.end(), isNodeNull), pFunc->vICodeStream.end());
}
	
void LSH_Parser::optimizeInstrStream(int iLoopTime) {
	int iFuncTableSize = funcTable.size();
	for(int iCurrFuncIndex = 0; iCurrFuncIndex < iFuncTableSize; ++iCurrFuncIndex) {
		LSH_FuncNode* pFunc = funcTable.getFuncByIndex(iCurrFuncIndex);
		if(!pFunc)
			break;
		
		for(int iCurrLoopTime = 0; iCurrLoopTime < iLoopTime; ++iCurrLoopTime) {
			combinePushPop(pFunc);
		

			combineMovs(pFunc);

			combineTempVariables(pFunc);

			removeNullNodes(pFunc);

			removeUselessMoves(pFunc);

			removeNullNodes(pFunc);

			calculateExpressions(pFunc);
		
		}
	}
}

bool LSH_Parser::isValueOperated(int iSymbolIndex, int iStartIndex, int iEndIndex, LSH_FuncNode* pFunc) {
	for(int iCurrIndex = iStartIndex+1; iCurrIndex < iEndIndex; ++iCurrIndex) {
		if(pFunc->vICodeStream[iCurrIndex]->iType == ICODE_NODE_INSTR) {
			if(pFunc->vICodeStream[iCurrIndex]->instr.iOpcode == INSTR_CALL) {
				if(!symbolTable.getSymbolByIndex(iSymbolIndex)->iScope == pFunc->iIndex)
					return true;
			}
			
			for(size_t iCurrOpIndex = 0; iCurrOpIndex < pFunc->vICodeStream[iCurrIndex]->instr.vOpList.size(); ++iCurrOpIndex) {
				if(pFunc->vICodeStream[iCurrIndex]->instr.vOpList[iCurrOpIndex]->iType == OP_TYPE_VAR) {
					if(pFunc->vICodeStream[iCurrIndex]->instr.vOpList[iCurrOpIndex]->iSymbolIndex == iSymbolIndex)
						return true;
				} else if(pFunc->vICodeStream[iCurrIndex]->instr.vOpList[iCurrOpIndex]->iType == OP_TYPE_VAR_ARRAY_INDEX) {
					if(pFunc->vICodeStream[iCurrIndex]->instr.vOpList[iCurrOpIndex]->iOffsetSymbolIndex == iSymbolIndex)
						return true;
				}
			}
		}
	}
	return false;
}

bool LSH_Parser::cmpOp(const LSH_Op* op1, const LSH_Op* op2) {
	switch (op1->iType) {
		case OP_TYPE_VAR:
			if(op2->iType != OP_TYPE_VAR)
				return false;
			return (op1->iSymbolIndex == op2->iSymbolIndex);
			break;
			
		case OP_TYPE_ABS_ARRAY_INDEX:
			if(op2->iType != OP_TYPE_ABS_ARRAY_INDEX)
				return false;
			if(op1->iSymbolIndex == op2->iSymbolIndex &&
			   op1->iOffset == op2->iOffset)
				return true;
			return false;
			
		case OP_TYPE_VAR_ARRAY_INDEX:
			if(op2->iType != OP_TYPE_VAR_ARRAY_INDEX)
				return false;
			if(op1->iSymbolIndex == op2->iSymbolIndex &&
			   op1->iOffsetSymbolIndex == op2->iOffsetSymbolIndex)
				return true;
			return false;
			
		default:
			return false;
			break;
	}
	return false;
}

bool LSH_Parser::isOpVar(const LSH_Op* op) {
	if(op->iType == OP_TYPE_VAR ||
	   op->iType == OP_TYPE_ABS_ARRAY_INDEX ||
	   op->iType == OP_TYPE_VAR_ARRAY_INDEX)
		return true;
	return false;
}

bool LSH_Parser::isOpTemp(const LSH_Op* op) {
	if(op->iType == OP_TYPE_VAR &&
	   (op->iSymbolIndex == iTempVar0SymbolIndex ||
	   op->iSymbolIndex == iTempVar1SymbolIndex))
		return true;
	return false;
}

void LSH_Parser::copyOp(LSH_Op* dest, LSH_Op* source) {
	dest->iType = source->iType;
	switch (source->iType) {
		case OP_TYPE_VAR:
			dest->iSymbolIndex = source->iSymbolIndex;
			break;
			
		case OP_TYPE_INT:
			dest->iIntLiteral = source->iIntLiteral;
			break;
			
		case OP_TYPE_FLOAT:
			dest->fFloatLiteral = source->fFloatLiteral;
			break;
			
		case OP_TYPE_STRING_INDEX:
			dest->iStringIndex = source->iStringIndex;
			break;
			
		case OP_TYPE_REG:
			dest->iRegCode = source->iRegCode;
			break;
			
		case OP_TYPE_ABS_ARRAY_INDEX:
			dest->iSymbolIndex = source->iSymbolIndex;
			dest->iOffset = source->iOffset;
			break;
			
		case OP_TYPE_VAR_ARRAY_INDEX:
			dest->iSymbolIndex = source->iSymbolIndex;
			dest->iOffsetSymbolIndex = source->iOffsetSymbolIndex;
			break;
	}
}