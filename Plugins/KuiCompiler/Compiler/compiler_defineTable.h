/*
 *  compiler_defineTable.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月15日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _COMPILER_DEFINETABLE_H_
#define _COMPILER_DEFINETABLE_H_

#include <vector>
#include <string>
#include <algorithm>

#include "compiler_global.h"

using namespace std;

typedef enum {
	LSH_DEFINE_TYPE_INT,
	LSH_DEFINE_TYPE_FLOAT,
	LSH_DEFINE_TYPE_STRING,
} LSH_DEFINE_TYPE;

#ifndef TOKEN
typedef int Token;
#endif

class LSH_Define {
public:
	string sSourceLine;
	string sDefineName;
	
	Token tDefineType;
	
	string sDefineLiteral;
};

class LSH_DefineTable {
public:
	LSH_DefineTable() {}
	~LSH_DefineTable() {}
	
	int isDefineExist(const string& sDefineName);
	
	void addDefineSymbol(const string& sSourceLine, const string& sDefineName, const string& sDefineLiteral, Token tType);
	
	int		getDefineSymbolAsInt(const string& sDefineName);
	float	getDefineSymbolAsFloat(const string& sDefineName);
	string	getDefineSymbolAsString(const string& sDefineName);
	
	string	getDefineSourceLine(const string& sDefineName);
	
	string  getDefineSymbol(const string& sDefineName);
	
private:
	vector<LSH_Define>::iterator getDefineByName(const string& sDefineName);
	
	vector<LSH_Define> vDefineList;
};
	

#endif