//
//  SoraCondition.h
//  Sora
//
//  Created by Ruiwei Bu on 7/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraCondition_h
#define Sora_SoraCondition_h

#include "SoraMutex.h"

#ifdef OS_WIN32
#include "SoraConditionImplWin32.h"
#endif

namespace sora {
    
    class SoraCondition: public uncopyable {
    public:
        explicit SoraCondition(SoraMutexLock& mutex):
        _mutex(mutex) {
#if defined(OS_WIN32) && !defined(SORA_WIN32_PTHREAD)
			_win32_pthread_cond_init(&cond, NULL);
#else
            pthread_cond_init(&cond, NULL);
#endif
        }
        
        ~SoraCondition();
        void wait();
        void notify();
        void notifyAll();
        
    private:
        pthread_cond_t cond;
        
    protected:
        SoraMutexLock& _mutex;
    };
    
} // namespace sora


#endif
