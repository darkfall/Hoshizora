#include <iostream>

#include "../Compiler/compiler.h"
#include "../Assembler/assembler.h"

#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main (int argc, char * const argv[]) {
   /* for(int i=1; i<argc; ++i)  {
        string fileName = argv[i];
        if(fileName.find(".lst") != string::npos) {
            vector<string> vList;
		
            ifstream inListFile(argv[i]);
            if(inListFile.is_open()) {
                string tmp;
                while(getline(inListFile, tmp)) {
                    if(tmp.find(".txt") != string::npos)
                        vList.push_back(tmp);
                }
            }
            inListFile.close();
		
            for(int j=0; j<vList.size(); ++j) {
                if(vList[i].size() < 3) continue;
                
                LSH_Compiler* pCompiler = new LSH_Compiler;
                LSL_Assembler* pAssembler = new LSL_Assembler;
		
                pCompiler->loadSourceFile(vList[j].c_str());
                pCompiler->compileSourceFile();
		
                string sFile = vList[j];
                //sFile.erase(0, sFile.rfind('/')+1);
                cout<<"Compiling: "<<(sFile+".lasm")<<endl;
                pCompiler->emitCode((sFile+".lasm").c_str());
		
                pAssembler->loadSourceFile((sFile+".lasm").c_str());
                pAssembler->assembleSourceFile();
		
                cout<<"Assembling: "<<(sFile+".lasmb")<<endl;
                pAssembler->buildAssembledFile((sFile+".lasmb").c_str());
                
                delete pCompiler;
                delete pAssembler;
            }
        } else {
            LSH_Compiler* pCompiler = new LSH_Compiler;
            LSL_Assembler* pAssembler = new LSL_Assembler;
            
            string sFile = argv[i];
            
            pCompiler->loadSourceFile(sFile.c_str());
            pCompiler->compileSourceFile();
            
            //sFile.erase(0, sFile.rfind('/')+1);
            cout<<"Compiling: "<<(sFile+".lasm")<<endl;
            pCompiler->emitCode((sFile+".lasm").c_str());
            
            pAssembler->loadSourceFile((sFile+".lasm").c_str());
            pAssembler->assembleSourceFile();
            
            cout<<"Assembling: "<<(sFile+".lasmb")<<endl;
            pAssembler->buildAssembledFile((sFile+".lasmb").c_str());
            
            delete pCompiler;
            delete pAssembler;
        }
	}*/
    system("~/Desktop/kuiCompiler  ~/Desktop/start.txt  ~/Desktop/initialize.txt");
    return 0;
}
