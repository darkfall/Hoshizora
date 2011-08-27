//
//  SoraMutex.h
//  Sora
//
//  Created by Ruiwei Bu on 7/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMutex_h
#define Sora_SoraMutex_h

#include "SoraPlatform.h"
#include "uncopyable.h"


#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
#include "SoraMutexImplPosix.h"
#else
#include "SoraMutexImplWin32.h"
#endif


namespace sora {
    
    class SoraMutexLock: SoraMutexLockImpl {
    public:
        SoraMutexLock() {}
        
        ~SoraMutexLock() {}
        
        void lock() {
            lockImpl();
        }
        
        void unlock() {
            unlockImpl();
        }
        
        void* getSysMutex() {
            return getSysMutexImpl();
        }
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
