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
    
    class SoraCondition {
    public:
        explicit SoraCondition(SoraMutexLock& mutex):
        _mutex(mutex) {
#if defined(OS_WIN32) && !defined(SORA_WIN32_PTHREAD)
			_win32_pthread_cond_init(&cond, NULL);
#else
            pthread_cond_init(&cond, NULL);
#endif
        }
        
        ~SoraCondition() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
            pthread_cond_destroy(&cond);
#else
			//_win32_pthread_cond_destroy(&cond);
#endif
        }
        
        inline void wait() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
            if(pthread_cond_wait(&cond,
                                 static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
                THROW_SORA_EXCEPTION("Error wait");
#else
			if(_win32_pthread_cond_wait(&cond,
                                 static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
				THROW_SORA_EXCEPTION("Error wait");
#endif
        }
        
        inline void notify() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
            if(pthread_cond_signal(&cond))
                THROW_SORA_EXCEPTION("Error notify");
#else
			if(_win32_pthread_cond_signal(&cond))
                THROW_SORA_EXCEPTION("Error notify");
#endif
        }
        
        inline void notifyAll() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
            if(pthread_cond_broadcast(&cond))
                THROW_SORA_EXCEPTION("Error notify all");
#else
			if(_win32_pthread_cond_broadcast(&cond))
                THROW_SORA_EXCEPTION("Error notify all");
#endif
        }
        
    private:
        pthread_cond_t cond;
        
    protected:
        SoraMutexLock& _mutex;
    };
    
} // namespace sora


#endif
