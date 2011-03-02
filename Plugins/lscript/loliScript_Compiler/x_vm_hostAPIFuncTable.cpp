#include "x_vm_hostAPIFuncTable.h"
#include "lsLog.h"

x_vm_hostAPIFuncTable* x_vm_hostAPIFuncTable::pInstance = 0;
x_vm_hostAPIFuncTable::x_vm_hostAPIFuncTable() {}
x_vm_hostAPIFuncTable::~x_vm_hostAPIFuncTable() {}

x_vm_hostAPIFuncTable* x_vm_hostAPIFuncTable::getInstance() {
	if(!pInstance)
		pInstance = new x_vm_hostAPIFuncTable;
	return pInstance;
}

void x_vm_hostAPIFuncTable::destroy() {
	if(pInstance)
		delete pInstance;
}

void x_vm_hostAPIFuncTable::registerHostAPIFunc(const string& funcName, HostAPIFuncPntr pFunc) {
//	vHostAPIFuncs.push_back(x_vm_hostAPIFuncInfo(funcName, pFunc));
	vHostAPIFuncs[strupr(const_cast<char*>(funcName.c_str()))] = x_vm_hostAPIFuncInfo(funcName, pFunc);
}

int x_vm_hostAPIFuncTable::callHostAPIFunc(const string& funcName, int iManagerID, int iScriptID) {
	itFuncInfo itInfo = vHostAPIFuncs.find(strupr(const_cast<char*>(funcName.c_str())));
	if(itInfo == vHostAPIFuncs.end())
		return -1;
	itInfo->second.pFunc(iScriptID, iManagerID);
	return TRUE;
}

itFuncInfo x_vm_hostAPIFuncTable::getHostAPIFuncInfo(const string& funcName) {
//  vs2008 not support bind2nd&bind1st?
//	itFuncInfo itInfo = find_if(vHostAPIFuncs.begin(), vHostAPIFuncs.end(), std::bind1st(std::equal<string>(), funcName));
/*	itFuncInfo p = vHostAPIFuncs.begin();
	while(p != vHostAPIFuncs.end()) {
		if(stricmp(p->sFuncName.c_str(), funcName.c_str()) == 0)
			return p;
		++p;
	}
	return vHostAPIFuncs.end();*/
	return vHostAPIFuncs.end();
}
