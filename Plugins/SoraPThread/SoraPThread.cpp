/*
 *  SoraThreadFactory.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraPThread.h"

namespace sora {

SoraThread::SoraThread() {}

int32 SoraThread::start(void* arg) {
	setarg(arg);
	int32 result = pthread_create(&thread, NULL, SoraThread::entry, this);
	return result;
}

int SoraThread::run(void* arg) {
	setup();
	execute(arg);
	return 1;
}

void* SoraThread::entry(void* pthis) {
	SoraThread* pt = static_cast<SoraThread*>(pthis);
	pt->run( pt->getarg() );
	
	pthread_exit(0);
	return 0;
}
	
} // namespace sora
