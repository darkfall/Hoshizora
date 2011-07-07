//
//  SoraThreadImplWin32.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraThreadImplWin32.h"

#ifdef OS_WIN32

namespace sora {
    
    DWORD WINAPI SoraThreadImpl::_ThreadProc(LPVOID param) {
        SoraThreadImpl* pt = static_cast<SoraThreadImpl*>(param);
        if(pt) {
            ++active_thread_num;
            pt->setActiveImpl(true);
            
            SoraThreadTask task(pt->getThreadTaskImpl());
            if(task.isValid())
                task( pt->getargImpl() );
            
            pt->setActiveImpl(false);
            --active_thread_num;
            return 0;
        }
    }
    
    int32 SoraThreadImpl::active_thread_num = 0;
    
    SoraThreadImpl::SoraThreadImpl():
    thread_handle(NULL),
    thread_id(0),
    active(false) {
    }
    
    SoraThreadImpl::~SoraThreadImpl() {
        if(thread_handle == NULL)
            return;
        exit();
        thread_id = 0;
        CloseHandle(thread_handle);
    }
    
    int32 SoraThreadImpl::startImpl() {
        if(active || !thread_task.isValid())
            return 0;
        
        thread_handle = CreateThread(NULL, 0, _ThreadProc, (LPVOID)this);
        if(thread_handle == INVALID_HANDLE_VALUE || thread_handle == NULL) {
            thread_id = 0;
            return 0;
        }
        return 1;
    }
    
    int32 SoraThreadImpl::startWithTaskImpl(const SoraThreadTask& task) {
        setThreadTaskImpl(task);
        startImpl();
    }
    
    void SoraThreadImpl::joinImpl() {
        WaitForSingleObject(thread_handle, INFINITE);
    }
    
    void SoraThreadImpl::exitImpl() {
        TerminateThread(thread_handle, 0);
    }
    
    bool SoraThreadImpl::isActiveImpl() const {
        return active;
    }
    
    int32 SoraThreadImpl::getActiveThreadNumImpl() const {
        return active_thread_num;
    }
    
    void SoraThreadImpl::setThreadTaskImpl(const SoraThreadTask& task) {
        thread_task = task;
    }
    
    SoraThreadTask SoraThreadImpl::getThreadTaskImpl() const {
        return thread_task;
    }
    
} // namespace sora

#endif