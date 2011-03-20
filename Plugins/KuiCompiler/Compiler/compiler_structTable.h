/*
 *  compiler_structTable.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月27日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _COMPILER_STRUCTTABLE_H_
#define _COMPILER_STRUCTTABLE_H_

#define INVALID_STRUCT -1

#include "compiler_global.h"

#include <iostream>
#include <string>
#include <vector>

#define LSH_STRUCTTABLE_SUCCESS			0
#define LSH_STRUCTTABLE_DUPLICATEELMNT	1
#define LSH_STRUCTTABLE_INVALID_INDEX	2

using namespace std;

typedef struct _LSH_StructElmnt {
	string sElmntName;
	
	size_t iSize;
	
	size_t iIndex;
	
	string sOriginatedName;
	
	_LSH_StructElmnt(): iSize(1) {}
} LSH_StructElmnt;

typedef struct _LSH_Struct {
	vector<LSH_StructElmnt> vElmnts;
	
	string sStructName;
	
	int iRealSize;
} LSH_Struct;

class LSH_StructTable {
public:
	LSH_StructTable() {}
	~LSH_StructTable() {
		for(size_t i=0; i<vStructs.size(); ++i)
			delete vStructs[i];
		vStructs.clear(); 
	}
	
	int		addStruct(const string& sName);
	int 	addStructElmnt(int iStructIndex, const string& sElmntName, size_t iSize=1, const char* pstrOriginatedName=0);
	
	int		getElmntOffset(const string& sStructName, const string& sElmntName, size_t iIndex=0,  const char* pstrOriginatedName=0);
	int		getStructSize(const string& sName);
	
	LSH_Struct*	getStructByIdent(const string& sName);
	
private:
	bool		isStructExist(const string& sName);
	
	bool		isElmntExist(int iStructIndex, const string& sElmntName);
	bool		isElmntStruct(const string& sElmntName);
	
	vector<LSH_Struct*> vStructs;
};

#endif