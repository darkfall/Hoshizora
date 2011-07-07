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
    
    class SoraCondition {
    public:
        explicit SoraCondition(SoraMutexLock& mutex):
        _mutex(mutex) {
            pthread_cond_init(&cond, NULL);
        }
        
        ~SoraCondition() {
            pthread_cond_destroy(&cond);
        }
        
        inline void wait() {
            if(pthread_cond_wait(&cond,
                                 static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
                THROW_SORA_EXCEPTION("Error wait");
        }
        
        inline void notify() {
            if(pthread_cond_signal(&cond))
                THROW_SORA_EXCEPTION("Error notify");
        }
        
        inline void notifyAll() {
            if(pthread_cond_broadcast(&cond))
                THROW_SORA_EXCEPTION("Error notify all");
        }
        
    private:
        pthread_cond_t cond;
        
    protected:
        SoraMutexLock& _mutex;
    };
    
} // namespace sora


#endif
