#include <iostream>

#include "vm_scriptGroup.h"

vm_scriptGroup* scriptReader;

void hostAdd(int iCurrThread) {
	int a = scriptReader->getParamAsInt(iCurrThread, 0);
	int b = scriptReader->getParamAsInt(iCurrThread, 1);
	
	printf("\n  ****HOST: Get Params From Script stack: %d,  %d\n\n", a, b);
	
	scriptReader->returnIntFromHost(iCurrThread, 2, a+b);
}

void hostPrintString(int iCurrThread) {
	char* str = scriptReader->getParamAsString(iCurrThread, 0);
	int t = scriptReader->getParamAsInt(iCurrThread, 1);
	
	for(int i=0; i<t; ++i)
		printf("%s\n", str);
	
	scriptReader->returnFromHost(iCurrThread, 2);
}

int main (int argc, char * const argv[]) {
	printf("LoliScript VirtualMachine Ver0.02\n");
	printf("Clare Griffin (darkfall) ring0Dev 2009.12.28\n");
	printf("\nBinary Script Code Reading....\n");
	
    scriptReader = new vm_scriptGroup;

	int iThread = -1;
	iThread = scriptReader->loadScript("test.lasmb");
//	iThread = scriptReader->loadScript("t1.lasmb");
	scriptReader->registerHostAPIFunc(iThread, "hostAdd", hostAdd);
///	scriptReader->registerHostAPIFunc(iThread, "hostPrintString", hostPrintString);
//	scriptReader->loadScript("t3.lasmb");
//	scriptReader->loadScript("t3.lasmb");
	
//	scriptReader->runScript(0);
	
//	if(!scriptReader->invokeScriptFunc(iThread, "testfunc"))
//		printf("Call Script Func Failed");
//	scriptReader->runScript(1);
	scriptReader->runScript(iThread);
//	scriptReader->runScript(2);
//	scriptReader->resetScript();
	scriptReader->run();
	
//	scriptReader->callScriptFunc(iThread, "testFunc");
//	printf("\nGet return val from script call, %d\n\n", scriptReader->getReturnValueAsInt(iThread));
	
//	scriptReader->callScriptFunc(iThread, "testFunc2");
//	printf("\nGet return val from script call, %s\n\n", scriptReader->getReturnValueAsString(iThread));;
	
	
    return 0;
}
