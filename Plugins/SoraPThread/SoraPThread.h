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

namespace sora {
		
	class SoraThread {
		friend class SoraThreadPool;

	public:
		SoraThread();
		
		int32 start(void* arg);
		
	protected:
		int32 run(void* arg);
		static void* entry(void*);
		
		virtual void setup() = 0;
		virtual void execute(void*) = 0;
		
		void* getarg() const { return _arg; }
		void setarg(void* a) { _arg = a; }
		
	private:
		pthread_t thread;
		pthread_attr_t attr;
		
		void* _arg;
	};
		
} // namespace sora


#endif