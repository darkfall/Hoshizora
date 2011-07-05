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

namespace sora {
    
    class SoraCondition: uncopyable {
    public:
        explicit SoraCondition(SoraMutexLock& mutex): _mutex(mutex) {
            pthread_cond_init(&cond, NULL);
        }
        
        ~SoraCondition() {
            pthread_cond_destroy(&cond);
        }
        
        void wait() {
            pthread_cond_wait(&cond, _mutex.getPthreadMutex());
        }
        
        void notify() {
            pthread_cond_signal(&cond);
        }
        
        void notifyAll() {
            pthread_cond_broadcast(&cond);
        }
        
    private:
        SoraMutexLock& _mutex;
        
        pthread_cond_t cond;
    };
    
} // namespace sora


#endif
