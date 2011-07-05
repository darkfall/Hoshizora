/*
 *  SoraThreadFactory.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraThread.h"

namespace sora {
    
    int32 SoraThread::active_thread_num = 0;

    SoraThread::SoraThread(): _arg(NULL), thread_task(NULL), active(false) {}
    
    SoraThread::SoraThread(SoraThreadTask* task): thread_task(task), _arg(NULL), active(false) {}
    
    SoraThread::~SoraThread() {
        if(isActive()) {
            pthread_join(thread, 0);
            --active_thread_num;
        }
    }
    
    void SoraThread::setThreadTask(SoraThreadTask* task) {
        thread_task = task;
    }
    
    bool SoraThread::isActive() const {
        return active;
    }
    
    int32 SoraThread::getActiveThreadNum() const {
        return active_thread_num;
    }
    
    SoraThreadTask* SoraThread::getThreadTask() const {
        return thread_task;
    }
    
    int32 SoraThread::start(void* arg) {
        if(!active) {
            setarg(arg);
            int32 result = pthread_create(&thread, NULL, SoraThread::entry, this);
            return result;
        }
        return 0;
    }
    
    int SoraThread::run(void* arg) {
        setup();
        execute(arg);
        return 1;
    }
    
    void SoraThread::join() {
        if(active)
            pthread_join(thread, 0);
    }
    
    void* SoraThread::getarg() const {
        return _arg;
    }
    
    void SoraThread::setarg(void* a) {
        _arg = a;
    }
    
    void SoraThread::setActive(bool flag) {
        active = flag;
    }
    
    void* SoraThread::entry(void* pthis) {
        SoraThread* pt = static_cast<SoraThread*>(pthis);
        
        ++active_thread_num;
        pt->setActive(true);
        
        if(pt->getThreadTask() == NULL)
            pt->run( pt->getarg() );
        else 
            pt->getThreadTask()->run( pt->getarg() );
        
        pthread_exit(0);
        pt->setActive(false);
        --active_thread_num;
        return 0;
    }
	
} // namespace sora
