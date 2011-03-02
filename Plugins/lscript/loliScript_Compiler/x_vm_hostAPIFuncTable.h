#ifndef X_VM_HOSTAPIFUNCTABLE_H
#define X_VM_HOSTAPIFUNCTABLE_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

#include "vm_defines.h"

/* 
	this is a global hostAPI func table
    all registered host API functions are available for all scripts
*/

typedef struct _x_vm_hostAPIFuncInfo {
	string sFuncName;
	HostAPIFuncPntr pFunc;

	_x_vm_hostAPIFuncInfo(const string& funcName, HostAPIFuncPntr func): sFuncName(funcName), pFunc(func) {}
	_x_vm_hostAPIFuncInfo(): pFunc(0) {}
} x_vm_hostAPIFuncInfo;

typedef map<string, x_vm_hostAPIFuncInfo>::iterator itFuncInfo;

class x_vm_hostAPIFuncTable {


protected:
	x_vm_hostAPIFuncTable();
	~x_vm_hostAPIFuncTable();
	
public:
	static x_vm_hostAPIFuncTable* getInstance();
	static void destroy();

	void registerHostAPIFunc(const string& funcName, HostAPIFuncPntr pFunc);

	int  callHostAPIFunc(const string& funcName, int iManagerID, int iScriptID);

private:
	itFuncInfo getHostAPIFuncInfo(const string& funcName);

	static x_vm_hostAPIFuncTable* pInstance;

	map<string, x_vm_hostAPIFuncInfo> vHostAPIFuncs;	
};

#endif