//
//  WindowsStackTrace.h
//  Sora
//
//  Created by Ruiwei Bu on 7/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_WindowsStackTrace_h
#define Sora_WindowsStackTrace_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

// use dbghelp to get stack frames
#include <dbghelp.h>

#pragma comment( lib, “dbghelp.lib” )

namespace sora {
    
    const char* dump_callstack( CONTEXT *context ) {
        STACKFRAME sf;
        memset( &sf, 0, sizeof( STACKFRAME ) );
        
        sf.AddrPC.Offset = context->Eip;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrStack.Offset = context->Esp;
        sf.AddrStack.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = context->Ebp;
        sf.AddrFrame.Mode = AddrModeFlat;
        
        DWORD machineType = IMAGE_FILE_MACHINE_I386;
        
        HANDLE hProcess = GetCurrentProcess();
        HANDLE hThread = GetCurrentThread();
        
        std::string stackTrace;
        for( ; ; )
        {
            if( !StackWalk(machineType, hProcess, hThread, &sf, context, 0, SymFunctionTableAccess, SymGetModuleBase, 0 ) )
            {
                break;
            }
            
            if( sf.AddrFrame.Offset == 0 )
            {
                break;
            }
            BYTE symbolBuffer[ sizeof( SYMBOL_INFO ) + 1024 ];
            PSYMBOL_INFO pSymbol = ( PSYMBOL_INFO ) symbolBuffer;
            
            pSymbol->SizeOfStruct = sizeof( symbolBuffer );
            pSymbol->MaxNameLen = 1024;
            
            DWORD64 symDisplacement = 0;
            if( SymFromAddr( hProcess, sf.AddrPC.Offset, 0, pSymbol ) )
            {
                stackTrace.append("Function: ");
                stackTrace.apeend(pSymbol->Name);
                stackTrace.apeend("\n");
            }
        }
        return stackTrace.c_str();
    }
    
    const char* excep_filter()
    {
        /// init dbghelp.dll
        SymInitialize( GetCurrentProcess(), NULL, TRUE );
        
        const char* result = dump_callstack( GetExceptionInformation()->ContextRecord );
        
        SymCleanup( GetCurrentProcess() );
        
        return result;
    }

} // namespace sora

#endif // OS_WIN32

#endif
