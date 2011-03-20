/*
 *  compiler.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月9日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */
#pragma once

#include "compiler_global.h"

#include <vector>

using std::vector;

#define PRIORITY_NONE	-1
#define PRIORITY_USER	0
#define PRIORITY_LOW	1
#define PRIORITY_MED	2
#define PRIORITY_HIGH	3

#define PRIORITY_LOW_KEYWORD	"LOW"
#define PRIORITY_MED_KEYWORD	"MEDIUM"
#define PRIORITY_HIGH_KEYWORD	"HIGH"

#include "parser.h"

typedef struct _file_IncludeList {
	string sFileName;
	
	vector<string> vIncludeFileList;
} file_IncludeList;

typedef enum SOURCE_FILE_LOAD_ERROR {
	SOURCE_FILE_LOAD_ERROR_DUPLICATE_INCLUDE,
	SOURCE_FILE_LOAD_ERROR_IO_ERROR,
	SOURCE_FILE_LOAD_SUCCESS,
};

typedef enum RSRVD_IDENT_TYPE {
	RSRVD_IDENT_TYPE_UNKNOWN,
	RSRVD_IDENT_TYPE_INCLUDE,
	RSRVD_IDENT_TYPE_DEFINE,
};

class LSH_Compiler {
public:	
	friend class LSH_Parser;
	
	LSH_Compiler();
	~LSH_Compiler();
	
	int loadSourceFile(const char* pstrSourceFile);

	int compileSourceFile();
	
	void emitCode(const char* pstrOutputFileName);	
	
private:
	void preprocessSourceFile();
	
	void emitHeader();
	void emitDirectives();
	void emitScopeSymbols(int iScope, int iType);
	void emitFunc(LSH_FuncNode* pFunc);
	
	SOURCE_FILE_LOAD_ERROR  doLoadSourceFile(const string& sFileName);
	RSRVD_IDENT_TYPE		getReservedIdentType(const string& sSourceLine);
	
	file_IncludeList includedFile;
		
	FILE*  pOutputFile;
	string sOutputFileName;
	string sSourceFileName;
	
	LSH_Parser* pParser;
};