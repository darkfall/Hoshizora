#include <iostream>

#include "compiler.h"
#include "assembler.h"

#include "vm_scriptGroup.h"
vm_scriptGroup* vm;
int iScriptIndex;

#include <string>
using std::string;

void hostAdd(int iThreadIndex, int iHostID) {
	int a = vm->getParamAsInt(iThreadIndex, 0);
	int b = vm->getParamAsInt(iThreadIndex, 1);
	
	vm->returnIntFromHost(iThreadIndex, 1, a+b);
}

int main (int argc, char * const argv[]) {
    for(int i=1; i<argc; ++i) {
        //compile script
        LSH_Compiler* compiler = new LSH_Compiler;
        compiler->loadSourceFile(argv[i]);
        compiler->compileSourceFile();
        string fileName = argv[i];
        compiler->emitCode((fileName+".lasm").c_str());
	
        //assemble script
        LSL_Assembler* assembler = new LSL_Assembler;
        assembler->loadSourceFile((fileName+".lasm").c_str());
        assembler->assembleSourceFile();
        assembler->buildAssembledFile((fileName+".lasmb").c_str());
    }
	
    string fileName = "start.txt";

    LSH_Compiler* compiler = new LSH_Compiler;
    compiler->loadSourceFile(fileName.c_str());
    compiler->compileSourceFile();
    compiler->emitCode((fileName+".lasm").c_str());
	
    //assemble script
    LSL_Assembler* assembler = new LSL_Assembler;
    assembler->loadSourceFile((fileName+".lasm").c_str());
    assembler->assembleSourceFile();
    assembler->buildAssembledFile((fileName+".lasmb").c_str());
    ////run script
//	vm = new vm_scriptGroup(0);
//	iScriptIndex = vm->loadScript("test.lasmb");
	
//	vm->registerHostAPIFunc(-1, "hostadd", hostAdd); 
//	vm->runScript(iScriptIndex);
//	vm->run();

	return 0;
}
