/*
 *  SoraThread.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraThread.h"

namespace sora {
    
    SoraThread::SoraThread() {
    
    }
    
    SoraThread::SoraThread(const SoraThreadTask& task, const std::string _name):
    name(_name) {
        setThreadTaskImpl(task);
    }
    
    SoraThread::~SoraThread() {
        
    }
    
    void SoraThread::setName(const std::string& _name) {
        name = _name;
    }
    
    std::string SoraThread::getName() const {
        return name;
    }
    
    void SoraThread::setThreadTask(const SoraThreadTask& task) {
        setThreadTaskImpl(task);
    }
    
    bool SoraThread::isActive() const {
        return isActiveImpl();
    }
    
    int32 SoraThread::getActiveThreadNum() const {
        return getActiveThreadNumImpl();
    }
    
    SoraThreadTask SoraThread::getThreadTask() const {
        return getThreadTaskImpl();
    }
    
    int32 SoraThread::start() {
       return startImpl();
    }
    
    int32 SoraThread::startWithTask(const SoraThreadTask& task) {
        return startWithTaskImpl(task);
    }
    
    void SoraThread::join() {
        joinImpl();
    }
	
    void SoraThread::exit() {
        exitImpl();
    }
} // namespace sora
