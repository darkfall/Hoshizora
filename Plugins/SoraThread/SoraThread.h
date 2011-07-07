/*
 *  SoraThreadFactory.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_THREAD_H
#define SORA_THREAD_H

#include "SoraPlatform.h"

#include "SoraThreadImplPosix.h"

#include "SoraThreadTask.h"
#include "SoraMutex.h"
#include "SoraCondition.h"
#include "SoraThreadLocal.h"
#include "SoraBlockingQueue.h"

namespace sora {
		
	class SoraThread: public SoraThreadImpl {
		friend class SoraThreadPool;

	public:
		SoraThread();
        SoraThread(const SoraThreadTask& task, const std::string name=std::string());
        ~SoraThread();
		
        int32 start();
        int32 startWithTask(const SoraThreadTask& task);
        
        bool isActive() const;
        int32 getActiveThreadNum() const;
        
        void setThreadTask(const SoraThreadTask& task);
        SoraThreadTask getThreadTask() const;
        
        void join();
        void exit();
		
        void setName(const std::string& name);
        std::string getName() const;
		
	private:
        std::string name;
    };
		
} // namespace sora


#endif