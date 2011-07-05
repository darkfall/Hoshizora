//
//  SoraMutex.h
//  Sora
//
//  Created by Ruiwei Bu on 7/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraMutex_h
#define Sora_SoraMutex_h

#ifndef OS_WIN32
#include <pthread.h>
#else
#include "pthread.h"
#endif

#include "SoraPlatform.h"
#include "uncopyable.h"

namespace sora {
    
    class SoraMutexLock: uncopyable {
    public:
        SoraMutexLock() {
            pthread_mutex_init(&mutex, NULL);
        }
        
        ~SoraMutexLock() {
            pthread_mutex_destroy(&mutex);
        }
        
        void lock() {
            pthread_mutex_lock(&mutex);
        }
        
        void unlock() {
            pthread_mutex_unlock(&mutex);
        }
        
        pthread_mutex_t* getPthreadMutex() {
            return &mutex;
        }
        
    private:
        pthread_mutex_t mutex;
    };
    
    class SoraMutexGuard: uncopyable {
    public:
        explicit SoraMutexGuard(SoraMutexLock& mutex): _mutex(mutex) {
            _mutex.lock();
        }
        
        ~SoraMutexGuard() {
            _mutex.unlock();
        }
        
    private:
        SoraMutexLock& _mutex;
    };
    
    // prevent use of SoraMutexGuard(my_mutex)
#define SoraMutexGuard(x) error "Missing mutex guard object name"
    
} // namespace sora


#endif
