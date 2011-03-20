/*
 *  lex_error.cpp
 *  loliScript_lexer
 *
 *  Created by griffin clare on 10年1月7日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "lex_errors.h"

#include "stdlib.h"

#include <iostream>

using namespace std;

//#include "../../KuiWin32/headers/xLog.h"

void Lex_ExitOnInvalidInputError(char cInput, int iLineIndex, const string& sScriptLine) {
	cout<<"Lexer Error: '"<<cInput<<"' unexpected."<<endl;
	cout<<"CodeLine: "<<sScriptLine<<endl;
	
	string sTmpLine(sScriptLine);
	for(size_t i=0; i<sTmpLine.size(); ++i) {
		if(sTmpLine[i] == '\t' || sTmpLine[i] == '\n')
			sTmpLine.erase(sTmpLine.begin()+i);
	}
//	exitWithMssgBox(-1 ,"Lexer Error: '%c' unexpected.\nScript Index: %d\nScript Line: %s\n", cInput, iLineIndex, sScriptLine.c_str());
}
