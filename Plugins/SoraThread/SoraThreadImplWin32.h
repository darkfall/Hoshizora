//
//  SoraThreadImplWin32.h
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraThreadImplWin32_h
#define Sora_SoraThreadImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

#include "SoraThreadTask.h"

namespace sora {
    
    class SoraThreadImpl {
    protected:
        SoraThreadImpl();
        ~SoraThreadImpl();
        
        int32 startImpl();
        int32 startWithTaskImpl(const SoraThreadTask& task);
        
        bool isActiveImpl() const;
        int32 getActiveThreadNumImpl() const;
        
        void setThreadTaskImpl(const SoraThreadTask& task);
        SoraThreadTask getThreadTaskImpl() const;
        
        void join();
        void exit();
        
    private:
        uint32 thread_id;
        HANDLE thread_handle;
        
        SoraThreadtask thread_task;
        static DWORD WINAPI _ThreadProc(LPVOID param);
        
        static int32 active_thread_num;
        void* arg;
        bool active;
    };
    
    
} // namespace sora

#endif

#endif
