//
//  SoraSemaphoreImplPosix.h
//  Sora
//
//  Created by Ruiwei Bu on 7/8/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSemaphoreImplPosix_h
#define Sora_SoraSemaphoreImplPosix_h

#include "SoraCondition.h"

namespace sora {
    
    
    class SoraSemaphoreImpl: public uncopyable {
    public:
        SoraSemaphoreImpl(int _n, int _max):
        mutex(),
        cond(mutex),
        n(_n),
        max(_max) {
            assert( n >= 0 && max > 0 && n <= max);
        }
        
        ~SoraSemaphoreImpl() {
            
        }
        
        void setImpl() {
            SoraMutexGuard lock(mutex);
            if(n < max) {
                ++n;
            } else {
                THROW_SORA_EXCEPTION("Cannot signal semaphore, exceed maximun");
            }
            cond.notify();
        }
        
        void waitImpl() {
            SoraMutexGuard lock(mutex);
            while(n < 1) {
                cond.wait();
            }
            --n;
        }
        
    private:
        
        volatile int n;
        int max;
        
        mutable SoraMutexLock mutex;
        SoraCondition cond;
    };
}

#endif