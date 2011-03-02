/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  Redistributions of source code must retain the above copyright notice,
*  this list of conditions and the following disclaimer.
*  
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the distribution.
*  
*  Neither the name of this project's author nor the names of its
*  contributors may be used to endorse or promote products derived from
*  this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
*  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
*  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>

#include "assembler.h"

int main (int argc, char * const argv[]) {
	LSL_Assembler* assembler = new LSL_Assembler;
	
	
	int f1 = FALSE;
	
	if(argc >= 2)
		f1 = TRUE;
	
	printf("\nLoliScript System Assembler Ver 0.05\n");
	printf("By Clare Griffin (Darkfall)	2009-2010\n");
	printf("\nUsage:   loliScript_Assembler Source\n");
	printf("Output File would be \"Source\"+b\n\n");
	
	if(!f1) {
		printf("Error:	No Input File Specified\n");
		exit(0);
	}
		
	// Load the source file into memory
	
	string inputFile;
	if(f1)
		inputFile = argv[1];
	
	if(inputFile.find('.') == string::npos)
		inputFile += ".lasm";
	
	string outpitFile(inputFile);
	outpitFile += "b";
	
	printf("Assembling %s...\n\n", inputFile.c_str());
	
	assembler->loadSourceFile(inputFile.c_str());
	
	// Assemble the source file
	
	assembler->assembleSourceFile();
	
	// Dump the assembled executable to an .XSE file
	// Print out assembly statistics

	assembler->buildAssembledFile(outpitFile.c_str());
		
	assembler->printAssembleStates(outpitFile.c_str());
    
	return 0;
}


