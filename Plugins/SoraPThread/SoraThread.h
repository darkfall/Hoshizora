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

// sora thread using pthread(posix thread)
#ifndef OS_WIN32
#include <pthread.h>
#else
#include "pthread.h"

#pragma comment(lib, "pthreadVC2.lib")
#endif

#include "SoraThreadTask.h"
#include "SoraMutex.h"
#include "SoraCondition.h"
#include "SoraThreadLocal.h"
#include "SoraBlockingQueue.h"

namespace sora {
		
	class SoraThread: uncopyable {
		friend class SoraThreadPool;

	public:
		SoraThread();
        SoraThread(const SoraThreadTask& task, const std::string name=std::string());
        ~SoraThread();
		
		int32 start(void* arg);
        int32 start(const SoraThreadTask& task);
        
        bool isActive() const;
        int32 getActiveThreadNum() const;
        
        void setThreadTask(const SoraThreadTask& task);
        /**
         * only available when using external task routine
         **/
        SoraThreadTask getThreadTask() const;
        
        void join();
		
        void setName(const std::string& name);
        std::string getName() const;
        
	protected:
		int32 run(void* arg);
		static void* entry(void*);
		
		virtual void setup() {}
		virtual void execute(void*) {}
		
		void* getarg() const;
		void setarg(void* a);
        
        void setActive(bool flag);
		
	private:
		pthread_t thread;
		pthread_attr_t attr;
        
        SoraThreadTask thread_task;
        
        static int32 active_thread_num;
		
        std::string name;
		void* _arg;
        bool active;
	};
		
} // namespace sora


#endif