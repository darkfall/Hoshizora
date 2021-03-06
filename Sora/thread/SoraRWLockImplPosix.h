//
//  SoraRWLockPosix.h
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraRWLockPosix_h
#define Sora_SoraRWLockPosix_h

#include "SoraPlatform.h"

#ifndef OS_WIN32
#include <pthread.h>
#else
#include "pthread.h"
#endif

#include "SoraException.h"
#include "uncopyable.h"

namespace sora {
    
    class SoraRWLockImpl: public uncopyable {
    protected:
        SoraRWLockImpl() {
            pthread_rwlock_init(&rwl, NULL);
        }
        
        ~SoraRWLockImpl() {
            pthread_rwlock_destroy(&rwl);
        }
        
        inline void readLockImpl() {
            if(pthread_rwlock_rdlock(&rwl))
                THROW_SORA_EXCEPTION("Cannot lock reader/writer lock");
        }
        
        inline void writeLockImpl() {
            if(pthread_rwlock_wrlock(&rwl))
                THROW_SORA_EXCEPTION("Cannot lock reader/writer lock");
        }
        
        inline void unlockImpl() {
            if(pthread_rwlock_unlock(&rwl))
                THROW_SORA_EXCEPTION("Cannot unlock reader/writer lock");
        }
        
    private:
        pthread_rwlock_t rwl;
    }
    
}

#endif
