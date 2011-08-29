//
//  WindowsStackTrace.h
//  Sora
//
//  Created by Robert Bu on 7/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_WindowsStackTrace_h
#define Sora_WindowsStackTrace_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

// use dbghelp to get stack frames
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

#include <string>
#include "SoraLogger.h"

/**
 * Retrived From
 * http://hi.baidu.com/zhoutianyang/blog/item/6fd19f0adbacba1894ca6bab.html
 * Modified by darkfall
 **/

namespace sora {

	#define INVALID_FP_RET_ADDR_VALUE 0x0000000
    
    static std::string DisplaySymbolDetails(DWORD dwAddress) {
       DWORD64 displacement = 0;
	   ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME*sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];

       PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	   pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	   pSymbol->MaxNameLen = MAX_SYM_NAME;

	   std::string info;

	   if (SymFromAddr(GetCurrentProcess(), dwAddress, &displacement, pSymbol)) {
			IMAGEHLP_MODULE64 moduleinfo;
			moduleinfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
			if(SymGetModuleInfo64(GetCurrentProcess(), pSymbol->Address, &moduleinfo)) {
				info.append(moduleinfo.ModuleName);
			} else 
				info.append("<Error_Module_Info>");

			if (pSymbol->MaxNameLen > 0) {
				info.append(pSymbol->Name);
			}
			else {
				info.append("<Unknown_Function>");
			}

       } else { 
		   info.append(" <Unable to get symbol details>");
	   }
	   return info;
	}

	static std::string WalkTheStack(){
       DWORD _ebp = INVALID_FP_RET_ADDR_VALUE;
	   DWORD dwIPOfCurrentFunction = (DWORD)&WalkTheStack;

       // Get the current Frame pointer
       __asm {
		   mov [_ebp], ebp
	   }

       // We cannot walk the stack (yet!) without a frame pointer
       if (_ebp == INVALID_FP_RET_ADDR_VALUE)
		   return false;

       //current Frame Pointe
       DWORD *pCurFP = (DWORD *)_ebp;
	   BOOL fFirstFP = TRUE;

	   std::string stackTrace("CurrFP\t\tRetAddr\t\tFunction\n");
       while (pCurFP != INVALID_FP_RET_ADDR_VALUE) {
              DWORD pRetAddrInCaller = (*((DWORD *)(pCurFP + 1)));

              stackTrace.append(vamssg("%.8p%*p        ", pCurFP, 20, (DWORD *)pRetAddrInCaller));

			  if (fFirstFP) {
				  fFirstFP = FALSE;
			  }
			  stackTrace.append(DisplaySymbolDetails(dwIPOfCurrentFunction));

			  dwIPOfCurrentFunction = pRetAddrInCaller;

			  stackTrace.append("\n");
			  if (pRetAddrInCaller == INVALID_FP_RET_ADDR_VALUE) {
				  break;
			  }
			  DWORD pCallerFP = *((DWORD *)pCurFP);
			  pCurFP = (DWORD *)pCallerFP;
	   }
	   return stackTrace;
	}

	static std::string getStackTrace() {
		HANDLE process = GetCurrentProcess();
		if(SymInitialize(process, NULL, TRUE)) {
			std::string result = WalkTheStack();
			SymCleanup(process);
			
			return result;
		}
		return "";
	}
} // namespace sora

#endif // OS_WIN32

#endif
