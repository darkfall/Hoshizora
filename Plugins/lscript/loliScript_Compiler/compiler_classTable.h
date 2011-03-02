/*
 *  compiler_classTable.h
 *  loliScript_Compiler
 *
 *  Created by griffin clare on 4/22/10.
 *  Copyright 2010 ClareGriffin(ring0Dev). All rights reserved.
 *
 */

#ifndef _COMPILER_CLASSTABLE_H_
#define _COMPILER_CLASSTABLE_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// prototype for Project STM Win32
//  - void hostFunc(int iThreadIndex, int iManagerID)
typedef void (*pHostFunc)(int, int);

#define CLASSVAR_TYPE_NULL 0
#define CLASSVAR_TYPE_INT 1
#define CLASSVAR_TYPE_FLOAT 2
#define CLASSVAR_TYPE_STRING 3

typedef struct _LSH_ClassVar {
	union {
		int iInt;
		float fFloat;
		char* pString;
	};
	int iVarType;
	
	_LSH_ClassVar(): iVarType(0) {}
} LSH_ClassVar;

typedef struct _LSH_HostFunc {
	string sFuncName;
	
	pHostFunc pFunc;
} LSH_HostFunc;

//template<typename T>
typedef struct _LSH_Class {
	string sClassName;
	
	size_t iRealSize;
	
	string sInheritateClass;
	
	vector<LSH_ClassVar> vVarList;
	vector<LSH_HostFunc> vFuncList;
	
	/*  prototype
			// returns the class identifier
	 *		- String GetReferenceName();
	 
			//
	 *	
	 */ 
//	T* pRealClass;
	
} LSH_Class;

// a class prototype for Project STM 
// only works for host function call & host variables
// script parsing not available


// Since this is a module class
// We don't need this to run in multithread or something
// So, Singleton is a good choice
#include "Singleton.h"

class LSH_Module_ClassTable: public mSingleton<LSH_Module_ClassTable> {
	friend class mSingleton<LSH_Module_ClassTable>;
	
protected:
	LSH_Module_ClassTable();
	~LSH_Module_ClassTable();
	
public:
	int AddClass(const string& sName);
	void AddClassFunc(const string& sFunctionName, pHostFunc which);
	
	void AddClassIntVar(const string& sClassName, const string& sVarName, int iDefault);
	void AddClassFloatVar(const string& sClassName, const string& sVarName, float fDefault);
	void AddClassStringVar(const string& sClassName, const string& sVarName, const string& sDefault);
	
	int SetClassIntVar(const string& sClassName, const string& sVarName, int iVar);
	int SetClassFloatVar(const string& sClassName, const string& sVarName, float fVar);
	int SetClassStringVar(const string& sClassName, const string& sVarName, const string& sVar);
	
	void AddClassFunctionRef(const string& sClassName, const string& sFuncName);
	int CallClassFunction(const string& sClassName, const string& sFuncName);

private:
	vector<LSH_Class> vClasses;
};

// base class for classes available in script
// receive different messages when a specified event happened in script
class LSH_Module_ClassFunctionCaller {
protected:
	LSH_Module_ClassFunctionCaller();
	~LSH_Module_ClassFunctionCaller();
	
public:
	// to do
	void ReceiveCall(const string& sClassName, const string& sFunctionName);
	void ReceiveIntVarAssign(const string& sClassName, const string& sVarName, int iVal);
	void ReceiveFloatVarAssign(const string& sClassName, const string& sVarName, float fVal);
	void ReceiveStringVarAssign(const string& sClassName, const string& sVarName, const string& sVal);
	
private:
	
};
#endif