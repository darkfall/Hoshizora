/*
 *  compiler_defineTable.cpp
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 10年1月15日.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#include "compiler_defineTable.h"

int LSH_DefineTable::isDefineExist(const string& sDefineName) {
	size_t iSize = 0;
	for(iSize; iSize < vDefineList.size(); ++iSize) {
		if(vDefineList[iSize].sDefineName.compare(sDefineName) == 0)
			return TRUE;
	}
	return FALSE;
}

void LSH_DefineTable::addDefineSymbol(const string& sSourceLine, const string& sDefineName, const string& sDefineLiteral, Token tType) {
	LSH_Define define;
	define.sSourceLine = sSourceLine;
	define.sDefineName = sDefineName;
	define.tDefineType = tType;
	define.sDefineLiteral = sDefineLiteral;
	vDefineList.push_back(define);
}

int LSH_DefineTable::getDefineSymbolAsInt(const string& sDefineName) {
	vector<LSH_Define>::iterator p = getDefineByName(sDefineName);
	if(p == vDefineList.end())
		return 0;
	return atoi(p->sDefineLiteral.c_str());
}

float LSH_DefineTable::getDefineSymbolAsFloat(const string& sDefineName) {
	vector<LSH_Define>::iterator p = getDefineByName(sDefineName);
	if(p == vDefineList.end())
		return 0.f;
	return atof(p->sDefineName.c_str());
}

string LSH_DefineTable::getDefineSymbolAsString(const string& sDefineName) {
	vector<LSH_Define>::iterator p = getDefineByName(sDefineName);
	if(p == vDefineList.end())
		return string("\0");
	return p->sDefineLiteral;
}

string LSH_DefineTable::getDefineSymbol(const string& sDefineName) {
	vector<LSH_Define>::iterator p = getDefineByName(sDefineName);
	if(p == vDefineList.end())
		return string("\0");
	return p->sDefineLiteral;
}

string LSH_DefineTable::getDefineSourceLine(const string& sDefineName) {
	vector<LSH_Define>::iterator p = getDefineByName(sDefineName);
	if(p == vDefineList.end())
		return string("\0");
	return p->sSourceLine;
}

vector<LSH_Define>::iterator LSH_DefineTable::getDefineByName(const string& sDefineName) {
	vector<LSH_Define>::iterator p = vDefineList.begin();
	for(p; p != vDefineList.end(); ++p) {
		if(p->sDefineName.compare(sDefineName) == 0)
			return p;
	}
	return vDefineList.end();
}