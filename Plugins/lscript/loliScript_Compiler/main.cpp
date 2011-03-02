#include <iostream>

#include "compiler.h"
#include "assembler.h"

#include "vm_scriptGroup.h"
vm_scriptGroup* vm;
int iScriptIndex;


void hostAdd(int iThreadIndex, int iHostID) {
	int a = vm->getParamAsInt(iThreadIndex, 0);
	int b = vm->getParamAsInt(iThreadIndex, 1);
	
	vm->returnIntFromHost(iThreadIndex, 1, a+b);
}

int main (int argc, char * const argv[]) {
	//compile script
    LSH_Compiler* compiler = new LSH_Compiler;
	compiler->loadSourceFile("untitled.txt");
	compiler->compileSourceFile();
	compiler->emitCode("test.lasm");
	
	//assemble script
	LSL_Assembler* assembler = new LSL_Assembler;
	assembler->loadSourceFile("test.lasm");
	assembler->assembleSourceFile();
	assembler->buildAssembledFile("test.lasmb");
		
	//run script
	vm = new vm_scriptGroup(0);
	iScriptIndex = vm->loadScript("test.lasmb");
	
	vm->registerHostAPIFunc(-1, "hostadd", hostAdd); 
	vm->runScript(iScriptIndex);
	vm->run();

	
	return 0;
}
