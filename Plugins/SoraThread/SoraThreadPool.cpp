//
//  SoraThreadPool.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/6/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraThreadPool.h"
#include "common.h"
#include "Debug/SoraInternalLogger.h"

#include <stdio.h>
#include <algorithm>

namespace sora {
    
    SoraThreadPool::SoraThreadPool(const std::string& _name):
    mutex(),
    cond(mutex),
    name(_name),
    running(false) {
        
    }
    
    SoraThreadPool::~SoraThreadPool() {
        running = false;
        std::for_each(threads.begin(), threads.end(), DeleteSTLPtr());
    }
    
    void SoraThreadPool::start(int32 numThreads) {
        if(!threads.empty())
            THROW_SORA_EXCEPTION(ExistsException, "Threads already running in pool!");
        
        running = true;
        threads.reserve(numThreads);
        for(int i=0; i<numThreads; ++i) {
            char id[32];
            snprintf(id, sizeof id, "%d", i);
            
            SoraThreadTask task;
            task.setAsMemberFunc(&SoraThreadPool::runInThread, this);
            threads.push_back(new SoraThread(task, id));
            threads[i]->start();
        }
    }
    
    void SoraThreadPool::stop() {
        running = false;
        cond.notifyAll();
        
        THREAD_LIST::iterator itThread = threads.begin();
        while(itThread != threads.end()) {
            (*itThread)->join();
            ++itThread;
        }
    }
    
    void SoraThreadPool::run(SoraThreadTask& task) {
        if(threads.empty())
            task();
        else {
            SoraMutexGuard lock(mutex);
            queue.push_back(task);
            cond.notify();
        }
    }
    
    SoraThreadTask SoraThreadPool::take() {
        SoraMutexGuard lock(mutex);
        while(queue.empty() && running)
            cond.wait();
    
        SoraThreadTask task;
        if(!queue.empty()) {
            task = queue.front();
            queue.pop_front();
        }
        return task;
    }
    
    void SoraThreadPool::runInThread(void* arg) {
        try {
            while(running) {
                SoraThreadTask task(take());
                task();
            }
        }
        catch(const SoraException& exp) {
            log_mssg(exp.what());
        }
    }
    
    
} // namespace sora
